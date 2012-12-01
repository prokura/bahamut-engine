#ifndef float2_h
#define float2_h

struct float3;
struct float4;

// float2_t
struct float2_t
    {
    float x;
    float y;
	};
    
// float2
struct float2 : public float2_t
    {    
    // Constructors
    float2();
    float2( float2_t v );
    float2( float x, float y );
    explicit float2( float v );
    float2( const float2& v );
    explicit float2( float3 v );
    explicit float2( float4 v );

    // Operators    
	operator float2_t() const;
	float2 operator=( float2 v );
	float2 operator+=( float2 v );
	float2 operator-=( float2 v );
	float2 operator*=( float2 v );	
	float2 operator/=( float2 v );
	float2 operator+=( float s );	
	float2 operator-=( float s );	
	float2 operator*=( float s );	
	float2 operator/=( float s );	

	static const float2 zero;
	static const float2 one;
	static const float2 right;
	static const float2 left;
	static const float2 up;
	static const float2 down;
	};

// Operators
float2 operator+( float2 a, float2 b );                         
float2 operator-( float2 a, float2 b );                          
float2 operator*( float2 a, float2 b );
float2 operator/( float2 a, float2 b );
float2 operator+( float s, float2 v );
float2 operator-( float s, float2 v );
float2 operator*( float s, float2 v );
float2 operator/( float s, float2 v );
float2 operator+( float2 v, float s );
float2 operator-( float2 v, float s );
float2 operator*( float2 v, float s );
float2 operator/( float2 v, float s );
float2 operator-( float2 v );    
bool operator==( float2 a, float2 b );                         
bool operator!=( float2 a, float2 b );                         

// Functions
float lengthsq( float2 v );
float length( float2 v );
float2 normalize( float2 v );
float dot( float2 a, float2 b );
float2 cross( float2 a, float2 b );
float2 lerp( float2 a, float2 b, float s );
float distancesq( float2 a, float2 b );
float distance( float2 a, float2 b );
float2 saturate( float2 v );
float2 smoothstep( float2 min, float2 max, float2 v );
float2 smootherstep( float2 min, float2 max, float2 v );

#endif /* float2_h */
