//*** Entity.cpp ***

#include "config.h"
#include <assert.h>

typedef unsigned Entity;

typedef void (*Component_Destroy_Callback)( Entity entity );

//*** Globals ***

static unsigned entity_next = 0;

static Entity entity_freelist[ MAX_ENTITIES ];
static unsigned entity_freelist_count = 0;

//USE 2bits of the u32 to implement a ref counter

//*** Create ***

Entity entity_create()
{
	// Create from the free list if we can
	if( entity_freelist_count > 0 )
	{
		Entity entity = entity_freelist[ entity_freelist_count - 1 ];
		entity_freelist_count--;
    	return entity;
	}
	else // And only expand the range if free list is empty
	{
		assert( entity_next < MAX_ENTITIES );
		Entity entity = entity_next;
		entity_next++;
    	return entity;
	}
}
	

//*** Destroy ***

void entity_destroy( Entity entity )
{
	assert( entity < MAX_ENTITIES );

	entity_freelist[ entity_freelist_count ] = entity;
	entity_freelist_count++;

	/*for( unsigned i = 0; i < destroy_function_count; i++ )
	{
		destroy_functions[ i ][ entity ];
	}*/
}	
