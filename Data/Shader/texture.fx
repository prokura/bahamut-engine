//--------------------------------------------------------------------------------------
// basicEffect.fx
//
//--------------------------------------------------------------------------------------

Texture2D tex2D;
SamplerState linearSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

uniform float2 resolution;

struct Transform
{
	float2 position;
	float2 scale;
	float angle;
};

Transform t;

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD_centroid;
};

struct VS_INPUT
{
	float2 Pos : POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT ps_input;
	
	float4 p;
	p.x = (input.Pos.x * t.scale.x ) + t.position.x;
	p.y = (input.Pos.y * t.scale.y ) + t.position.y;
	p.z = 0;
	p.w = 1;

	p.x += 0.5/resolution.x;
	p.y += 0.5/resolution.y;

	ps_input.Pos = p;
	ps_input.Color = input.Color;
	ps_input.Tex = input.Tex;
	
    return ps_input;  
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 textured( PS_INPUT input ) : SV_Target
{
    return tex2D.Sample( linearSampler, input.Tex ); 
}

float4 noTexture( PS_INPUT input ) : SV_Target
{
    return input.Color; 
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 Main
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, textured() ) );
    }
}

technique10 texturingDisabled
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, noTexture() ) );
    }
}