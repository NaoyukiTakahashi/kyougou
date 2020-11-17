//=============================================================================
//
// コンティニュー処理 [continue.h]
// Author : 佐藤颯紀
//
//=============================================================================

#ifndef _CONTINUE_H_
#define _CONTINUE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CNumber;
class CUi;
class CUiEnd;

//=============================================================================
// マクロ定義
//=============================================================================
#define CONTINUE_LIMIT		(15)		// 制限時間
#define MAX_CONTINUE		(2)			// 桁数
#define CONTINUE_SIZE_X		(50)		// 数字のＸサイズ
#define CONTINUE_SIZE_Y		(50)		// 数字のＹサイズ
#define CONTINUE_POS_X		(640.0f)	// タイムのX座標
#define CONTINUE_POS_Y		(300.0f)		// タイムのY座標
#define CONTINUE_INTERVAL	(50)		// 数字の置く間隔

//================================================
//クラス宣言
//================================================
class CContinue :public CScene
{
public:
	CContinue(int nPriority = PRIORITY_0);
	~CContinue();
	static CContinue *Create(D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddTime(int nValue);
	void SetTime(int nTime) { m_nContinue = nTime; }
	int GetTime(void) { return m_nContinue; }
	static CUi *GetUi(void) { return m_pUi; }
	bool GetEnd(void);
private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3	m_pos;						// ポリゴンの位置
	CNumber	*m_apContinue[MAX_CONTINUE];	// 数字の配置
	int m_nContinue;						// タイム
	int m_nContinueCount;					// タイムのカウンター
	static CUi *m_pUi;							// uiのポインタ
	CUiEnd *m_pUiEnd;			// コンティニュー
	bool m_bEnd;
	bool m_bTimeUp;
};
#endif // !_CONTINUE_H_

