#ifndef WindowHandler_h
#define WindowHandler_h
#include <windows.h>
#include <string>

//*** Window Init

void Window_Init( int width, int height, const char* title, bool fullscreen );
void Window_Destroy();
bool Window_GetMessage();
HWND Window_GetHWND();


#endif /* WindowHandler_h */