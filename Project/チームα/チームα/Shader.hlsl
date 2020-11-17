// グローバル変数宣言ブロック
float4x4 	g_Matrix;	// 合成用マトリクス
float4		g_Color;	// カラー情報

// 頂点シェーダプログラムブロック
void VertexShaderFunction(float3 in_pos : POSITION,
		out float4 out_pos : POSITION,
		out float4 out_color :COLOR0)
{
	// 座標変換(ローカル座標と掛け合わせ)
	out_pos = mul(float4(in_pos, 1.0f), g_Matrix);

	// 頂点の色の決定(そのまま使用)
	out_color = g_Color;
}

// ピクセルシェーダプログラムブロック
void PixelShaderFunction(float4 in_color : COLOR0,
	out float4 out_color : COLOR0)
{
	// 色をそのまま使用
	out_color = in_color;
}


// テクニック宣言
technique StandardDraw
{
	pass normal
	{
		vertexShader = compile vs_3_0 VertexShaderFunction();
		pixelShader  = compile ps_3_0 PixelShaderFunction();
	}
}
