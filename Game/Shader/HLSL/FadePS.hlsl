#include "Fade.hlsli"
Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
Texture2D tex3 : register(t2);
SamplerState samLinear : register(s0);

float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}

float noise(float2 uv)
{
    float2 p = floor(uv);
    return random(p);
}

float perlinNoise(float2 uv)
{
    float2 p = floor(uv);
    float2 f = frac(uv);
    float2 u = f * f * (3.0f - 2.0f * f);
    float v00 = -1 + 2 * random(p + float2(0, 0));
    float v10 = -1 + 2 * random(p + float2(1, 0));
    float v01 = -1 + 2 * random(p + float2(0, 1));
    float v11 = -1 + 2 * random(p + float2(1, 1));

    return lerp(
	lerp(dot(v00, f - float2(0, 0)), dot(v10, f - float2(1, 0)), u.x),
	lerp(dot(v01, f - float2(0, 1)), dot(v11, f - float2(1, 1)), u.x),
	u.y) + 0.5f;
}


float fBm(float2 uv)
{
    float f = 0;
    float2 q = uv;
    f += 0.5000f * perlinNoise(q);
    q = q * 2.01f;
    f += 0.2500f * perlinNoise(q);
    q = q * 2.02f;
    f += 0.2500f * perlinNoise(q);
    return f;
}

float WhiteNoise(float2 coord)
{
    return frac(sin(dot(coord, float2(8.7819, 3.255))) * 437.645);
}

//画像の色をセピアに変更
float4 sepia(float4 col)
{
    return float4(col.r * 1.44f, col.g * 0.99f, col.b * 0.57f, col.a);
}

//画像をグレースケール化
float gray(float4 col)
{
    return col.r * 0.3f + col.g * 0.6f + col.b * 0.1f;
}



float4 blur(float2 uv)
{
    float power = 0.1f;
    float center = 1.0f - power * 9.0f;

    float4 output = 0;
    for (int i = 0; i < 9; i++)
    {
        output += tex.Sample(samLinear, uv
		+ float2(((i % 3) - 1) * 0.01f, ((i / 3) - 1) * 0.01f)) * power;
    }
    output += tex.Sample(samLinear, uv) * center;

    return output;
}

float4 motionBlur(float2 uv)
{
    float power = 0.1f;

    float4 output = 0;
    for (int i = 0; i < 9; i++)
    {
        output += tex.Sample(samLinear, uv
		+ float2(sin(Time.x - i * 0.1f) * 0.05f, 0.0f)) * power * (10.0f - i) / 5.0f;
    }
	
    return output;
}


float4 mosaic(float2 baseUV, float mag)
{
    return tex.Sample(samLinear, floor(baseUV * mag) / mag);
}

/*
歪み
center：ゆがませる位置
r：半径
*/
float distortion(float2 center, float r, float d_r, float2 uv)
{
	//今のピクセルと中心座標の距離
    float dist = distance(center, uv);
	//半径d_rの歪みを作る。
    return 1 - smoothstep(r, d_r, dist);
}

float4 oldTV(float2 inUV)
{
    float2 uv = inUV;
    
    //中心をずらす
    uv = uv - float2(0.5f, 0.5f);
    float vignette = length(uv);
    uv /= 1 - vignette * 0.2f;
    
    //中心に戻す
    float2 texUV = uv + float2(0.5f, 0.5f);
    
    texUV.x += (random(floor(texUV.y * 100.0f) + Time.x) - 0.5f) * 0.02f;
    
    float4 base = tex.Sample(samLinear, texUV);
    
    float3 col;
    col.r = tex.Sample(samLinear, texUV).r;
    col.g = tex.Sample(samLinear, texUV + float2(0.002f, 0)).g;
    col.b = tex.Sample(samLinear, texUV + float2(0.004f, 0)).b;
    col = lerp(col, float3(
		random(uv + float2(123 + Time.x, 0)),
		random(uv + float2(123 + Time.x, 1)),
		random(uv + float2(123 + Time.x, 2))),
		step(random(floor(texUV.y * 500) + Time.x), 0.01f));
    col *= 1 - vignette * 1.3f;
    base = float4(col, base.a);

    //中心以外（画面外）は黒く塗りつぶす
   base = lerp(base, float4(0, 0, 0, 1), step(0, max(abs(uv.y) - 0.5f, abs(uv.x) - 0.5f)));

    return base;
}

//画像無しフェード
//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float4 fade = float4(0, 0, 0, 0);
//    fade.a = sin(Time.x);
//    return fade;
//}

////画像付きフェード
//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float4 fade = tex.Sample(samLinear, input.Tex);
//    fade.a = sin(Time.x);
//    return fade;
//}

//画像に応じたフェード( if 文有)
//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float4 fade = tex.Sample(samLinear, input.Tex);
//    if (fade.b > sin(Time.x))
//    {
//        fade.a = 0.0f;
//    }
//    return fade;
//}

//画像に応じたフェード（if文無）
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 fade = tex.Sample(samLinear, input.Tex);
    fade.a = 1.0f - ceil(fade.b - sin(Time.x));
    fade.rgb = float3(0, 0, 0);
    return fade;
}

//画像情報を複数使用して混ぜて表示する。
//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float4 fade = tex.Sample(samLinear, input.Tex);
//    float4 fade2 = tex2.Sample(samLinear, input.Tex);
//    return lerp(fade, fade2, sin(Time.x));
//}

//別の画像色情報等を元に、透明化するタイミングをずらす。
//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float4 fade = tex.Sample(samLinear, input.Tex);
//    float4 fade2 = tex2.Sample(samLinear, input.Tex);
//    float alpha = fade2.r -
//    sin(Time.x);
//    fade.a = alpha;
//    return fade;

//}

//別の色（白など）に変化するパターン
//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float4 fade = tex.Sample(samLinear, input.Tex);
//    float4 fade2 = tex2.Sample(samLinear, input.Tex);
//    float alpha = fade2.r -
//    sin(Time.x);
//    return lerp(float4(1, 1, 1, 1), fade, alpha);
//}

//はっきり変化させるパターン
//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float4 fade = tex.Sample(samLinear, input.Tex);
//    float4 fade2 = tex2.Sample(samLinear, input.Tex);
//    float alpha = fade2.r -
//    sin(Time.x);
//    fade.a = step(alpha, 0);
//    return fade;
//}

//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float4 fade = tex.Sample(samLinear, input.Tex);
//    float4 fade2 = tex2.Sample(samLinear, input.Tex);
//    float alpha = fade2.r - sin(Time.x);
//    fade = lerp(float4(0, 1, 1, 1), fade, step(alpha, 0.0f));
//   // fade = lerp(float4(0, 0, 0, 0), fade, step(alpha, 0.02f));
//    return fade;
//}


//float4 main(PS_INPUT input) : SV_TARGET
//{
//    float4 img = tex.Sample(samLinear, input.Tex);
//    float n = noise(input.Tex * round(Time.x));

//    return lerp(float4(0, 0, 0, 0), img, step(n - sin(Time.x), 0));

//}



//float4 main(PS_INPUT input) : SV_TARGET
//{
//    //float2 samplePoint = input.Tex;
//    float4 Tex = tex.Sample(samLinear, input.Tex);
//    //float noise = WhiteNoise(input.Tex * Time.x) - 0.5;
//    //Tex.rgb += float3(noise, noise, noise);
//    return Tex;

//}

