#include "Particle.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
	
	float4 sand = tex.Sample(samLinear, input.Tex);
	
	sand.a = sand.rgb;

	sand.rgb = diffuse.rgb;

	
	sand.rgb = diffuse.rgb;
	
	

	return sand;
}