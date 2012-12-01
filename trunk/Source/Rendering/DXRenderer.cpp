#include "Renderer.h"
#include "WindowHandler.h"
#include <assert.h>


///*** Globals ***

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


//*** Initialise ***

void Renderer_Init( unsigned width, unsigned height, const char* title )
{
	Window_Init( width, height, title, false );
}

//*** Terminate ***

void Renderer_Terminate()
{
	Window_Destroy();
}

//*** Draw ***

void Renderer_Draw()
{

}