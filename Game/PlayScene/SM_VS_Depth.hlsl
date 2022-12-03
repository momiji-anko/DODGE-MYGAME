#include "SM_Header.hlsli"

// シャドウマップ作成用の頂点シェーダー
float4 main(VSInput vin) : SV_POSITION
{
	// 座標変換しかやらない
	return mul(vin.Position, WorldViewProj);
}
