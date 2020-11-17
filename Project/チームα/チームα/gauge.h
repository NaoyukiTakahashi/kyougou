//=============================================================================
// 
// ゲージ描画処理(gauge.h)
// Author : 佐藤颯紀
//
//=============================================================================

#ifndef  _GAUGE_H_
#define  _GAUGE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//マクロ定義
//=============================================================================

//=============================================================================
//クラス宣言
//=============================================================================
class CGauge : public CScene
{
public:
	CGauge(int nPriority = PRIORITY_0);
	~CGauge();
	static CGauge* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTextuer) { m_pTextuer = pTextuer; }
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize( D3DXVECTOR3 nSize) { m_size = nSize; }
	void SetCol(D3DXCOLOR col);

	D3DXVECTOR3 GetPos(void)const { return m_pos; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXCOLOR   GetCol(void) { return m_col; }
private:

	LPDIRECT3DTEXTURE9 m_pTextuer;       // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVetxBuff; // 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;                   // ポリゴンの位置
	D3DXVECTOR3 m_size;					 // ポリゴンのサイズ
	D3DXCOLOR   m_col;
};

#endif // ! _GAUGE_H_
