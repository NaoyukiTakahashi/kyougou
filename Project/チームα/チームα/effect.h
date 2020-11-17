#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
//
// エフェクトクラスヘッダー	[effect.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "billboard.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define EFFECT_SIZE_X			(120)	// サイズ
#define EFFECT_SIZE_Y			(120)	// サイズ
#define MAX_EFFECT_TEXTURE		(1)		// テクスチャ数

#define EFFECT_LASER_SIZE_X		(250)	// サイズ
#define EFFECT_LASER_SIZE_Y		(250)	// サイズ

#define EFFECT_LIFE				(7)		// エフェクトの体力
#define EFFECT_LASER_LIFE		(20)	// エフェクトの体力

//=============================================================================
// 前方宣言
//=============================================================================
class CBillboard;

//=============================================================================
// BGクラス
//=============================================================================
class CEffect : public CBillboard
{
public:

	CEffect();			// コンストラクタ
	~CEffect();			// デストラクタ

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);		// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	static CEffect * Create(										// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size,
		D3DXCOLOR col, int nLife);

	void SetMove(D3DXVECTOR3 move);									// 移動量設定
	static HRESULT Load(void);										// テクスチャロード
	static void UnLoad(void);										// テクスチャアンロード
	void ScaleDown(void);											// 縮小
	void SetColor(D3DXCOLOR col);									// カラーの初期化
private:
	D3DXVECTOR3 m_Pos;												// 座標
	D3DXVECTOR3 m_move;												// 移動量
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// 頂点バッファのポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_EFFECT_TEXTURE];		// テクスチャ情報のポインタ
	bool m_bScaleDown;												// 拡大終わりフラグ
	float m_fScale;													// スケール
	float m_fScaleNum;												// スケールの値
	int m_nLife;													// ライフ
	D3DXCOLOR m_col;												// カラー
};

#endif