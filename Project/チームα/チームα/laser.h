#ifndef _LASER_H_
#define _LASER_H_
//=============================================================================
//
// レーザーのクラスヘッダー [beam.h]
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
#define LASER_SIZE_X			(6)		// ビームのサイズ
#define LASER_SIZE_Y			(5)		// ビームのサイズ
#define LASER_SIZE_Z			(6)		// ビームのサイズ
#define LASER_SPEED				(200)	// ビームのスピード

#define MAX_LASER_TEXTURE		(1)		// テクスチャの数

//=============================================================================
// 背景クラス
//=============================================================================
class CLaser : public CBullet2
{
public:
	CLaser();		// コンストラクタ
	~CLaser();		// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
		D3DXVECTOR3 size, BULLET2_USER user);							// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理

	void Scale(void);													// 拡縮
	static CLaser *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,
	D3DXVECTOR3 rot, D3DXVECTOR3 size, BULLET2_USER user);				// インスタンス生成
	static HRESULT Load(void);											// テクスチャロード
	static void UnLoad(void);											// テクスチャアンロード

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_LASER_TEXTURE];	// テクスチャ情報のポインタ
	static LPD3DXMESH m_pMesh;									// メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMat;								// マテリアル情報へのポインタ
	static DWORD m_nNumMat;										// マテリアル情報
	float m_fFlash;												// 点滅用
	float m_fFlashNum;											// 点滅用の値
	float m_fScale;												// 拡大用
	float m_fScaleNum;											// 拡大用の値
	float m_fSizeZ;												// レーザーの長さ
};

#endif
