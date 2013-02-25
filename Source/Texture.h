#ifndef Texture_h
#define Texture_h

//REFERENCE INTERFACE FOR TEXTURE

typedef unsigned TextureID;

TextureID Texture_Create( const char* filename );
void Texture_Destroy( TextureID Texture );
void Texture_ClearAll();

#endif /* Texture_h */