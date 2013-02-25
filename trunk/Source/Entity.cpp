//*** Entity.cpp ***

#include "Entity.h"
#include "config.h"
#include <assert.h>

//*** Globals ***

static unsigned Entity_Count = 0;
static EntityID Entity_Freelist[ MAX_ENTITIES ];
static unsigned Entity_Freelist_Count = 0;
static Entity Entities[ MAX_ENTITIES ];


//*** Create ***

EntityID Entity_Create()
{
	// Create from the free list if we can
	if( Entity_Freelist_Count > 0 )
	{
		EntityID entity = Entity_Freelist[ Entity_Freelist_Count - 1 ];
		Entity_Freelist_Count--;
    	return entity;
	}
	else // And only expand the range if free list is empty
	{
		assert( Entity_Count < MAX_ENTITIES );
		EntityID entity = Entity_Count;
		Entity_Count++;
    	return entity;
	}
}	

//*** Destroy ***

void Entity_Destroy( EntityID entity )
{
	assert( entity < MAX_ENTITIES );

	Entity_Freelist[ Entity_Freelist_Count ] = entity;
	Entity_Freelist_Count++;
	Entity_Count--;
}	

//*** GetEntity ***

Entity* Entity_Get( EntityID ID )
{
	assert( ID < Entity_Count  );

	for( unsigned nEntity = 0 ; nEntity < Entity_Freelist_Count; nEntity++ )
	{
		if( Entity_Freelist[nEntity] == ID )
			return 0;
	}
	
	return &Entities[ID];
}
