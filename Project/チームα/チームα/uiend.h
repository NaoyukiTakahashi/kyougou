//=============================================================================
//
// 終了時のui処理 [uiend.h]
// Author : 佐藤颯紀
//
//=============================================================================

#ifndef _UIEND_H_
#define _UIEND_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene2d.h"

//=============================================================================
//マクロ定義
//=============================================================================
//continue
#define UI_CONTINUE_SIZE_X			(200.0f)	// CONTINUEのXサイズ
#define UI_CONTINUE_SIZE_Y			(30.0f)		// CONTINUEのYサイズ
#define UI_CONTINUE_POS_X			(640.0f)	// CONTINUEの右側のX軸
#define UI_CONTINUE_POS_Y			(240.0f)	// CONTINUEのY軸
//種類
#define UIEND_TYPE					(2)		// UIの種類の最大数
//=============================================================================
//クラス宣言
//=============================================================================
class CUiEnd : public CScene2d
{
public:
	typedef enum
	{
		UIENDTYPE_NONE = -1,
		UIENDTYPE_CONTINUE,
		UIENDTYPE_THANKS,
		UIENDTYPE_MAX
	}UIENDTYPE;

	CUiEnd();
	~CUiEnd();

	static CUiEnd *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UIENDTYPE type);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, UIENDTYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[UIEND_TYPE];

	D3DXVECTOR3		m_pos;		// ポリゴンの位置
	D3DXVECTOR3		m_move;		// ポリゴンの移動量
	D3DXVECTOR3		m_size;		// ポリゴンのサイズ
	D3DXCOLOR		m_col;		// カラー
	UIENDTYPE		m_type;		// ポリゴンのタイプ
	int m_nCount;				// フレームカウント

};
#endif // !_UI_H_