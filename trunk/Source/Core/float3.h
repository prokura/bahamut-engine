#ifndef float3_h
#define float3_h

struct float2;
struct float4;

// float3_t
struct float3_t
    {
    float x;
    float y;
    float z;   
	};
    
// float3
struct float3 : public float3_t
    {
    // Constructors
    float3();
    float3( float3_t v );
    float3( float x, float y, float z );
    explicit float3( float v );
    explicit float3( float2 v );
    float3( const float3& v );
    explicit float3( float4 v );

    // Operators    
	operator float3_t() const;
	float3 operator=( float3 v );
	float3 operator+=( float3 v );
	float3 operator-=( float3 v );
	float3 operator*=( float3 v );	
	float3 operator/=( float3 v );
	float3 operator+=( float s );
	float3 operator-=( float s );	
	float3 operator*=( float s );	
	float3 operator/=( float s );	

	static float3 zero();
	static float3 one();
	static float3 right();
	static float3 left();
	static float3 up();
	static float3 down();
	static float3 forward();
	static float3 back();
	};

// Operators
float3 operator+( float3 a, float3 b );                         
float3 operator-( float3 a, float3 b );                          
float3 operator*( float3 a, float3 b );
float3 operator/( float3 a, float3 b );
float3 operator+( float s, float3 v );
float3 operator-( float s, float3 v );
float3 operator*( float s, float3 v );
float3 operator/( float s, float3 v );
float3 operator+( float3 v, float s );
float3 operator-( float3 v, float s );
float3 operator*( float3 v, float s );
float3 operator/( float3 v, float s );
float3 operator-( float3 v );    
bool operator==( float3 a, float3 b );                         
bool operator!=( float3 a, float3 b );                         

// Functions
float lengthsq( float3 v );
float length( float3 v );
float3 normalize( float3 v );
float dot( float3 a, float3 b );
float3 cross( float3 a, float3 b );
float3 lerp( float3 a, float3 b, float s );
float distancesq( float3 a, float3 b );
float distance( float3 a, float3 b );
float3 saturate( float3 v );
float3 smoothstep( float3 min, float3 max, float3 v );
float3 smootherstep( float3 min, float3 max, float3 v );


#endif /* float3_h */
