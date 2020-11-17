//=============================================================================
// 
// ライフゲージ処理(life.h)
// Author : 佐藤颯紀
//
//=============================================================================

#ifndef  _LIFE_H_
#define  _LIFE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "gauge.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_LIFE			(400)		// ライフの最大数(ライフのXサイズ)
#define HALF_LIFE			(200)		// ライフの半分
#define LIFE_SIZE_PLAYER_Y	(20)		// プレイヤーライフのYサイズ
#define LIFE_SIZE_ENEMY_Y	(10)		// エネミーライフのYサイズ
#define LIFE_POS_LEFT_X		(130.0f)	// 左側画面のライフのX座標
#define LIFE_POS_RIGHT_X	(860.0f)	// 右側画面のライフのX座標
#define LIFE_POS_UP_Y		(30.0f)		// 上側ライフのY座標
#define LIFE_POS_DOWN_Y		(65.0f)		// 下側ライフのY座標
#define LIFE_TYPE			(2)			// ライフの種類

//=============================================================================
//クラス宣言
//=============================================================================
class CLife :public CGauge
{
public:
	//１Ｐと２Ｐライフのタイプ分け
	typedef enum
	{
		LIFETYPE_NONE = -1,
		LIFETYPE_FAST_PLAYER,		//１Ｐ側表示
		LIFETYPE_SECOND_PLAYER,		//２Ｐ相手側表示
		LIFETYPE_PLAYER_MAX
	}LIFETYPE;

	CLife();
	~CLife();
	static CLife* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, const LIFETYPE type);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, const LIFETYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Decrease(int Reduce,int PlayerNamber,bool Life);
	void Lifereduce(void);
	void LifeFlashing(void);
	void LifeStart(void);
	static void SetReady(bool bReady);
	static bool GetReadey(void);
	float GetLife(void);
	bool GetbLife(void);
	//bool GetLife(void);
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[LIFE_TYPE];	// テクスチャーのポインタ
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_size;									// サイズ
	D3DXCOLOR   m_col;									// カラー
	LIFETYPE    m_type;									// ライフの種類
	LIFETYPE	m_typeDecrease;							// ダメージの受けた方の種類
	int m_nCounter;										// カウンター
	int m_nCounterLife;									// 減らす分のカウンター
	int m_nReduce;										// いくつ減らすか
	int m_nPlayerNum;									// １Ｐか２Ｐか
	bool m_bLife;										// 減らす処理を使用するかしないか
	static bool m_bStart;								// スタートの処理を使用するかしないか
	bool m_bEnd;
	float m_fLife;										// ライフ
	bool m_bArmor;
};
#endif // ! _LIFE_H_
