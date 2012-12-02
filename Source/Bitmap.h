#ifndef Bitmap_h
#define Bitmap_h
#include "config.h"

//REFERENCE INTERFACE FOR BITMAP

typedef unsigned BitmapId;

struct Bitmap
{
    unsigned width;
    unsigned height;
	int x;
	int y;
	unsigned* data;
};

static unsigned bitmap_next = 0;
static unsigned bitmap_freelist[ MAX_BITMAPS ];
static unsigned bitmap_freelist_count = 0;
static Bitmap bitmaps[ MAX_BITMAPS ];

BitmapId Create_Bitmap( const char* filename );
void Destroy_Bitmap( BitmapId bitmap );
void Clear_All_Bitmap();

#endif /* Bitmap_h */