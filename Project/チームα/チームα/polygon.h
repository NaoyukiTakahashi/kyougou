//=============================================================================
//
// メイン処理 [polygon.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//ライフクラス
//=============================================================================
class CPolygon
{
public:
	//=========================================================================
	//列挙型定義
	//=========================================================================
	typedef enum
	{
		TEX_TYPE_NORE = 0,
		TEX_TYPE_TITLE,	//タイトルテクスチャ
		TEX_TYPE_RESULET,	//リザルトテクスチャ
		TEX_TYPE_LIFE_GAUGE,	//ライフテクスチャ
		TEX_TYPE_UNDERUI,	//アンダーUIテクスチャ
		TEX_TYPE_CHEETAH,	//チーター
		TEX_TYPE_GORILLA,	//ゴリラ
		TEX_TYPE_TURTLE,	//カメ
		TEX_TYPE_PRESSENTER,	//PRESS ENTERテクスチャ
		TEX_TYPE_MAX,	//最大テクスチャ
	}TEX_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CPolygon();
	~CPolygon();

	static CPolygon *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(const TEX_TYPE type);
	void SetColor(const D3DXCOLOR color);
	

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_TYPE_MAX];	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 m_Size;	//ポリゴンのサイズ
	D3DXVECTOR3 m_pos;	//ライフの位置
	TEX_TYPE m_Type;
};
#endif 