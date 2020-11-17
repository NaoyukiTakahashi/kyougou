#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//=============================================================================
//
// 火花クラスヘッダー [particl.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "billboard.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SPARK_SIZE_X			(20)		// 横サイズ
#define SPARK_SIZE_Y			(5)			// 縦サイズ
#define MAX_SPARK_TEXTURE		(1)			// テクスチャ数
#define SPARK_LIFE				(14)		// スパークの体力
#define SPARK_BASE_ANGLE		(90)		// 火花の向き
#define SPARK_NUM				(50)		// スパークの数
#define SPARK_SPPED				(1)			// スピード
#define SPARK_DISTANCE			(20)		// 発生距離
#define SPARK_ANGLE				(30)		// 角度

//=============================================================================
// 前方宣言
//=============================================================================
class CBillboard;

//=============================================================================
// BGクラス
//=============================================================================
class CParticle : public CBillboard
{
public:
	CParticle();			// コンストラクタ
	~CParticle();			// デストラクタ

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	static CParticle * Create(										// ポリゴン生成
		D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col,
		int nAngle, int nBaseAngle, 
		int nDistance, int nLife, int nSpeed);

	static HRESULT Load(void);										// テクスチャロード
	static void UnLoad(void);										// テクスチャアンロード

	void SetColor(D3DXCOLOR col);									// パーティクルのカラーを設定
	void SetLife(int nLife);										// ライフの設定
private:
	int m_nLife;												// 体力
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_SPARK_TEXTURE];	// テクスチャ情報のポインタ
};

#endif