#ifndef _SHAPE_H_
#define _SHAPE_H_

//=====================================================
//
// 球メッシュのクラス [shape.h]
// Author : Konishi Yuuto
//
//=====================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "model.h"

//=============================================================================
// マクロ定義
//=============================================================================
//#define FIELD_WIDTH					(4)
//#define FIELD_HEIGHT				(4)
//#define FIELD_WIDTH_SIZE			(10000.0f)
//#define FIELD_HEIGHT_SIZE			(10000.0f)
//
//#define FIELD_WIDTH_SIZE_TUTORIAL	(300.0f)
//#define FIELD_HEIGHT_SIZE_TUTORIAL	(300.0f)
//
//#define MAX_MESHFIELD_TEXTURE		(1)				// テクスチャの数

//=============================================================================
// メッシュフィールドクラス
//=============================================================================
class CMeshShape : public CModel
{
public:
	CMeshShape();			// コンストラクタ
	~CMeshShape();			// デストラクタ

	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	static CMeshShape*Create(void);		// メッシュフィールド
	static HRESULT Load(void);			// テクスチャロード
	static void UnLoad(void);			// テクスチャアンロード
	D3DXVECTOR3 GetPos(void);
private:
	static LPD3DXMESH m_pMesh;				// メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMat;			// マテリアル情報へのポインタ
	static DWORD m_nNumMat;					// マテリアル情報の数
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_MESHFIELD_TEXTURE];	// テクスチャ情報のポインタ
	D3DXVECTOR3 m_pos;						// 座標
	D3DXVECTOR3 m_move;						// 移動
};


#endif
