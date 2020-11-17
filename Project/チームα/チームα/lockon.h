//=============================================================================
//
// ロックオン処理 [lockon.h]
// Author : 佐藤颯紀
//
//=============================================================================

#ifndef  _LOCKON_H_
#define  _LOCKON_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define UI_LOCKON_SIZE_X		(70.0f)		// ロックオンのXサイズ
#define UI_LOCKON_SIZE_Y		(70.0f)		// ロックオンのYサイズ

//=============================================================================
//クラス宣言
//=============================================================================
class CLockon :public CScene2d
{
public:
	//１Ｐと２Ｐのタイプ分け
	typedef enum
	{
		LOCKONTYPE_NONE = -1,
		LOCKONTYPE_FAST_PLAYER,		//１Ｐ側表示
		LOCKONTYPE_SECOND_PLAYER,	//２Ｐ相手側表示
		LOCKONTYPE_PLAYER_MAX
	}LOCKONTYPE;

	CLockon();
	~CLockon();

	static CLockon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, LOCKONTYPE type);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, LOCKONTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3	m_pos;		// ポリゴンの位置
	D3DXVECTOR3 m_move;		// ポリゴンの移動量
	D3DXVECTOR3 m_size;		// ポリゴンのサイズ
	D3DXCOLOR   m_col;		// カラー
	LOCKONTYPE m_type;
	int m_nLockCounter;		// ロックオンカウンター
	int m_nLockPattern;		// ロックオンのパターン
	static int m_nLockon;

};
#endif // ! _LOCKON_H_

