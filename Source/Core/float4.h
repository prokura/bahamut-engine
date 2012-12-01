#ifndef float4_h
#define float4_h

struct float2;
struct float3;

// float4_t
struct float4_t
    {
    float x;
    float y;
    float z;    
    float w;    
	};
    
// float4
struct float4 : public float4_t
	{
    // Constructors
    float4();
	float4( float4_t v );
    float4( float x, float y, float z, float w );   
    explicit float4( float v );
    explicit float4( float2 v );
    explicit float4( float3 v );
    float4( const float4& v );

    // Operators    
	operator float4_t() const;
	float4 operator=( float4 v );
	float4 operator+=( float4 v );
	float4 operator-=( float4 v );
	float4 operator*=( float4 v );	
	float4 operator/=( float4 v );
	float4 operator+=( float s );	
	float4 operator-=( float s );	
	float4 operator*=( float s );	
	float4 operator/=( float s );	

	static const float4 zero;
	static const float4 one;
	static const float4 right;
	static const float4 left;
	static const float4 up;
	static const float4 down;
	static const float4 forward;
	static const float4 back;
	};

// Operators
float4 operator+( float4 a, float4 b );                         
float4 operator-( float4 a, float4 b );                          
float4 operator*( float4 a, float4 b );
float4 operator/( float4 a, float4 b );
float4 operator+( float s, float4 v );
float4 operator-( float s, float4 v );
float4 operator*( float s, float4 v );
float4 operator/( float s, float4 v );
float4 operator+( float4 v, float s );
float4 operator-( float4 v, float s );
float4 operator*( float4 v, float s );
float4 operator/( float4 v, float s );
float4 operator-( float4 v );    
bool operator==( float4 a, float4 b );                         
bool operator!=( float4 a, float4 b );                         

// Functions
float lengthsq( float4 v );
float length( float4 v );
float4 normalize( float4 v );
float dot( float4 a, float4 b );
float4 cross( float4 a, float4 b );
float4 lerp( float4 a, float4 b, float s );
float distancesq( float4 a, float4 b );
float distance( float4 a, float4 b );
float4 saturate( float4 v );
float4 smoothstep( float4 min, float4 max, float4 v );
float4 smootherstep( float4 min, float4 max, float4 v );

#endif /* float4_h */
