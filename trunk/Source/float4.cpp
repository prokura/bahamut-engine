//*** float4.cpp ***

#include "float4.h"
#include "float2.h"
#include "float3.h"

#include <math.h>

const float4 float4::zero( 0, 0, 0, 0 );
const float4 float4::one( 1, 1, 1, 1 );
const float4 float4::right( 1, 0, 0, 0 );
const float4 float4::left( -1, 0, 0, 0 );
const float4 float4::up( 0, 1, 0, 0 );
const float4 float4::down( 0, -1, 0, 0 );
const float4 float4::forward( 0, 0, 1, 0 );
const float4 float4::back( 0, 0, -1, 0 );


//*** Constructors ***

float4::float4()
    {
	x = 0;
	y = 0;
	z = 0;
	w = 0;
	}

float4::float4( float4_t v )
	{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	}

float4::float4( float x, float y, float z, float w )
    {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
    }

float4::float4( float v )
    {
	x = v;
	y = v;
	z = v;
	w = v;
    }

float4::float4( float2 v )
    {
	x = v.x;
	y = v.y;
	z = 0;
	w = 0;
    }

float4::float4( float3 v )
    {
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0;
    }

float4::float4( const float4& v )
    {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
    }


//*** Operators ***

float4::operator float4_t() const
	{
	return * ( (float4_t*) this );
	}

float4 float4::operator=( float4 v )
    {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
    }

float4 float4::operator+=( float4 v )
    {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
    }

float4 float4::operator*=( float4 v )
    {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
    }

float4 float4::operator/=( float4 v )
    {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
    }

float4 float4::operator+=( float s )
    {
    x += s;
    y += s;
    z += s;
    w += s;
    return *this;
    }

float4 float4::operator-=( float s )
    {
    x -= s;
    y -= s;
    z -= s;
    w -= s;
    return *this;
    }

float4 float4::operator*=( float s )
    {
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
    }

float4 float4::operator/=( float s )
    {
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
    }



//*** Operators ***

float4 operator+( float4 a, float4 b )
    {
    return float4( a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w );
    }
                          
float4 operator-( float4 a, float4 b )
    {
    return float4( a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w );
    }
                          
float4 operator*( float4 a, float4 b )
    {
    return float4( a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w );
    }
                          
float4 operator/( float4 a, float4 b )
    {
    return float4( a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w );
    }


float4 operator+( float s, float4 v )
	{
	return float4( s + v.x, s + v.y, s + v.z, s + v.w );
	}
float4 operator-( float s, float4 v )
	{
	return float4( s - v.x, s - v.y, s - v.z, s - v.w );
	}
float4 operator*( float s, float4 v )
	{
	return float4( s * v.x, s * v.y, s * v. z, s * v.w );
	}
float4 operator/( float s, float4 v )
	{
	return float4( s / v.x, s / v.y, s / v.z, s / v.w );
	}
float4 operator+( float4 v, float s )
	{
	return float4( v.x + s, v.y + s, v.z + s, v.w + s );
	}
float4 operator-( float4 v, float s )
	{
	return float4( v.x - s, v.y - s, v.z - s, v.w - s );
	}
float4 operator*( float4 v, float s )
	{
	return float4( v.x * s, v.y * s, v.z * s, v.w * s );
	}
float4 operator/( float4 v, float s )
	{
	return float4( v.x / s, v.y / s, v.z / s, v.w / s );
	}

float4 operator-( float4 v )
	{
	return float4( -v.x, -v.y, -v.z, -v.w );
	}

bool operator==( float4 a, float4 b )
	{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

bool operator!=( float4 a, float4 b )                         
	{
	return !( a == b);
	}


//*** Functions ***

float lengthsq( float4 v )
    {
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
    }
    
float length( float4 v )
    {
    return sqrtf( lengthsq( v ) );
    }

float4 normalize( float4 v )
    {
    // skip divide if length is zero
    float const len = length( v );
    return ( len > 0 ) ? v / len : v;
	}

float dot( float4 a, float4 b )
    {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }
    
float4 cross( float4 a, float4 b )
    {
    return float4( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 0 );
    }

float4 lerp( float4 a, float4 b, float s )
    {
	return float4( a.x + ( b.x - a.x ) * s, a.y + ( b.y - a.y ) * s, a.z + ( b.z - a.z ) * s, a.z + ( b.w - a.w ) * s );
	}

float distancesq( float4 a, float4 b )
	{
	return lengthsq( a - b );
	}

float distance( float4 a, float4 b )
	{
	return length( a - b );
	}

float4 saturate( float4 v )
	{
	float4 t( 
		v.x < 0.0f ? 0.0f : v.x,
		v.y < 0.0f ? 0.0f : v.y,
		v.z < 0.0f ? 0.0f : v.z,
		v.w < 0.0f ? 0.0f : v.w
		);
	
	return float4(
		t.x > 1.0f ? 1.0f : t.x,
		t.y > 1.0f ? 1.0f : t.y,
		t.z > 1.0f ? 1.0f : t.z,
		t.w > 1.0f ? 1.0f : t.w
		);
	}

float4 smoothstep( float4 min, float4 max, float4 v )
	{
	float4 result;
    
	// Scale, bias and saturate x to 0..1 range
    v.x = ( v.x - min.x ) / ( max.x - min.x ); 
    v.y = ( v.y - min.y ) / ( max.y - min.y ); 
    v.z = ( v.z - min.z ) / ( max.z - min.z ); 
    v.w = ( v.w - min.w ) / ( max.w - min.w ); 
	v = saturate( v );

    // Evaluate polynomial
    result.x = v.x * v.x * ( 3 - 2 * v.x );
	result.y = v.y * v.y * ( 3 - 2 * v.y );
	result.z = v.z * v.z * ( 3 - 2 * v.z );
	result.w = v.w * v.w * ( 3 - 2 * v.w );
	
	return result;
	}

float4 smootherstep( float4 min, float4 max, float4 v )
	{
	float4 result;
    
	// Scale, bias and saturate x to 0..1 range
    v.x = ( v.x - min.x ) / ( max.x - min.x ); 
    v.y = ( v.y - min.y ) / ( max.y - min.y ); 
    v.z = ( v.z - min.z ) / ( max.z - min.z ); 
    v.w = ( v.w - min.w ) / ( max.w - min.w ); 
	v = saturate( v );

    // Evaluate polynomial
    result.x = v.x * v.x * v.x * ( v.x * ( v.x * 6 - 15 ) + 10 );
	result.y = v.y * v.y * v.y * ( v.y * ( v.y * 6 - 15 ) + 10 );
	result.z = v.z * v.z * v.z * ( v.z * ( v.z * 6 - 15 ) + 10 );
	result.w = v.w * v.w * v.w * ( v.w * ( v.w * 6 - 15 ) + 10 );
	
	return result;
	}
