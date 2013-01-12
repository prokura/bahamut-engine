#ifndef Core_Config_h
#define Core_Config_h

//ENTITY
const unsigned MAX_ENTITIES = 1024;

//COMPONENTS
const unsigned MAX_COMPONENTS_PER_ENTITY = 255;
const unsigned MAX_SPRITE_COMPONENTS = 1024;

//RENDERER
const int GAME_RESOLUTION_X = 800;
const int GAME_RESOLUTION_Y = 600;

const unsigned REFRESH_RATE = 60;
const bool VSYNC_ENABLED = true;
const bool FULL_SCREEN = false;
const bool MULTI_SAMPLING = true;
const int VERTEX_BUFFER_SIZE = 4096;
const unsigned MAX_TEXTURES = 256;

#endif /* config_h */
