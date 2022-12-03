
void AddSpecular(inout float4 color, float3 specular)
{
    color.rgb += specular * color.a;
}

float CalcShadow(float4 ZCalcTex)
{
    // ライト目線によるZ値の再算出
    float ZValue = ZCalcTex.z / ZCalcTex.w;

    // 射影空間のXY座標をテクスチャ座標に変換
    float2 TransTexCoord;
    TransTexCoord.x = (1.0f + ZCalcTex.x / ZCalcTex.w) * 0.5f;
    TransTexCoord.y = (1.0f - ZCalcTex.y / ZCalcTex.w) * 0.5f;

    // リアルZ値抽出
    float SM_Z = SM_Texture.Sample(SM_Sampler, TransTexCoord).x;

    // 算出点がシャドウマップのZ値よりも大きければ影と判断
    return (ZValue > SM_Z + 0.0001f) ? 0.5 : 1.0;
}
