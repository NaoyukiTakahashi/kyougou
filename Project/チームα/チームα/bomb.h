#ifndef _BOMB_H_
#define _BOMB_H_
//=============================================================================
//
// ボムのクラスヘッダー [bomb.h]
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
#define BOMB_SIZE_X				(3)		// ボムのサイズ
#define BOMB_SIZE_Y				(3)		// ボムのサイズ
#define BOMB_SIZE_Z				(3)		// ボムのサイズ
#define BOMB_SPEED				(30)	// ボムのスピード
#define BOMB_MOVE_Y				(4.0f)	// ボムのスピード
#define MAX_BOMB_TEXTURE		(1)		// テクスチャの数

//=============================================================================
// 背景クラス
//=============================================================================
class CBomb : public CBullet2
{
public:
	CBomb();		// コンストラクタ
	~CBomb();		// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos,
		D3DXVECTOR3 size, BULLET2_USER user);							// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理

	static CBomb *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,
		D3DXVECTOR3 size, BULLET2_USER user);							// インスタンス生成

	static HRESULT Load(void);											// テクスチャロード
	static void UnLoad(void);											// テクスチャアンロード
	void AddRot(void);													// 角度を加算

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BOMB_TEXTURE];	// テクスチャ情報のポインタ
	static LPD3DXMESH m_pMesh;									// メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMat;								// マテリアル情報へのポインタ
	static DWORD m_nNumMat;										// マテリアル情報
	int m_fAddRotNum;
};

#endif
