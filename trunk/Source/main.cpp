/*** main.cpp ***/

#include "renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "WindowHandler.h"
#include "config.h"
#include "Entity.h"

//*** Game ***

void Game()
{
    bool running = Renderer_Init( 800, 600, "Bahamuto" );

	Texture_Create( "Data\\Image\\bahaAlpha.png" );
	Shader_Create("Data\\Shader\\texture.fx");

	while( running  && !Window_GetMessage() )
	{
		/*float delta_time = 1.0f/60.0f;*/
		// Run systems
		Renderer_Draw();		
	}
		
	Renderer_Terminate();

	float* a = new float;

	*a = 5.0f;
}



/* include so we can use leak detection */
#include <crtdbg.h>

/*** WinMain ***/

int CALLBACK WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ )
{
	#ifndef NDEBUG
		int flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); /* Get current flag */
		flag ^= _CRTDBG_LEAK_CHECK_DF; /* Turn on leak-checking bit */
		_CrtSetDbgFlag( flag ); /* Set flag to the new value */
	#endif

    Game();

	return 0;
}