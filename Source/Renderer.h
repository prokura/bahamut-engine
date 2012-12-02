#ifndef Renderer_h
#define Renderer_h

//EVERY RENDERER CLASS SHOULD HAVE THE SAME INTERFACE

bool Renderer_Init( unsigned width, unsigned height, const char* title );
void Renderer_Terminate();
void Renderer_Draw();

#endif /* Renderer_h */
