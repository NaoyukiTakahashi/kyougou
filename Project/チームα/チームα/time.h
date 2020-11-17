
//=============================================================================
//
// タイム処理 [time.h]
// Author : 佐藤颯紀
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CNumber;

//=============================================================================
// マクロ定義
//=============================================================================
#define TIME_LIMIT		(8000.0f)	// 制限時間	8000
#define MAX_TIME		(4)			// 桁数
#define TIME_S_SIZE_X	(50.0f)		// 数字のＸサイズ
#define TIME_S_SIZE_Y	(50.0f)		// 数字のＹサイズ
#define TIME_MS_SIZE_X	(35.0f)		// ミリ秒数字のＸサイズ
#define TIME_MS_SIZE_Y	(35.0f)		// ミリ秒数字のＹサイズ
#define TIME_POS_X		(585.0f)	// タイムのX座標
#define TIME_POS_Y		(55.0f)		// タイムのY座標
#define TIME_INTERVAL	(50.0f)		// 最初の2桁の数字の置く間隔
#define TIME_INTERVAL2	(45.0f)		// 小数点の数字の置く間隔
#define TIME_INTERVAL3	(40.0f)		// 小数点の数字の置く間隔
#define TIME_INTERVAL_Y	(10.0f)		// Y軸の数字の置く間隔

//================================================
//クラス宣言
//================================================
class CTime : public CScene
{
public:
	CTime(int nPriority = PRIORITY_0);
	~CTime();
	static CTime *Create(D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddTime(int nValue);
	void SetTime(int nTime) { m_nTime = nTime; }
	int GetTime(void) { return m_nTime; }

private:
	LPDIRECT3DTEXTURE9		m_pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3	m_pos;					// ポリゴンの位置
	CNumber	*m_apNumber[MAX_TIME];		// 数字の配置
	int m_nTime;						// タイム
	int m_nTimeCount;					// タイムのカウンター


};
#endif