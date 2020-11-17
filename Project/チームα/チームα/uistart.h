//=============================================================================
//
// ui処理 [ui.h]
// Author : 佐藤颯紀
//
//=============================================================================

#ifndef _UISTART_H_
#define _UISTART_H_

#include "ui.h"

//=============================================================================
//マクロ定義
//=============================================================================
//ready
#define UI_READY_SIZE_X			(200.0f)	// READYのXサイズ
#define UI_READY_SIZE_Y			(30.0f)		// READYのYサイズ
#define UI_READY_POS_LEFT_X		(320.0f)	// READYの左側のX軸
#define UI_READY_POS_RIGHT_X	(960.0f)	// READYの右側のX軸
#define UI_READY_POS_Y			(240.0f)	// READYのY軸
//round
#define UI_ROUND_SIZE_X			(150.0f)	// READYのXサイズ
#define UI_ROUND_SIZE_Y			(20.0f)		// READYのYサイズ
#define UI_ROUND_POS_LEFT_X		(280.0f)	// READYの左側のX軸
#define UI_ROUND_POS_RIGHT_X	(920.0f)	// READYの右側のX軸
#define UI_ROUND_POS_Y			(180.0f)	// READYのY軸
//種類
#define UISTART_TYPE					(6)		// UIの種類の最大数
//=============================================================================
//クラス宣言
//=============================================================================
class CUiStart : public CScene2d
{
public:
	typedef enum 
	{
		UISTARTTYPE_NONE = -1,
		UISTARTTYPE_READY,
		UISTARTTYPE_ROUND_1,
		UISTARTTYPE_ROUND_2,
		UISTARTTYPE_ROUND_3,
		UISTARTTYPE_MAX
	}UISTARTTYPE;

	CUiStart();
	~CUiStart();

	static CUiStart *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UISTARTTYPE type);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, UISTARTTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[UISTART_TYPE];

	D3DXVECTOR3	m_pos;		// ポリゴンの位置
	D3DXVECTOR3 m_move;		// ポリゴンの移動量
	D3DXVECTOR3 m_size;		// ポリゴンのサイズ
	D3DXCOLOR   m_col;		// カラー
	UISTARTTYPE		m_type;		// ポリゴンのタイプ
};
#endif // !_UI_H_