//*** float2.cpp ***

#include "float2.h"
#include "float3.h"
#include "float4.h"

#include <math.h>

const float2 float2::zero( 0, 0 );
const float2 float2::one( 1, 1 );
const float2 float2::right( 1, 0 );
const float2 float2::left( -1, 0 );
const float2 float2::up( 0, 1 );
const float2 float2::down( 0, -1 );

//*** Constructors ***

float2::float2()
    {
	x = 0;
	y = 0;
	}

float2::float2( float2_t v )
	{
	x = v.x;
	y = v.y;
	}

float2::float2( float x, float y )
    {
    this->x = x;
    this->y = y;
	}

float2::float2( float v )
    {
    x = v;
    y = v;
    }

float2::float2( const float2& v )
    {
	x = v.x;
	y = v.y;
    }

float2::float2( float3 v )
    {
    x = v.x;
    y = v.y;
    }

float2::float2( float4 v )
    {
    x = v.x;
    y = v.y;
    }


//*** Operators ***

float2::operator float2_t() const
	{
	return * ( (float2_t*) this );
	}

float2 float2::operator=( float2 v )
    {
    x = v.x;
    y = v.y;
    return *this;
    }

float2 float2::operator+=( float2 v )
    {
    x += v.x;
    y += v.y;
    return *this;
    }

float2 float2::operator*=( float2 v )
    {
    x *= v.x;
    y *= v.y;
    return *this;
    }

float2 float2::operator/=( float2 v )
    {
    x /= v.x;
    y /= v.y;
    return *this;
    }

float2 float2::operator+=( float s )
    {
    x += s;
    y += s;
    return *this;
    }

float2 float2::operator-=( float s )
    {
    x -= s;
    y -= s;
    return *this;
    }

float2 float2::operator*=( float s )
    {
    x *= s;
    y *= s;
    return *this;
    }

float2 float2::operator/=( float s )
    {
    x /= s;
    y /= s;
    return *this;
    }



//*** Operators ***

float2 operator+( float2 a, float2 b )
    {
    return float2( a.x + b.x, a.y + b.y );
    }
                          
float2 operator-( float2 a, float2 b )
    {
    return float2( a.x - b.x, a.y - b.y );
    }
                          
float2 operator*( float2 a, float2 b )
    {
    return float2( a.x * b.x, a.y * b.y );
    }
                          
float2 operator/( float2 a, float2 b )
    {
    return float2( a.x / b.x, a.y / b.y );
    }

float2 operator+( float s, float2 v )
	{
	return float2( s + v.x, s + v.y );
	}
float2 operator-( float s, float2 v )
	{
	return float2( s - v.x, s - v.y );
	}
float2 operator*( float s, float2 v )
	{
	return float2( s * v.x, s * v.y );
	}
float2 operator/( float s, float2 v )
	{
	return float2( s / v.x, s / v.y );
	}
float2 operator+( float2 v, float s )
	{
	return float2( v.x + s, v.y + s );
	}
float2 operator-( float2 v, float s )
	{
	return float2( v.x - s, v.y - s );
	}
float2 operator*( float2 v, float s )
	{
	return float2( v.x * s, v.y * s );
	}
float2 operator/( float2 v, float s )
	{
	return float2( v.x / s, v.y / s );
	}

float2 operator-( float2 v )
	{
	return float2( -v.x, -v.y );
	}

bool operator==( float2 a, float2 b )
	{
	return a.x == b.x && a.y == b.y;
	}

bool operator!=( float2 a, float2 b )                         
	{
	return !( a == b);
	}


//*** Functions ***

float lengthsq( float2 v )
    {
    return v.x * v.x + v.y * v.y;
    }
    
float length( float2 v )
    {
    return sqrtf( lengthsq( v ) );
    }

float2 normalize( float2 v )
    {
    // skip divide if length is zero
    float const len = length( v );
    return ( len > 0 ) ? v / len : v;
    }

float dot( float2 a, float2 b )
    {
    return a.x * b.x + a.y * b.y;
    }
    
float2 cross( float2 a, float2 b )
    {
    return float2( a.y * b.x - a.x * b.y, a.x * b.y - a.y * b.x );
    }

float2 lerp( float2 a, float2 b, float s )
    {
	return float2( a.x + ( b.x - a.x ) * s, a.y + ( b.y - a.y ) * s );
	}

float distancesq( float2 a, float2 b )
	{
	return lengthsq( a - b );
	}

float distance( float2 a, float2 b )
	{
	return length( a - b );
	}

float2 saturate( float2 v )
	{
	float2 t( 
		v.x < 0.0f ? 0.0f : v.x,
		v.y < 0.0f ? 0.0f : v.y
		);
	
	return float2(
		t.x > 1.0f ? 1.0f : t.x,
		t.y > 1.0f ? 1.0f : t.y
		);
	}

float2 smoothstep( float2 min, float2 max, float2 v )
	{
	float2 result;
    
	// Scale, bias and saturate x to 0..1 range
    v.x = ( v.x - min.x ) / ( max.x - min.x ); 
    v.y = ( v.y - min.y ) / ( max.y - min.y ); 
	v = saturate( v );

    // Evaluate polynomial
    result.x = v.x * v.x * ( 3 - 2 * v.x );
	result.y = v.y * v.y * ( 3 - 2 * v.y );
	
	return result;
	}


float2 smootherstep( float2 min, float2 max, float2 v )
	{
	float2 result;
    
	// Scale, bias and saturate x to 0..1 range
    v.x = ( v.x - min.x ) / ( max.x - min.x ); 
    v.y = ( v.y - min.y ) / ( max.y - min.y ); 
	v = saturate( v );

    // Evaluate polynomial
    result.x = v.x * v.x * v.x * ( v.x * ( v.x * 6 - 15 ) + 10 );
	result.y = v.y * v.y * v.y * ( v.y * ( v.y * 6 - 15 ) + 10 );
	
	return result;
	}
	