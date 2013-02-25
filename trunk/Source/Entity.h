#ifndef Entity_h
#define Entity_h

#include "float2.h"

typedef unsigned EntityID;

struct Entity
{
	float2 Position;
};

EntityID Entity_Create();
void Entity_Destroy( EntityID entity );
Entity* Entity_Get( EntityID ID );

#endif /* Entity_h */
