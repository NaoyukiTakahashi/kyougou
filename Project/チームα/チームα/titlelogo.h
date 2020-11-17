//=============================================================================
//
// タイトルロゴの処理 [titlelogo.h]
// Author : 佐藤颯紀
//
//=============================================================================
#ifndef  _TITLELOGO_H_
#define  _TITLELOGO_H_

#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
//タイトルロゴ
#define TITLE_SIZE_X			(400.0f)	// タイトルのXサイズ
#define TITLE_SIZE_Y			(200.0f)		// タイトルのYサイズ
#define TITLE_POS_X				(640.0f)	// タイトルのX座標
#define TITLE_POS_Y				(350.0f)	// タイトルのY座標
//UI
#define TITLE_UI_SIZE			(700.0f)	// UIサイズ
#define TITLE_UI_POS_X			(640.0f)	// UIのX座標
#define TITLE_UI_POS_Y			(360.0f)	// UIのY座標
//PLESS
#define TITLE_PLESS_SIZE_X		(400.0f)	// PLESSのXサイズ
#define TITLE_PLESS_SIZE_Y		(30.0f)		// PLESSのYサイズ
#define TITLE_PLESS_POS_X		(640.0f)	// PLESSのX座標
#define TITLE_PLESS_POS_Y		(650.0f)	// PLESSのY座標
//種類
#define TITLELOGO_TYPE			(3)			// ロゴの種類の最大数

//=============================================================================
//クラス宣言
//=============================================================================
class CTitlelogo :public CScene2d
{
public:
	typedef enum
	{
		LOGOTYPE_NONE = -1,
		LOGOTIPE_TITLE,			// タイトル
		LOGOTIPE_PRESS,			// プレス文字
		LOGOTIPE_UI,			// UI
		LOGOTYPE_MAX
	}LOGOTYPE;

	CTitlelogo();
	~CTitlelogo();

	static CTitlelogo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, LOGOTYPE type);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, LOGOTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Flashing(void);
	void Rotation(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TITLELOGO_TYPE];

	D3DXVECTOR3	m_pos;		// ポリゴンの位置
	D3DXVECTOR3 m_move;		// ポリゴンの移動量
	D3DXVECTOR3 m_size;		// ポリゴンのサイズ
	D3DXCOLOR   m_col;		// カラー
	LOGOTYPE	m_type;		// ポリゴンのタイプ
	int m_nCounter;			// カウンター
	int m_nPattern;			// 使用しているかどうか

	float m_fRotasion; //回転速度
};
#endif // ! _TITLELOGO_H_
