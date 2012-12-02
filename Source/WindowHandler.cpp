#include "WindowHandler.h"

//*** Windows Api Values ***

HINSTANCE hinstance;
HWND hwnd;
WNDCLASSEX wc;
std::string window_title;

//*** Window Message CallBack ***

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
    HDC hdc;

	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		// Check if the window is being closed.
		case WM_CLOSE:
			PostQuitMessage(0);		
		break;

		case WM_PAINT:
			hdc = BeginPaint( hwnd, &ps );
            EndPaint( hwnd, &ps );
		break;

		default:
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		break;
	}

	return 0;
}

//*** Window Init ***

void Window_Init( int width, int height, const char* title, bool fullscreen )
{
	window_title = std::string( title );

	// Get the instance of this application.
	hinstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = title;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	int posX;
	int posY;

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(fullscreen)
	{
		DEVMODE dmScreenSettings;

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - width)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
		hwnd = CreateWindowEx(WS_EX_APPWINDOW, title, title, 
								WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
								posX, posY, width, height, NULL, NULL, hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow( hwnd, SW_SHOW );
	SetForegroundWindow( hwnd );
	SetFocus( hwnd );

	// Hide the mouse cursor.
	//ShowCursor(false);
}

//*** Window Destroy ***

void Window_Destroy()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Remove the window.
	DestroyWindow(hwnd);
	hwnd = NULL;

	// Remove the application instance.
	UnregisterClass( window_title.c_str(), hinstance);
	hinstance = NULL;

	return;
}

//*** Window GetMessage ***

bool Window_GetMessage()
{
	MSG msg = {0};

	if( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
		return true;
	}

	return false;
}

//*** Window GetHWND ***

HWND Window_GetHWND()
{
	return hwnd;
}