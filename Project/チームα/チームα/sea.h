#ifndef _SEA_H_
#define _SEA_H_

//=====================================================
//
// 海（メッシュフィールド）のクラス [sea.h]
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
#define SEA_WIDTH					(4)
#define SEA_HEIGHT				(4)
#define SEA_WIDTH_SIZE			(50000.0f)
#define SEA_HEIGHT_SIZE			(50000.0f)
#define SEA_HEIGHT_POS_Y		(-500.0f)

#define MAX_SEA_TEXTURE		(1)				// テクスチャの数

//=============================================================================
// メッシュフィールドクラス
//=============================================================================
class CSea
{
public:
	CSea();			// コンストラクタ
	~CSea();			// デストラクタ

	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	static CSea*Create(void);		// メッシュフィールド
	static HRESULT Load(void);			// テクスチャロード
	static void UnLoad(void);			// テクスチャアンロード
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_SEA_TEXTURE];	// テクスチャ情報のポインタ
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
