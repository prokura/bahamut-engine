#include "Bitmap.h"
#include "DXRenderer.h"
#include <d3dx10.h>
#include <assert.h>

BitmapId Create_Bitmap( const char* filename )
{
	unsigned handle = 0;
    
	// Create from the free list if we can
	if( bitmap_freelist_count > 0 )
	{
		handle = bitmap_freelist[ bitmap_freelist_count - 1 ];
		bitmap_freelist_count--;
	}
	else // And only expand the range if free list is empty
	{
		assert( bitmap_next < MAX_BITMAPS);
		handle = bitmap_next;
		bitmap_next++;
	}

    Bitmap* bitmap = &bitmaps[ handle ];

	int w = 0;
	int h = 0;
	
	//unsigned int* image = (unsigned int*) stbi_load( filename, &w, &h, 0, 4 );
	
	ID3D10Texture2D* image = NULL;
    ID3D10Resource* ressource = NULL;
    
    // Loads the texture into a temporary ID3D10Resource object
	bool texture_load_fail = SUCCEEDED( D3DX10CreateTextureFromFile(Get_Device(),filename,NULL,NULL,&ressource,NULL) );

    // Make sure the texture was loaded in successfully
    assert( texture_load_fail );

    // Translates the ID3D10Resource object into a ID3D10Texture2D object
    ressource ->QueryInterface(__uuidof( ID3D10Texture2D), (LPVOID*)&image);
    ressource ->Release();

    bitmap->width = w;
    bitmap->height = h;
	bitmap->dirty = false;
    bitmap->data = (unsigned*) malloc( sizeof( unsigned ) * w * h );
    memcpy( bitmap->data, image, sizeof( unsigned ) * w * h );

	image->Release();

    return handle;
}

void Destroy_Bitmap( BitmapId bitmap )
{
	assert( bitmap < MAX_BITMAPS );

	bitmap_freelist[ bitmap_freelist_count ] = bitmap;
	bitmap_freelist_count++;
	free( bitmaps[ bitmap ].data );
}

void Clear_All_Bitmap()
{
	for( unsigned nBitmap = 0 ; nBitmap < bitmap_next; nBitmap++ )
	{
		Destroy_Bitmap( nBitmap );
	}
}