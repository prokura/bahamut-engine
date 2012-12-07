/*** main.cpp ***/

#include "renderer.h"
#include "Bitmap.h"
#include "WindowHandler.h"
#include "config.h"

//*** Game ***

void Game()
{
    bool running = Renderer_Init( GAME_RESOLUTION_X, GAME_RESOLUTION_Y, "Bahamuto" );

	Create_Bitmap( "Data\\Image\\baha.png" );
	
    /*bitmap_handle_t bitmap = renderer_create_bitmap( "white.png" );
	entity_t player = entity_create();
	component_player_create( player, 0 );
	component_locomotion_create( player, float3( 320, 0, 240 ) );
	component_sprite_create( player, bitmap );
	*/


	while( running  && !Window_GetMessage() )
	{
		float delta_time = 1.0f/60.0f;
		// Run systems
		Renderer_Draw();		
	}
		
	Renderer_Terminate();
}



/* include so we can use leak detection */
#include <crtdbg.h>

/*** WinMain ***/

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	#ifndef NDEBUG
		int flag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); /* Get current flag */
		flag ^= _CRTDBG_LEAK_CHECK_DF; /* Turn on leak-checking bit */
		_CrtSetDbgFlag( flag ); /* Set flag to the new value */
	#endif

    Game();	

	return 0;
}