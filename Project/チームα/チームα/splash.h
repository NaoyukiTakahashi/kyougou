#ifndef _SPLASH_H_
#define _SPLASH_H_

//=====================================================
//
// スプラッシュクラスヘッダー [splash.h]
// Author : Konishi Yuuto
//
//=====================================================

//=====================================================
// インクルード
//=====================================================
#include "billboard.h"

//=====================================================
// マクロ定義
//=====================================================
#define SPLASH_SIZE_X_2D	(300)
#define SPLASH_SIZE_Y_2D	(300)
#define SPLASH_SIZE_Z_2D	(300)

//=====================================================
// 前方宣言
//=====================================================
class CBillboard;

//=====================================================
// ビルボードクラス
//=====================================================
class CSplash : public CBillboard
{
public:
	CSplash();									// コンストラクタ
	~CSplash();								// デストラクタ

	static CSplash *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	static HRESULT Load(void);										// テクスチャロード
	static void UnLoad(void);										// テクスチャアンロード

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;						// ポリゴンのテクスチャ
};

#endif