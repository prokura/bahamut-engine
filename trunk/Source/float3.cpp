//*** float3.cpp ***

#include "float3.h"
#include "float2.h"
#include "float4.h"

#include <math.h>

float3 float3::zero() { return float3( 0, 0, 0 ); }
float3 float3::one() { return float3( 1, 1, 1 ); }
float3 float3::right() { return float3( 1, 0, 0 ); }
float3 float3::left() { return float3( -1, 0, 0 ); }
float3 float3::up() { return float3( 0, 1, 0 ); }
float3 float3::down() { return float3( 0, -1, 0 ); }
float3 float3::forward() { return float3( 0, 0, 1 ); }
float3 float3::back() { return float3( 0, 0, -1 ); }


//*** Constructors ***

float3::float3()
    {
	x = 0;
	y = 0;
	z = 0;
    }

float3::float3( float3_t v )
	{
	x = v.x;
	y = v.y;
	z = v.z;
	}

float3::float3( float x, float y, float z )
    {
    this->x = x;
    this->y = y;
    this->z = z;
	}

float3::float3( float v )
    {
	x = v;
	y = v;
	z = v;
    }

float3::float3( float2 v )
    {
	x = v.x;
	y = v.y;
	z = 0;
    }

float3::float3( const float3& v )
    {
	x = v.x;
	y = v.y;
	z = v.z;
    }

float3::float3( float4 v )
    {
	x = v.x;
	y = v.y;
	z = v.z;
    }


//*** Operators ***

float3::operator float3_t() const
	{
	return * ( (float3_t*) this );
	}

float3 float3::operator=( float3 v )
    {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
    }

float3 float3::operator+=( float3 v )
    {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
    }

float3 float3::operator-=( float3 v )
    {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
    }

float3 float3::operator*=( float3 v )
    {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
    }

float3 float3::operator/=( float3 v )
    {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
    }

float3 float3::operator+=( float s )
    {
    x += s;
    y += s;
    z += s;
    return *this;
    }

float3 float3::operator-=( float s )
    {
    x -= s;
    y -= s;
    z -= s;
    return *this;
    }

float3 float3::operator*=( float s )
    {
    x *= s;
    y *= s;
    z *= s;
    return *this;
    }

float3 float3::operator/=( float s )
    {
    x /= s;
    y /= s;
    z /= s;
    return *this;
    }

//*** Operators ***

float3 operator+( float3 a, float3 b )
    {
    return float3( a.x + b.x, a.y + b.y, a.z + b.z );
    }
                          
float3 operator-( float3 a, float3 b )
    {
    return float3( a.x - b.x, a.y - b.y, a.z - b.z );
    }
                          
float3 operator*( float3 a, float3 b )
    {
    return float3( a.x * b.x, a.y * b.y, a.z * b.z );
    }
                          
float3 operator/( float3 a, float3 b )
    {
    return float3( a.x / b.x, a.y / b.y, a.z / b.z );
    }

float3 operator+( float s, float3 v )
	{
	return float3( s + v.x, s + v.y, s + v.z );
	}
float3 operator-( float s, float3 v )
	{
	return float3( s - v.x, s - v.y, s - v.z );
	}
float3 operator*( float s, float3 v )
	{
	return float3( s * v.x, s * v.y, s * v. z );
	}
float3 operator/( float s, float3 v )
	{
	return float3( s / v.x, s / v.y, s / v.z );
	}
float3 operator+( float3 v, float s )
	{
	return float3( v.x + s, v.y + s, v.z + s );
	}
float3 operator-( float3 v, float s )
	{
	return float3( v.x - s, v.y - s, v.z - s );
	}
float3 operator*( float3 v, float s )
	{
	return float3( v.x * s, v.y * s, v.z * s );
	}
float3 operator/( float3 v, float s )
	{
	return float3( v.x / s, v.y / s, v.z / s );
	}

float3 operator-( float3 v )
	{
	return float3( -v.x, -v.y, -v.z );
	}

bool operator==( float3 a, float3 b )
	{
	return a.x == b.x && a.y == b.y && a.z == b.z;
	}

bool operator!=( float3 a, float3 b )                         
	{
	return !( a == b);
	}


//*** Functions ***

float lengthsq( float3 v )
    {
    return v.x * v.x + v.y * v.y + v.z * v.z ;
    }
    
float length( float3 v )
    {
    return sqrtf( lengthsq( v ) );
    }

float3 normalize( float3 v )
    {
    // skip divide if length is zero
    float const len = length( v );
    return ( len > 0 ) ? v / len : v;
    }

float dot( float3 a, float3 b )
    {
    return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
float3 cross( float3 a, float3 b )
    {
    return float3( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
    }

float3 lerp( float3 a, float3 b, float s )
    {
	return float3( a.x + ( b.x - a.x ) * s, a.y + ( b.y - a.y ) * s, a.z + ( b.z - a.z ) * s );
	}

float distancesq( float3 a, float3 b )
	{
	return lengthsq( a - b );
	}

float distance( float3 a, float3 b )
	{
	return length( a - b );
	}

float3 saturate( float3 v )
	{
	float3 t( 
		v.x < 0.0f ? 0.0f : v.x,
		v.y < 0.0f ? 0.0f : v.y,
		v.z < 0.0f ? 0.0f : v.z
		);
	
	return float3(
		t.x > 1.0f ? 1.0f : t.x,
		t.y > 1.0f ? 1.0f : t.y,
		t.z > 1.0f ? 1.0f : t.z
		);
	}

float3 smoothstep( float3 min, float3 max, float3 v )
	{
	float3 result;
    
	// Scale, bias and saturate x to 0..1 range
    v.x = ( v.x - min.x ) / ( max.x - min.x ); 
    v.y = ( v.y - min.y ) / ( max.y - min.y ); 
    v.z = ( v.z - min.z ) / ( max.z - min.z ); 
	v = saturate( v );

    // Evaluate polynomial
    result.x = v.x * v.x * ( 3 - 2 * v.x );
	result.y = v.y * v.y * ( 3 - 2 * v.y );
	result.z = v.z * v.z * ( 3 - 2 * v.z );
	
	return result;
	}

float3 smootherstep( float3 min, float3 max, float3 v )
	{
	float3 result;
    
	// Scale, bias and saturate x to 0..1 range
    v.x = ( v.x - min.x ) / ( max.x - min.x ); 
    v.y = ( v.y - min.y ) / ( max.y - min.y ); 
    v.z = ( v.z - min.z ) / ( max.z - min.z ); 
	v = saturate( v );

    // Evaluate polynomial
    result.x = v.x * v.x * v.x * ( v.x * ( v.x * 6 - 15 ) + 10 );
	result.y = v.y * v.y * v.y * ( v.y * ( v.y * 6 - 15 ) + 10 );
	result.z = v.z * v.z * v.z * ( v.z * ( v.z * 6 - 15 ) + 10 );
	
	return result;
	}
