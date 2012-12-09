//*** component_sprite.cpp ***
#include "config.h"
#include <assert.h>
#include "Texture.h"
#include "Entity.h"
#include "Sprite.h"
#include "float2.h"

struct Sprite
{
	Entity entity;
	float2 position;
	float2 size;
	TextureId atlasId;
};
	
//*** Globals ***

static Sprite Sprites[ MAX_SPRITE_COMPONENTS ];
static unsigned Sprite_Count = 0;
static unsigned Entity_To_Sprite_Index[ MAX_ENTITIES ];

//*** Create ***

void Component_Create_Sprite( Entity entity, TextureId textureId, float x, float y )
{
    assert( entity < MAX_ENTITIES );
	Entity_To_Sprite_Index[ entity ] = Sprite_Count;
	Sprites[ Sprite_Count ].entity = entity;
	Sprites[ Sprite_Count ].position = float2( x, y );
	Sprites[ Sprite_Count ].atlasId = textureId;
	Sprite_Count++;
}

//*** Destroy ***

void Component_Sprite_Destroy( Entity entity )
{
    if( entity < MAX_ENTITIES )
	{
		unsigned index = Entity_To_Sprite_Index[ entity ];
		
		if( index < MAX_SPRITE_COMPONENTS  && Sprites[ index ].entity == entity  )
		{
			Sprites[ index ] = Sprites[ Sprite_Count - 1 ];
			Sprite_Count--;

			Entity_To_Sprite_Index[ entity ] = MAX_SPRITE_COMPONENTS;
		}
	}
}


//*** update ***
void Component_Sprite_Update( float delta_time )
{
}

