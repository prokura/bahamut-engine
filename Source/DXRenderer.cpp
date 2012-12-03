#include "Renderer.h"
#include "Bitmap.h"
#include "WindowHandler.h"
#include <assert.h>
#include "config.h"
#include <d3dx10.h>

///*** DirectXValue ***
static ID3D10Device*				dx_device				= NULL;
static IDXGISwapChain*				dx_swap_chain			= NULL;
static ID3D10RenderTargetView*		dx_render_target		= NULL;
static D3D10_VIEWPORT				dx_viewport;
ID3D10RasterizerState*				dx_rasterizer_state		= NULL;

static D3DXMATRIX					dx_view_matrix;
static D3DXMATRIX					dx_projection_matrix;
static float						clear_color[4]			={ 0,0,0,1 };

static ID3D10Buffer*				dx_vertex_buffer		= NULL;
static ID3D10InputLayout*			dx_vertex_layout		= NULL;

//effects and techniques
static ID3D10Effect*						dx_basic_effect			= NULL;
static ID3D10EffectTechnique*				effect_technique		= NULL;
static ID3D10EffectMatrixVariable*			effect_mat_view			= NULL;
static ID3D10EffectMatrixVariable*			effect_mat_proj			= NULL;
static ID3D10EffectMatrixVariable*			effect_mat_world		= NULL;
static ID3D10EffectShaderResourceVariable*	effect_texture			= NULL;

struct vertex
{
      D3DXVECTOR3 pos;
      D3DXVECTOR4 color;
	  D3DXVECTOR2 texCoord;

      vertex( D3DXVECTOR3 p, D3DXVECTOR4 c, D3DXVECTOR2 uv ) : pos(p), color(c), texCoord(uv) {}
};

bool Init_Device( int width, int height );
bool Init_Shader();
bool Init_Vertex_Buffer();
void Setup_Rasterization();
void Cleanup_Device();


//*** Texture Interface ***

struct Bitmap
{
	ID3D10ShaderResourceView* texture;
};

static unsigned bitmap_next = 0;
static unsigned bitmap_freelist[ MAX_BITMAPS ];
static unsigned bitmap_freelist_count = 0;
static Bitmap bitmaps[ MAX_BITMAPS ];

//*** Create bitmaps ***

BitmapId Create_Bitmap( const char* filename )
{
	unsigned handle = 0;

	// Create from the free list if we can
	if( bitmap_freelist_count > 0 )
	{
		handle = bitmap_freelist[ bitmap_freelist_count - 1 ];
		bitmap_freelist_count--;
	}
	else // And only expand the range if free list is empty
	{
		assert( bitmap_next < MAX_BITMAPS);
		handle = bitmap_next;
		bitmap_next++;
	}

	Bitmap* bitmap = &bitmaps[ handle ];
	 
	HRESULT load_texture = D3DX10CreateShaderResourceViewFromFile( dx_device, filename, NULL, NULL, &bitmap->texture, NULL );
	assert( SUCCEEDED( load_texture ) );

	return handle;
}

//*** Destroy bitmaps ***

void Destroy_Bitmap( BitmapId bitmap )
{
	assert( bitmap < MAX_BITMAPS );

	bitmap_freelist[ bitmap_freelist_count ] = bitmap;
	bitmap_freelist_count++;
	bitmaps[ bitmap ].texture->Release();
}

//*** Clear All bitmaps ***

void Clear_All_Bitmap()
{
	for( unsigned nBitmap = 0 ; nBitmap < bitmap_next; nBitmap++ )
	{
		Destroy_Bitmap( nBitmap );
	}
}

//*** Initialise ***

bool Renderer_Init( unsigned width, unsigned height, const char* title )
{
	Window_Init( width, height, title, FULL_SCREEN );

	if( !Init_Device(width,height) )
    {
        Cleanup_Device();
        return false;
    }

	if( !Init_Shader() ) return false;

	if( !Init_Vertex_Buffer() ) return false;

	Setup_Rasterization();

	return true;
}

//*** Terminate ***

void Renderer_Terminate()
{
	Clear_All_Bitmap();
	Cleanup_Device();
	Window_Destroy();
}

//*** Draw ***

void Renderer_Draw()
{
	//clear scene
	dx_device->ClearRenderTargetView( dx_render_target, clear_color );

	//create world matrix
	D3DXMATRIX w;
	D3DXMatrixIdentity(&w);
	D3DXMatrixTranslation(&w,100,0,0);

	//set effect matrices
	effect_mat_world->SetMatrix( w );
	effect_texture->SetResource( bitmaps[0].texture );

	//fill vertex buffer with vertices
	UINT numVertices = 4;
	vertex* v = NULL;

	//lock vertex buffer for CPU use
	dx_vertex_buffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**) &v );

	v[0] = vertex( D3DXVECTOR3(-100,-100,0),D3DXVECTOR4(100,0,0,100),D3DXVECTOR2(0.0f, 1.0f) );
	v[1] = vertex( D3DXVECTOR3(-100,100,0),D3DXVECTOR4(0,100,0,100),D3DXVECTOR2(0.0f, 0.0f) );
	v[2] = vertex( D3DXVECTOR3(100,-100,0),D3DXVECTOR4(0,0,100,100),D3DXVECTOR2(1.0f, 1.0f) );
	v[3] = vertex( D3DXVECTOR3(100,100,0),D3DXVECTOR4(100,100,0,100),D3DXVECTOR2(1.0f, 0.0f) );

	dx_vertex_buffer->Unmap();

	// Set primitive topology 
	dx_device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//get technique desc
	D3D10_TECHNIQUE_DESC techDesc;
	effect_technique->GetDesc( &techDesc );
	
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		//apply technique
		effect_technique->GetPassByIndex( p )->Apply( 0 );
				
		//draw
		dx_device->Draw( numVertices, 0 );
	}


	//flip buffers
	dx_swap_chain->Present(0,0);
}


//*** Init Directx Device ***

bool Init_Device( int width, int height )
{
	//Set up DX swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//set buffer dimensions and format
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;;

	//set refresh rate
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	//sampling settings
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;

	//output window handle
	swapChainDesc.OutputWindow = Window_GetHWND();
	swapChainDesc.Windowed = true;

	//Create the D3D device
	if ( FAILED( D3D10CreateDeviceAndSwapChain(	NULL,D3D10_DRIVER_TYPE_HARDWARE,NULL,0,D3D10_SDK_VERSION,&swapChainDesc,&dx_swap_chain,&dx_device) ) )
			return false;

	//try to get the back buffer
	ID3D10Texture2D* pBackBuffer;

	if ( FAILED( dx_swap_chain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer) ) ) return false;

	//try to create render target view
	if ( FAILED( dx_device->CreateRenderTargetView(pBackBuffer, NULL, &dx_render_target) ) ) return false;

	//release the back buffer
	pBackBuffer->Release();

	//set the render target
	dx_device->OMSetRenderTargets(1, &dx_render_target, NULL);

	//create viewport structure
	dx_viewport.Width = width;
	dx_viewport.Height = height;
	dx_viewport.MinDepth = 0.0f;
	dx_viewport.MaxDepth = 1.0f;
	dx_viewport.TopLeftX = 0;
	dx_viewport.TopLeftY = 0;

	//set the viewport
	dx_device->RSSetViewports(1, &dx_viewport);

	// Set up the view matrix
	D3DXVECTOR3 eye(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 view(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH( &dx_view_matrix, &eye, &view, &up );

	//Set up projection matrix
	D3DXMatrixOrthoLH(&dx_projection_matrix, (float)width, (float)height, 0.0f, 1.0f );
	//D3DXMatrixPerspectiveFovLH(&dx_projection_matrix, (float)D3DX_PI * 0.5f, (float)width/height, 0.1f, 100.0f);

	return true;
}

//*** Init Shader ***

bool Init_Shader()
{
	if ( FAILED( D3DX10CreateEffectFromFile( "Data\\Shader\\texture.fx",NULL,NULL,
		"fx_4_0",D3D10_SHADER_ENABLE_STRICTNESS,0,dx_device,NULL, NULL,&dx_basic_effect,NULL, NULL  ) ) )
		return false;

	effect_technique	= dx_basic_effect->GetTechniqueByName("full");

	//create matrix effect pointers
	effect_mat_view		= dx_basic_effect->GetVariableByName( "View" )->AsMatrix();
	effect_mat_proj		= dx_basic_effect->GetVariableByName( "Projection" )->AsMatrix();
	effect_mat_world	= dx_basic_effect->GetVariableByName( "World" )->AsMatrix();
	effect_texture		= dx_basic_effect->GetVariableByName( "tex2D" )->AsShaderResource(); 

	//tell directx how to handle vertex format we defined in the vertex struct
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	//create input layout
	D3D10_PASS_DESC PassDesc;
	effect_technique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	HRESULT create_effect_layout = dx_device->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature,PassDesc.IAInputSignatureSize, &dx_vertex_layout );
	
	assert( SUCCEEDED( create_effect_layout ) );

	dx_device->IASetInputLayout( dx_vertex_layout );

	//Set up the shader matrix
	effect_mat_view->SetMatrix( dx_view_matrix );
	effect_mat_proj->SetMatrix( dx_projection_matrix );

	return true;
}

//*** Init Vertex Buffer ***

bool Init_Vertex_Buffer()
{
	UINT numVertices = VERTEX_BUFFER_SIZE;

	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( vertex ) * numVertices; //total size of buffer in bytes
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	if ( FAILED( dx_device->CreateBuffer( &bd, NULL, &dx_vertex_buffer ) ) ) return false;

	// Set vertex buffer
	UINT stride = sizeof( vertex );
	UINT offset = 0;
	dx_device->IASetVertexBuffers( 0, 1, &dx_vertex_buffer, &stride, &offset );

	return true;
}

//*** Set Up Rasterization ***

void Setup_Rasterization()
{
	D3D10_RASTERIZER_DESC rasterizerState;
	rasterizerState.CullMode = D3D10_CULL_NONE;
	rasterizerState.FillMode = D3D10_FILL_SOLID;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = false;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = true;
	rasterizerState.AntialiasedLineEnable = true;

	dx_device->CreateRasterizerState( &rasterizerState, &dx_rasterizer_state );
	dx_device->RSSetState( dx_rasterizer_state );
}

//*** Cleanup Device ***

void Cleanup_Device()
{
	if ( dx_render_target ) dx_render_target->Release();
	if ( dx_swap_chain ) dx_swap_chain->Release();
	if ( dx_device ) dx_device->Release();
	if ( dx_vertex_buffer ) dx_vertex_buffer->Release();
	if ( dx_vertex_layout ) dx_vertex_layout->Release();
	if ( dx_rasterizer_state ) dx_rasterizer_state->Release();
	if ( dx_basic_effect ) dx_basic_effect->Release();
}


