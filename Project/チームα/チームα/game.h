#ifndef _GAME_H_
#define _GAME_H_
//=============================================================================
//
// ゲームの処理 [game.h]
// Author : Konishi Yuuto
//
//=============================================================================

//***************************************************************************************
// インクルードファイル
//***************************************************************************************
#include "scene.h"

//***************************************************************************************
// マクロ定義
//***************************************************************************************
#ifdef VIIEW_PORT_TEST
#endif // VIIEW_PORT_TEST
#define MAX_PLAYER (2)			// プレイヤーの数

//***************************************************************************************
// 前方宣言
//***************************************************************************************
class CCamera;
class CLight;
class CMeshField;
class CMeshShape;
class CBg;
class CPlayer;
class CTime;
class CUi;
class CUiStart;
class CLockon;
class CBill;
class CContinue;
class CUiEnd;
class CPause;
class CSea;

//***************************************************************************************
// インクルードファイル
//***************************************************************************************
class CGame : public CScene
{
public:
	CGame(int nPriority = PRIORITY_0);
	~CGame();
	static CGame* Create();
	static CCamera *GetCamera(int nCount);
	static CLight *GetLight(void);
	static CPlayer *GetPlayer(int nCount);
	static CTime *GetTime(void);
	static CMeshShape*GetSphere();
	static CUi *GetUi(void);
	static CUiStart *GetUiStart(void);
	static CUiEnd *GetUiEnd(void);
	static CLockon *GetLockon(void);
	static CBill *GetBill(void);
	static CContinue *GetContinue(void);
	static CPause *GetPause(void);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetGame(void);
	void ResetGame(void);

private:	static CCamera *m_apCamera[MAX_PLAYER];		// カメラのポインタ	
	static CLight *m_pLight;					// ライトのポインタ
	static CMeshField *m_pMeshField;			// メッシュフィールドのポインタ
	static CMeshShape * m_pSphere;					// メッシュスフィアのポインタ
	static CBg *m_pBg;							// 背景のポインタ
	static CPlayer *m_apPlayer[MAX_PLAYER];		// プレイヤーのポインタ
	static CTime *m_pTime;						// タイムのポインタ
	static CUi *m_pUi;							// uiのポインタ
	static CUiStart *m_pUiStart;				// uistartのポインタ
	static CUiEnd *m_pUiEnd;					// uiendのポインタ
	static CLockon *m_pLockon;					// ロックオンのポインタ
	static CBill *m_pBill;						// ビルのポインタ
	static CSea *m_pSea;						// 海のポインタ
	static CContinue *m_pContinue;				// コンテニューのポインタ
	static CPause *m_pPause;					// ポーズのポインタ
	static int m_nRoundNum;						// 現在のラウンド数
	static int m_aWinNum[MAX_PLAYER];			// 勝利した数
	LPD3DXFONT m_pFont;
	bool m_bGameEnd;

};
#endif