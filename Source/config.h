#ifndef Core_Config_h
#define Core_Config_h

//ENTITY
const unsigned MAX_ENTITIES = 1024;

//COMPONENTS
const unsigned MAX_MESSAGES = 16;
const unsigned MAX_COMPONENTS_PER_ENTITY = 255;
const unsigned MAX_SPRITE_COMPONENTS = 1024;
const unsigned MAX_LOCOMOTION_COMPONENTS = 1024;
const unsigned MAX_PLAYER_COMPONENTS = 4;

//RENDERER
const unsigned MAX_RENDERER_SUBMITS = 4096;
const unsigned MAX_BITMAPS = 256;
const unsigned REFRESH_RATE = 60;
const bool VSYNC_ENABLED = true;
const bool FULL_SCREEN = false;
const bool MULTI_SAMPLING = true;

#endif /* config_h */
