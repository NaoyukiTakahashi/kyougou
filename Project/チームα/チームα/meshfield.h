#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//=====================================================
//
// メッシュフィールドのクラス [meshfield.h]
// Author : Konishi Yuuto
//
//=====================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FIELD_WIDTH					(4)
#define FIELD_HEIGHT				(4)
#define FIELD_WIDTH_SIZE			(7000.0f)
#define FIELD_HEIGHT_SIZE			(7000.0f)

#define FIELD_WIDTH_SIZE_TUTORIAL	(300.0f)
#define FIELD_HEIGHT_SIZE_TUTORIAL	(300.0f)

#define MAX_MESHFIELD_TEXTURE		(1)				// テクスチャの数

//=============================================================================
// メッシュフィールドクラス
//=============================================================================
class CMeshField
{
public:
	CMeshField();			// コンストラクタ
	~CMeshField();			// デストラクタ

	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	static CMeshField*Create(void);		// メッシュフィールド
	static HRESULT Load(void);			// テクスチャロード
	static void UnLoad(void);			// テクスチャアンロード
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_MESHFIELD_TEXTURE];	// テクスチャ情報のポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;						
	D3DXVECTOR3 m_pos;												// 位置
	D3DXVECTOR3 m_rot;												// 向き（回転）
	D3DXMATRIX m_mtxWorld;											// ワールドマトリックス
	int m_nNumVertex;												// 総頂点数
	int m_nNumIndex;												// 総インデックス数
	int m_nNumPolygon;												// 総ポリゴン数
	float m_fOne_SizeW;
	float m_fOne_SizeH;
};


#endif
