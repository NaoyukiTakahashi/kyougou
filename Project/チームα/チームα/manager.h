//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : 
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//前方宣言
//=============================================================================
class CRenderer;
class CInputKeyboard;
class CConection;
class CFade;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CInputJoypad;
class CSound;
class CScene;
//=============================================================================
//マネージャークラス
//=============================================================================
class CManager
{
public:
	//=========================================================================
	//列挙型定義
	//=========================================================================
	typedef enum
	{
		MODE_TYPE_NONE = 0, 
		MODE_TYPE_TITLE,	//タイトルモード
		MODE_TYPE_TUTORIAL,		//チュートリアル
		MODE_TYPE_GAME,		//ゲームモード
		MODE_TYPE_RESULT,	//リザルトモード
		MODE_TYPE_MAX,
	}MODE_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadAll(void);
	void UnLoadAll(void);

	static void SetMode(MODE_TYPE mode);
	static MODE_TYPE GetMode(void);

	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetKeyboard(void);
	static CConection *GetConection(void);
	static CFade *GetFade(void);
	static CInputJoypad *GetJoypad(void);
	static CSound *GetSound(void);
	static CScene *GetScene(void) { return m_pScene; }

private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static MODE_TYPE m_mode;
	static CRenderer *m_pRenderer;	//レンダラークラスのポインタ
	static CInputKeyboard *m_pInput;
	static CConection *m_pConection;
	static CFade *m_pFade;
	static CTitle *m_pTitle;
	static CTutorial *m_pTutorial;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CInputJoypad *m_pJoypad;
	static CSound *m_pSound;
	static CScene *m_pScene;
};

#endif