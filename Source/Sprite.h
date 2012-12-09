#ifndef Sprite_h
#define Sprite_h

//REFERENCE INTERFACE FOR SPRITE

typedef unsigned SpriteId;

SpriteId Component_Create_Sprite( const char* filename );
void Component_Destroy_Sprite( TextureId textureId );

#endif /* Texture_h */