#ifndef _BG_H_
#define _BG_H_
//=============================================================================
//
// ゲーム背景の処理 [bg.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "model.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define BG_SIZE_X				(1000)
#define BG_SIZE_Y				(500)
#define BG_SIZE_Z				(1000)
#define MAX_BG_TEXTURE			(1)

//=============================================================================
// 背景クラス
//=============================================================================
class CBg : public CModel
{
public:
	CBg();		// コンストラクタ
	~CBg();		// デストラクタ

	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	static CBg *Create(void);		// インスタンス生成
	static HRESULT Load(void);		// テクスチャロード
	static void UnLoad(void);		// テクスチャアンロード

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BG_TEXTURE];	// テクスチャ情報のポインタ
	D3DXVECTOR3 m_pos;										// 現在の位置
	D3DXVECTOR3 m_rot;										// 現在の角度
	D3DXVECTOR3 m_size;										// 大きさ
	D3DXMATRIX m_mtxWorld;									// 行列計算用
	static LPD3DXMESH m_pMesh;				// メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMat;			// マテリアル情報へのポインタ
	static DWORD m_nNumMat;					// マテリアル情報の数
};

#endif
