#ifndef Texture_h
#define Texture_h

//REFERENCE INTERFACE FOR TEXTURE

typedef unsigned TextureId;

TextureId Create_Texture( const char* filename );
void Destroy_Texture( TextureId Texture );
void Clear_All_Texture();

#endif /* Texture_h */