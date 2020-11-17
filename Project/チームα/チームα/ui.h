//=============================================================================
//
// ui処理 [ui.h]
// Author : 佐藤颯紀
//
//=============================================================================

#ifndef _UI_H_
#define _UI_H_

#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
//ライフ
#define UI_LIFE_SIZE_X				(405.0f)	// ライフUIのXサイズ
#define UI_LIFE_SIZE_PLAYERY		(22.0f)		// プレイヤーライフUIのYサイズ
#define UI_LIFE_SIZE_ENEMYY			(11.2)		// エネミーライフUIのYサイズ
//タイム
#define UI_TIME_SIZE_X				(85.0f)		// タイムUIのXサイズ
#define UI_TIME_SIZE_Y				(40.0f)		// タイムUIのYサイズ
//プレイヤー・エネミーの文字
#define UI_PLAYER_SIZE_X			(50.0f)		// プレイヤーUIのXサイズ
#define UI_PLAYER_SIZE_Y			(15.0f)		// プレイヤーUIのYサイズ
#define UI_ENEMY_SIZE_X				(45.0f)		// エネミーUIのXサイズ
#define UI_ENEMY_SIZE_Y				(10.0f)		// エネミーUIのYサイズ
//HIT
#define UI_HIT_SIZE_X				(100.0f)	// ヒットUIのXサイズ
#define UI_HIT_SIZE_Y				(50.0f)		// ヒットUIのYサイズ
#define UI_HIT_POS_LEFT_X			(320.0f)	// ヒットUIの左側のX軸
#define UI_HIT_POS_RIGHT_X			(960.0f)	// ヒットUIの右側のX軸
#define UI_HIT_POS_Y				(600.0f)	// ヒットUIのYサイズ
//WINとLOSE
#define UI_RESULT_SIZE_X			(200.0f)	// 勝敗文字のXサイズ
#define UI_RESULT_SIZE_Y			(30.0f)		// 勝敗文字のYサイズ
#define UI_RESULT_POS_LEFT_X		(320.0f)	// 勝敗文字出すX軸（左側）
#define UI_RESULT_POS_RIGHT_X		(960.0f)	// 勝敗文字出すX軸（右側）
#define UI_RESULT_POS_Y				(360.0f)	// 勝敗文字出す場所のY軸
//標準
#define UI_LOCKON_SIZE_SMALL_X		(40.0f)		// ロックオンのXサイズ
#define UI_LOCKON_SIZE_SMALL_Y		(40.0f)		// ロックオンのYサイズ
#define UI_LOCKON_POS_LEFT_X		(320.0f)	// ロックオンの左側のX軸
#define UI_LOCKON_POS_RIGHT_X		(960.0f)	// ロックオンの右側のX軸
#define UI_LOCKON_POS_Y				(360.0f)	// ロックオンのY軸

//WINMark
#define UI_WINMARK_SIZE_X			(15.0f)		// WINMarkのXサイズ
#define UI_WINMARK_SIZE_Y			(15.0f)		// WINMarkのXサイズ
#define UI_WINMARK_POS_LEFT1_X		(50.0f)
#define UI_WINMARK_POS_LEFT2_X		(85.0f)
#define UI_WINMARK_POS_RIGHT1_X		(790.0f)
#define UI_WINMARK_POS_RIGHT2_X		(825.0f)
#define UI_WINMARK_POS_Y			(100.0f)

//種類
#define UI_TYPE					(12)		// UIの種類の最大数

//=============================================================================
//クラス宣言
//=============================================================================
class CUi :public CScene2d
{
public:
	typedef enum
	{
		UITYPE_NONE = -1,
		UITTYPE_LIFE,			// ライフバー
		UITYPE_TIME,			// タイムバー
		UITYPE_PLAYER,			// プレイヤー文字
		UITYPE_ENEMY,			// エネミー文字
		UITYPE_HIT,				// ヒット文字
		UITYPE_WIN,				// WIN文字
		UITYPE_LOSE,			// LOSE文字
		UITYPE_STANDARD,		// 標準表示
		UYTYPE_WINMARKFRAME,	// 勝ちの枠
		UYTYPE_WINMARK,			// 勝ったマーク
		UYTYPE_CONTINUE,	// CONTINUE文字
		UITYPE_DRAW,		// 引き分け
		UITYPE_MAX
	}UITYPE;

	CUi();
	~CUi();

	static CUi *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UITYPE type);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, UITYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UiHit(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[UI_TYPE];
	D3DXVECTOR3	m_pos;		// ポリゴンの位置
	D3DXVECTOR3 m_move;		// ポリゴンの移動量
	D3DXVECTOR3 m_size;		// ポリゴンのサイズ
	D3DXCOLOR   m_col;		// カラー
	UITYPE		m_type;		// ポリゴンのタイプ
	int m_nCounter;			// カウンター
	int m_nPattern;			// 使用しているかどうか
	static int m_nUi;
	
};

#endif // !_UI_H_
