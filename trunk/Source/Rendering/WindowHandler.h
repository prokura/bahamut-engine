#ifndef WindowHandler_h
#define WindowHandler_h
#include <windows.h>
#include <string>

//*** Globals ***

static HINSTANCE hinstance;
static HWND hwnd;
static WNDCLASSEX wc;
static std::string window_title;

//*** Window Init

void Window_Init( int width, int height, const char* title, bool fullscreen );
void Window_Destroy();
bool Window_GetMessage();

//*** Window Message CallBack

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

#endif /* WindowHandler_h */