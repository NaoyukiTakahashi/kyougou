//=============================================================================
// 
// 弾のチャージゲージの処理(charge.h)
// Author : 佐藤颯紀
//
//=============================================================================

#ifndef _CHARGE_H_
#define _CHARGE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "gauge.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_CHARGE			(400)		// ゲージの最大数(ゲージのXサイズ)
#define HALF_CHARGE			(200)		// ゲージの半分
#define CHARGE_POS_LEFT_X	(120.0f)	// 左側のゲージのX座標
#define CHARGE_POS_RIGHT_X	(760.0f)	// 右側のゲージのX座標
#define CHARGE_POS_Y		(680.0f)	// ゲージのY座標
#define CHARGE_SIZE_Y		(15)		// ゲージのYサイズ
#define CHARGE_LIMIT		(2)
#define CHARGE_PLAYER		(2)
//=============================================================================
//クラス宣言
//=============================================================================
class CCharge :public CGauge
{
public:
	CCharge();
	~CCharge();

	static CCharge* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Reduce(int Counter, bool Charge, int player);
	void GaugeRedce(void);
	void GaugeReturn(void);
	void GaugeMax(void);
	static float GetCharge(int limit);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_size;				// サイズ
	D3DXCOLOR   m_col;						// カラー
	int m_nCounterCharge;					// 減らす分のカウンター
	int m_nReduce;							// いくつ減らすか
	int m_nMaxCounter;						// ゲージ最大のときのカウンター
	int m_nPlayerNum;						// プレイヤーの番号
	bool m_bCharge;							// 減らす処理を使用するかしないか
	float m_nBlue;							// 青色の設定
	float m_nRed;							// 赤色の設定
	static float m_fLimit[CHARGE_LIMIT];	// 弾の制御
};

#endif // !_CHARGE_H_
