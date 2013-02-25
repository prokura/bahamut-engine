#ifndef Shader_h
#define Shader_h

//REFERENCE INTERFACE FOR SHADERS

typedef unsigned ShaderID;

TextureID Shader_Create( const char* filename );
void Shader_Destroy( ShaderID Texture );
void Shader_ClearAll();

#endif /* Texture_h */