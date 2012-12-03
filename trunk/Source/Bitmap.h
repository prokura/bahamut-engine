#ifndef Bitmap_h
#define Bitmap_h

//REFERENCE INTERFACE FOR BITMAP

typedef unsigned BitmapId;

BitmapId Create_Bitmap( const char* filename );
void Destroy_Bitmap( BitmapId bitmap );
void Clear_All_Bitmap();

#endif /* Bitmap_h */