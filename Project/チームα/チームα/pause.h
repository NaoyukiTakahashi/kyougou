//=============================================================================
//
//  [pause.h]
// Author : 佐藤 颯紀
//
//=============================================================================

#ifndef  _PAUSE_H_
#define  _PAUSE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//前方宣言
//=============================================================================
class CScene2d;
class CUiPause;

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PAUSE			(3)		//ポーズUIの最大数

//=============================================================================
// クラス宣言
//=============================================================================
class CPause :public CScene
{
public:
	CPause(int nPriority = PRIORITY_1);
	~CPause();
	static CPause* Create(void);
	static HRESULT Load(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Select(void);

	static CUiPause *GetPause(int nCount) { return m_apPauselogo[nCount]; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[1];	//テクスチャへのポインタ
	CScene2d* m_pScene;	//ポインタ
	static CUiPause *m_apPauselogo[MAX_PAUSE];			//タイトルロゴのポインタ
	D3DXVECTOR3 m_SelectPos;					//ロゴのある座標
};

#endif // ! _PAUSE_H_