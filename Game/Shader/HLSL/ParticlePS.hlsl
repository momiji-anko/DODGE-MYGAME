#include "Particle.hlsli"
Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
	//�摜�\��
	float4 output = tex.Sample(samLinear, input.Tex);
	//�^�����Ȕ|���S��
	float4 outputw = float4(1, 1, 1, 1);
	
	//�F(RGBA)
	float4 white = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 red	 = float4(1.0f, 0.0f, 0.0f, 1.0f);
	float4 green = float4(0.0f, 1.0f, 0.0f, 1.0f);
	float4 bule  = float4(0.0f, 0.0f, 1.0f, 1.0f);
	float4 black = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 b = float4(0.5f, 0.5f, 0.5f, 1.0f);
	float4 fire = float4(1.0f, 1.0f, 0.1f, 1.0f);
	float4 sand = tex.Sample(samLinear, input.Tex);
	
	sand.a = sand.rgb;

	sand.rgb = diffuse.rgb;

	
	sand.rgb = diffuse.rgb;
	
	

	return sand;
}