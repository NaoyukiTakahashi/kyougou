#ifndef _LOCUS_H_
#define _LOCUS_H_

//=============================================================================
//
// 軌跡の処理 [locus.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=========================================================================
// インクルード
//=========================================================================
#include "scene3D.h"

//=========================================================================
// マクロ定義
//=========================================================================
#define LOCUS_SIZE_X	(30.0f)
#define LOCUS_SIZE_Y	(100.0f)
#define LOCUS_SIZE_Z	(50.0f)
#define LOCUS_LIFE		(8)

//=========================================================================
// 軌跡クラス
//=========================================================================
class CLocus :public CScene3D
{
public:
	CLocus();
	~CLocus();

	static CLocus *Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2,
		D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2, D3DXVECTOR3 rot, D3DXVECTOR3 size, int nLife);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);		// 初期化
	HRESULT Init(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2,
		D3DXVECTOR3 posOld1, D3DXVECTOR3 posOld2);		// 初期化

	void Uninit(void);													// 終了
	void Update(void);													// 更新
	void Draw(void);													// 描画
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャポインタ
	int m_nLife;								// ライフ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファポインタ
	D3DXMATRIX	m_mtxWorld;						// ワールドマトリクス
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 古い座標
	D3DXVECTOR3 m_size;							// サイズ
	D3DXVECTOR3 m_rot;							// 向き

};
#endif