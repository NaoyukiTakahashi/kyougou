//=============================================================================
//
//  [tutorial.h]
// Author : 佐藤颯紀
//
//=============================================================================

#ifndef  _TUTORIAL_H_
#define  _TUTORIAL_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//前方宣言
//=============================================================================
class CScene2d;

//=============================================================================
//マクロ定義
//=============================================================================

//=============================================================================
//クラス宣言
//=============================================================================
class CTutorial : public CScene
{
public:
	CTutorial(int nPriority = PRIORITY_0);
	~CTutorial();
	static CTutorial* Create(void);
	static HRESULT Load(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[1];	//テクスチャへのポインタ
	CScene2d* m_pScene;	//ポインタ
};

#endif // ! _TUTORIAL_H_

