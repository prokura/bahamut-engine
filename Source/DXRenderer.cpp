#include "Renderer.h"
#include "Texture.h"
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

static float						clear_color[4]			={ 0.93f,0.95f,0.98f,1 };

static ID3D10Buffer*				dx_vertex_buffer		= NULL;
static ID3D10InputLayout*			dx_vertex_layout		= NULL;
static int							dx_vertex_count			= 0;

//renderstate
static ID3D10BlendState*			dx_AlphaBlendState		= NULL;

//effects and techniques
static ID3D10Effect*						dx_basic_effect			= NULL;
static ID3D10EffectTechnique*				effect_technique		= NULL;
static ID3D10EffectVariable*				effect_transform		= NULL;
static ID3D10EffectVectorVariable*			effect_resolution		= NULL;
static ID3D10EffectShaderResourceVariable*	effect_texture			= NULL;

struct vertex
{
      D3DXVECTOR2 pos;
      D3DXVECTOR4 color;
	  D3DXVECTOR2 texCoord;

      vertex( D3DXVECTOR2 p, D3DXVECTOR4 c, D3DXVECTOR2 uv ) : pos(p), color(c), texCoord(uv) {}
};

bool Init_Device( int width );
bool Init_Shader();
bool Init_Vertex_Buffer();
void Setup_Rasterization();
void Cleanup_Device();

//*** Texture Interface ***

struct Transform
{
	float position[2];
	float scale[2];
	float rotation;
};

struct Texture
{
	// x,y,scale,rotation
	Transform transform;
	ID3D10ShaderResourceView* texture;
};

static unsigned Texture_next = 0;
static unsigned Texture_freelist[ MAX_TEXTURES ];
static unsigned Texture_freelist_count = 0;
static Texture Textures[ MAX_TEXTURES ];

//*** Create Textures ***

TextureId Create_Texture( const char* filename )
{
	unsigned handle = 0;

	// Create from the free list if we can
	if( Texture_freelist_count > 0 )
	{
		handle = Texture_freelist[ Texture_freelist_count - 1 ];
		Texture_freelist_count--;
	}
	else // And only expand the range if free list is empty
	{
		assert( Texture_next < MAX_TEXTURES );
		handle = Texture_next;
		Texture_next++;
	}

	Texture* Texture = &Textures[ handle ];
	 
	HRESULT load_texture = D3DX10CreateShaderResourceViewFromFile( dx_device, filename, NULL, NULL, &Texture->texture, NULL );
	assert( SUCCEEDED( load_texture ) );

	ID3D10Resource* res;
	Texture->texture->GetResource( &res ); 
	D3D10_TEXTURE2D_DESC desc2D;
    ((ID3D10Texture2D*)res)->GetDesc(&desc2D);

	Texture->transform.position[0] = 0;
	Texture->transform.position[1] = 0;
	Texture->transform.scale[0] = (float)(desc2D.Width/(float)GAME_RESOLUTION_X);
	Texture->transform.scale[1] = (float)(desc2D.Height/(float)GAME_RESOLUTION_Y);
	Texture->transform.rotation = 180;

	res->Release();

	return handle;
}

//*** Destroy Textures ***

void Destroy_Texture( TextureId Texture )
{
	assert( Texture < MAX_TEXTURES );

	Texture_freelist[ Texture_freelist_count ] = Texture;
	Texture_freelist_count++;
	Textures[ Texture ].texture->Release();
}

//*** Clear All Textures ***

void Clear_All_Texture()
{
	for( unsigned nTexture = 0 ; nTexture < Texture_next; nTexture++ )
	{
		Destroy_Texture( nTexture );
	}
}

//*** Initialise ***

bool Renderer_Init( unsigned width, unsigned height, const char* title )
{
	Window_Init( width, height, title, FULL_SCREEN );

	bool Initialisation = false;

	Initialisation = Init_Device( width );
        
    assert( Initialisation );

	Initialisation = Init_Shader();
	
	assert( Initialisation );

	Initialisation = Init_Vertex_Buffer();

	assert( Initialisation );

	Setup_Rasterization();

	return Initialisation;
}

//*** Terminate ***

void Renderer_Terminate()
{
	Clear_All_Texture();
	Cleanup_Device();
	Window_Destroy();
}

//*** Draw ***

void Renderer_Draw()
{
	//clear scene
	dx_device->ClearRenderTargetView( dx_render_target, clear_color );

	effect_texture->SetResource( Textures[0].texture );
	effect_transform->SetRawValue( &Textures[0].transform,0,36 );

	// Set primitive topology 
	dx_device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//get technique desc
	D3D10_TECHNIQUE_DESC techDesc;
	effect_technique->GetDesc( &techDesc );
	
	//Apply technique
	effect_technique->GetPassByIndex( 0 )->Apply( 0 );

	//Draw
	dx_device->Draw( dx_vertex_count, 0 );

	//flip buffers
	dx_swap_chain->Present(0,0);
}


//*** Init Directx Device ***

bool Init_Device( int width )
{
	//Set up DX swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );

	//set buffer dimensions and format
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = GAME_RESOLUTION_X;
	swapChainDesc.BufferDesc.Height = GAME_RESOLUTION_Y;
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
	swapChainDesc.Windowed = !FULL_SCREEN;

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
	dx_viewport.Width = GAME_RESOLUTION_X;
	dx_viewport.Height = GAME_RESOLUTION_Y;
	dx_viewport.MinDepth = 0.0f;
	dx_viewport.MaxDepth = 1.0f;
	dx_viewport.TopLeftX = 0;
	dx_viewport.TopLeftY = 0;

	//set the viewport
	dx_device->RSSetViewports(1, &dx_viewport);

	//setup blend state
	D3D10_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D10_BLEND_DESC));

	BlendState.BlendEnable[0] = TRUE;

	BlendState.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	BlendState.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	BlendState.BlendOp = D3D10_BLEND_OP_ADD;

	BlendState.SrcBlendAlpha = D3D10_BLEND_ONE;
	BlendState.DestBlendAlpha = D3D10_BLEND_ONE;
	BlendState.BlendOpAlpha = D3D10_BLEND_OP_ADD;

	BlendState.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	dx_device->CreateBlendState(&BlendState, &dx_AlphaBlendState);

	//set blend state
	dx_device->OMSetBlendState(dx_AlphaBlendState, 0, 0xfffffff);

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
	effect_resolution	= dx_basic_effect->GetVariableByName( "resolution" )->AsVector();
	effect_transform	= dx_basic_effect->GetVariableByName( "t" );
	effect_texture		= dx_basic_effect->GetVariableByName( "tex2D" )->AsShaderResource(); 

	//tell directx how to handle vertex format we defined in the vertex struct
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	//create input layout
	D3D10_PASS_DESC PassDesc;
	effect_technique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	HRESULT create_effect_layout = dx_device->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature,PassDesc.IAInputSignatureSize, &dx_vertex_layout );
	
	float resolution[2];
	resolution[0] = GAME_RESOLUTION_X;
	resolution[1] = GAME_RESOLUTION_Y;
	effect_resolution->SetFloatVector( resolution );

	assert( SUCCEEDED( create_effect_layout ) );

	dx_device->IASetInputLayout( dx_vertex_layout );

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

	//fill vertex buffer with vertices

	vertex* v = NULL;
	dx_vertex_count = 4;

	//lock vertex buffer for CPU use
	dx_vertex_buffer->Map( D3D10_MAP_WRITE_DISCARD, 0, (void**) &v );

	v[0] = vertex( D3DXVECTOR2(-1,-1),D3DXVECTOR4(1,0,0,1),D3DXVECTOR2(0.0f, 1.0f) );
	v[1] = vertex( D3DXVECTOR2(-1,1),D3DXVECTOR4(0,1,0,1),D3DXVECTOR2(0.0f, 0.0f) );
	v[2] = vertex( D3DXVECTOR2(1,-1),D3DXVECTOR4(0,0,1,1),D3DXVECTOR2(1.0f, 1.0f) );
	v[3] = vertex( D3DXVECTOR2(1,1),D3DXVECTOR4(1,1,0,1),D3DXVECTOR2(1.0f, 0.0f) );

	dx_vertex_buffer->Unmap();

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
	if( dx_AlphaBlendState ) dx_AlphaBlendState->Release();
}


