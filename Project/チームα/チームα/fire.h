#ifndef _FIRE_H_
#define _FIRE_H_

//=============================================================================
//
// 炎クラスヘッダー [fire.h]
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
#define FIRE_SIZE_X			(40)	// 横サイズ
#define FIRE_SIZE_Y			(40)	// 縦サイズ
#define MAX_FIRE_TEXTURE	(1)		// テクスチャ数
#define FIRE_LIFE			(12)	// スパークの体力
#define FIRE_NUM			(30)	// スパークの数
#define FIRE_SPPED			(1)		// スピード
#define FIRE_DISTANCE		(10)	// 発生距離

//=============================================================================
// 前方宣言
//=============================================================================
class CBillboard;

//=============================================================================
// BGクラス
//=============================================================================
class CFire : public CBillboard
{
public:
	CFire();			// コンストラクタ
	~CFire();			// デストラクタ

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	static CFire * Create(										// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 size,
		int nLife);

	static HRESULT Load(void);										// テクスチャロード
	static void UnLoad(void);										// テクスチャアンロード

	void SetColor(D3DXCOLOR col);									// パーティクルのカラーを設定
	void SetLife(int nLife);										// ライフの設定
	void ScaleDown(void);											// 縮小
	void Alpha(void);												// 透明度
private:
	int m_nLife;												// 体力
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_FIRE_TEXTURE];	// テクスチャ情報のポインタ
	float m_fScale;												// 拡縮用
	float m_fScaleNum;											// 拡縮の値
	float m_fAlpha;												// 透明用
	float m_fAlphaNum;											// 透明の値
};

#endif