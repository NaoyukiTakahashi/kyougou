//=============================================================================
//
//  [title.h]
// Author : 伊藤　航
//
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_
//***************************************************************************************
// インクルードファイル
//***************************************************************************************
#include "scene.h"

class CScene2d;
class CTitlelogo;

//***************************************************************************************
// マクロ定義
//***************************************************************************************
#define MAX_TITLE (3)

//***************************************************************************************
// タイトルクラス
//***************************************************************************************
class CTitle : public CScene
{
public:
	CTitle(int nPriority = PRIORITY_0);
	~CTitle();
	static CTitle* Create(void);
	static HRESULT Load(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static CTitlelogo *GetLogo(int nCount) { return m_apTitlelogo[nCount]; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[1];	//テクスチャへのポインタ
	CScene2d* m_pScene;	//ポインタ
	bool m_bModechenge;
	static CTitlelogo *m_apTitlelogo[MAX_TITLE];			//タイトルロゴのポインタ
};

#endif // !_TITEL_H_