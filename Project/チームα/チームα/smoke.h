#ifndef _SMOKE_H_
#define _SMOKE_H_
//=============================================================================
//
// 煙のクラスヘッダー [smoke.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "bullet2.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SMOKE_SIZE_X			(20)			// 煙のサイズ
#define SMOKE_SIZE_Z			(20)			// 煙のサイズ
#define SMOKE_SIZE_Y			(4)			// 煙のサイズ

#define MAX_SMOKE_TEXTURE		(1)			// テクスチャの数

//=============================================================================
// 背景クラス
//=============================================================================
class CSmoke : public CModel
{
public:
	CSmoke();		// コンストラクタ
	~CSmoke();		// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理

	static CSmoke *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,
		D3DXVECTOR3 size);												// インスタンス生成
	static HRESULT Load(void);											// テクスチャロード
	static void UnLoad(void);											// テクスチャアンロード

	void ScaleUp(void);													// 拡大
	void AddRot(void);													// 角度を加算
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_SMOKE_TEXTURE];	// テクスチャ情報のポインタ
	static LPD3DXMESH m_pMesh;									// メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMat;								// マテリアル情報へのポインタ
	static DWORD m_nNumMat;										// マテリアル情報
	D3DXVECTOR3 m_FirstSize;									// 最初のサイズ
	float m_fScale;												// 拡縮用
	float m_fScaleNum;											// 拡縮用の値
	float m_fAddRotNum;											// 角度の加算
	float m_fAlphaNum;											// 透明度
};

#endif
