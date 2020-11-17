#ifndef _BEAM_H_
#define _BEAM_H_
//=============================================================================
//
// ビームのクラスヘッダー [beam.h]
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
#define BEAM_SIZE_X				(1)		// ビームのサイズ
#define BEAM_SIZE_Y				(1)		// ビームのサイズ
#define BEAM_SIZE_Z				(1)		// ビームのサイズ
#define BEAM_SPEED				(100)	// ビームのスピード

#define MAX_BEAM_TEXTURE		(1)		// テクスチャの数

//=============================================================================
// 背景クラス
//=============================================================================
class CBeam : public CBullet2
{
public:
	CBeam();		// コンストラクタ
	~CBeam();		// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, 
		D3DXVECTOR3 size, BULLET2_USER user);							// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理

	static CBeam *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, 
		D3DXVECTOR3 size, BULLET2_USER user);							// インスタンス生成
	static HRESULT Load(void);											// テクスチャロード
	static void UnLoad(void);											// テクスチャアンロード

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BEAM_TEXTURE];	// テクスチャ情報のポインタ
	static LPD3DXMESH m_pMesh;									// メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMat;								// マテリアル情報へのポインタ
	static DWORD m_nNumMat;										// マテリアル情報
};

#endif
