#include "Renderer.h"
#include "DXRenderer.h"
#include "Bitmap.h"
#include "WindowHandler.h"
#include <assert.h>
#include "config.h"
#include <d3dx10.h>

///*** DirectXValue ***

D3D10_DRIVER_TYPE			dx_driver				= D3D10_DRIVER_TYPE_NULL;
ID3D10Device*				dx_device				= NULL;
IDXGISwapChain*				dx_swap_chain			= NULL;
ID3D10RenderTargetView*		dx_render_target		= NULL;
ID3D10Texture2D*			dx_depthStencil_buffer	= NULL;
ID3D10DepthStencilState*	dx_depthStencil_on		= NULL;
ID3D10DepthStencilState*	dx_depthStencil_off		= NULL;
ID3D10DepthStencilView*		dx_depthStencil_view	= NULL;
ID3D10RasterizerState*		dx_raster_state			= NULL;

D3DXMATRIX					dx_mat_projection;
D3DXMATRIX					dx_mat_world;
D3DXMATRIX					dx_mat_ortho;
D3DXMATRIX					dx_mat_view;

float						clear_color[4]			={ 0,0,0,1 };
float						screen_left				= 0;
float						screen_right			= 0;
float						screen_top				= 0;
float						screen_bottom			= 0;

HRESULT Init_Device( int width, int height );
void Init_View_Matrix();
void Cleanup_Device();
void Turn_ZBuffer_On();
void Turn_ZBuffer_Off();

//*** Initialise ***

bool Renderer_Init( unsigned width, unsigned height, const char* title )
{
	Window_Init( width, height, title, FULL_SCREEN );

	if( FAILED( Init_Device(width,height) ) )
    {
        Cleanup_Device();
        return false;
    }

	Init_View_Matrix();

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
	// Clear the back and depth buffer.
	dx_device->ClearRenderTargetView( dx_render_target, clear_color );
	dx_device->ClearDepthStencilView( dx_depthStencil_view, D3D10_CLEAR_DEPTH, 1.0f, 0);

	// Turn off the Z buffer to begin all 2D rendering.
	Turn_ZBuffer_Off();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	/*result = m_Bitmap->Render(m_D3D->GetDevice(), 100, 100);
	if(!result)
	{
		return false;
	}*/

	// Render the bitmap using the texture shader.
	//m_TextureShader->Render(m_D3D->GetDevice(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	Turn_ZBuffer_On();

	if( VSYNC_ENABLED )
	{
		// Lock to screen refresh rate.
		dx_swap_chain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		dx_swap_chain->Present(0, 0);
	}

}


//*** Get Device() ***

ID3D10Device* Get_Device()
{
	return dx_device;
}

void Update_Bitmap_Geometry( unsigned bitmapId )
{
}

//*** Init View Matrix ***
void Init_View_Matrix()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = 0;
	position.y = 0;
	position.z = 0;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = 0;
	yaw   = 0;
	roll  = 0;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&dx_mat_view, &position, &lookAt, &up);
}

//*** Init Directx Device ***

HRESULT Init_Device( int width, int height )
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned numModes = 0;
	unsigned numerator = 0;
	unsigned denominator = 0;
	unsigned stringLength = 0;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error = 0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D10Texture2D* backBufferPtr;
	D3D10_TEXTURE2D_DESC depthBufferDesc;
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D10_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D10_RASTERIZER_DESC rasterDesc;
	D3D10_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

	//set screen limit
	screen_left = width/2*-1;
	screen_right = width/2;
	screen_top = height/2;
	screen_bottom = height/2*-1;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(unsigned i=0; i<numModes; i++)
	{
		if(displayModeList[i].Width == (unsigned int)width)
		{
			if(displayModeList[i].Height == (unsigned int)height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	//video_Card_Memory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	/*char videoCardDescription[128];
	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		return false;
	}*/

	// Release the display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
    swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;

	// Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if(VSYNC_ENABLED)
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = Window_GetHWND();

	// Turn multisampling off.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(FULL_SCREEN)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Create the swap chain and the Direct3D device.
	result = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &swapChainDesc, &dx_swap_chain, &dx_device);
	if(FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = dx_swap_chain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = dx_device->CreateRenderTargetView(backBufferPtr, NULL, &dx_render_target);
	if(FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = dx_device->CreateTexture2D(&depthBufferDesc, NULL, &dx_depthStencil_buffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = dx_device->CreateDepthStencilState(&depthStencilDesc, &dx_depthStencil_on);
	if(FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state on the D3D device.
	dx_device->OMSetDepthStencilState(dx_depthStencil_on, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = dx_device->CreateDepthStencilView(dx_depthStencil_buffer, &depthStencilViewDesc, &dx_depthStencil_view);
	if(FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	dx_device->OMSetRenderTargets(1, &dx_render_target, dx_depthStencil_view);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D10_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D10_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = MULTI_SAMPLING;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = dx_device->CreateRasterizerState(&rasterDesc, &dx_raster_state);
	if(FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	dx_device->RSSetState(dx_raster_state);

	// Setup the viewport for rendering.
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

	// Create the viewport.
	dx_device->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)width / (float)height;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&dx_mat_projection, fieldOfView, screenAspect, 0, 1);

    // Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&dx_mat_world);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&dx_mat_ortho, (float)width, (float)height, 0, 1);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = dx_device->CreateDepthStencilState(&depthDisabledStencilDesc, &dx_depthStencil_off);
	if(FAILED(result))
	{
		return false;
	}

    return true;
}

//*** Cleanup Device ***

void Cleanup_Device()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(dx_swap_chain)
	{
		dx_swap_chain->SetFullscreenState(false, NULL);
	}

	if(dx_depthStencil_off)
	{
		dx_depthStencil_off->Release();
		dx_depthStencil_off = 0;
	}

	if(dx_raster_state)
	{
		dx_raster_state->Release();
		dx_raster_state = NULL;
	}

	if(dx_depthStencil_view)
	{
		dx_depthStencil_view->Release();
		dx_depthStencil_view = NULL;
	}

	if(dx_depthStencil_on)
	{
		dx_depthStencil_on->Release();
		dx_depthStencil_on = NULL;
	}

	if(dx_depthStencil_buffer)
	{
		dx_depthStencil_buffer->Release();
		dx_depthStencil_buffer = NULL;
	}

	if(dx_render_target)
	{
		dx_render_target->Release();
		dx_render_target = NULL;
	}

	if(dx_swap_chain)
	{
		dx_swap_chain->Release();
		dx_swap_chain = NULL;
	}

	if(dx_device)
	{
		dx_device->Release();
		dx_device = NULL;
	}
}

void Turn_ZBuffer_On()
{
	dx_device->OMSetDepthStencilState(dx_depthStencil_on, 1);
}


void Turn_ZBuffer_Off()
{
	dx_device->OMSetDepthStencilState(dx_depthStencil_off, 1);
}
