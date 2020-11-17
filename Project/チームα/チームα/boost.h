#ifndef _BOOST_H_
#define _BOOST_H_

//=============================================================================
//
// ブースターの処理 [boost.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=========================================================================
// インクルード
//=========================================================================
#include "model.h"

//=========================================================================
// マクロ定義
//=========================================================================
#define BOOST_SIZE_X	(1.0f)
#define BOOST_SIZE_Y	(1.0f)
#define BOOST_SIZE_Z	(1.0f)
#define BOOST_LIFE		(15)

#define MAX_BOOST		(2)
#define MAX_LASER_TEXTURE		(1)		// テクスチャの数

//=========================================================================
// ブースタークラス
//=========================================================================
class CBoost :public CModel
{
public:
	CBoost();
	~CBoost();

	static CBoost *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, int nNumber);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nNum);		// 初期化
	void Uninit(void);													// 終了
	void Update(void);													// 更新
	void Draw(void);													// 描画
	void BoostMove(void);												// ブーストの更新処理
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_LASER_TEXTURE];	// テクスチャ情報のポインタ
	static LPD3DXMESH m_pMesh;									// メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMat;								// マテリアル情報へのポインタ
	static DWORD m_nNumMat;										// マテリアル情報
	D3DXVECTOR3 m_pos;						// 座標
	D3DXVECTOR3 m_rot;						// 回転
	static int m_nBoost;							// boostナンバー
	int m_nBoostNum;
	int m_nLife;
	int m_nNum;								// ナンバー
};
#endif