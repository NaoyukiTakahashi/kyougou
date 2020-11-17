//=======================================================================================
// 
// ポリゴン描画処理(scene2d.h)
// Author : 伊藤　航
//
//=======================================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//***************************************************************************************
// インクルードファイル
//***************************************************************************************
#include "scene.h"

//***************************************************************************************
// クラス定義
//***************************************************************************************
class CScene2d : public CScene
{
public:

	CScene2d(int nPriority = PRIORITY_0);
	~CScene2d();
	static CScene2d* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTextuer) { m_pTextuer = pTextuer; }
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPolygonSize(const D3DXVECTOR3 nSize) { m_PolygonSize = nSize; }
	void SetCol(D3DXCOLOR col);
	void SetAnim(int PatternAnim, float Countea);
	void SetRotation(float rotasion);
	D3DXVECTOR3 GetPos(void)const { return m_pos; }
	D3DXVECTOR3 GetSize(void)const { return m_PolygonSize; }

	D3DXCOLOR   GetCol(void) { return m_col; }
	
private:


	LPDIRECT3DTEXTURE9 m_pTextuer;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVetxBuff;	 // 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;						// ポリゴンの位置
	D3DXVECTOR3 m_PolygonSize;				// ポリゴンのサイズ
	D3DXCOLOR   m_col;						// ポリゴンのカラー	
	D3DXVECTOR2 m_tex;						// テクスチャー設定

	float m_fRotasion;
};

#endif // !_SCENE2D_H_
