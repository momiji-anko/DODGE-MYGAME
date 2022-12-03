#include "SM_Header.hlsli"
#include "SM_Common.hlsli"

// 頂点カラーのみのピクセルシェーダー
float4 main(VSOutput pin) : SV_TARGET0
{
    float4 color = pin.Diffuse;

    AddSpecular(color, pin.Specular);

    return saturate(color * CalcShadow(pin.ZCalcTex));
}
