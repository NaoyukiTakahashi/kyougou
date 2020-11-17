//=============================================================================
//
// ミサイル処理 [missile.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _MISSILE_H_
#define _MISSILE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_MODEL_MISSILE_PARTS 2
#define MISSILE_SPPED			(45)

#define MISSILE_SIZE_X			(3.0f)
#define MISSILE_SIZE_Y			(3.0f)
#define MISSILE_SIZE_Z			(3.0f)

//=============================================================================
//前方宣言
//=============================================================================
class CModelAnime;
class CPlayer;

//=============================================================================
//ミサイルクラス
//=============================================================================
class CMissile : public CScene
{
public:
	//=============================================================================
	//　モデルファイル情報の構造体
	//=============================================================================
	typedef struct
	{
		char xFileName[1024];	//ファイルネーム
		D3DXVECTOR3 offsetPos;	//位置のオフセット情報
		D3DXVECTOR3 offsetRot;	//向きのオフセット情報
		int nParent;	//親情報
	}MODELFILLEs;

	//=========================================================================
	//弾の使用者判別用の列挙型
	//=========================================================================
	typedef enum
	{
		MISSILE_USER_NONE = -1,
		MISSILE_USER_PL1,	//PL1
		MISSILE_USER_PL2,	//PL2
		MISSILE_USER_MAX		//最大数
	}MISSILE_USER;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CMissile(int nPriority = PRIORITY_0);
	~CMissile();

	static CMissile *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,
		const MISSILE_USER user, float fSpeed);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetRot(D3DXVECTOR3 rot);
	void SetMove(D3DXVECTOR3 move);
	HRESULT ReadFile(void);
	bool Collision(void);									// 当たり判定
	D3DXVECTOR3 VectorMath(D3DXVECTOR3 TargetPos,			// 追従
		float fSpeed);

private:
	D3DXVECTOR3 m_pos;										// 位置
	D3DXVECTOR3 m_rot;										// 向き
	D3DXVECTOR3 m_move;										// 移動量
	D3DXVECTOR3 m_size;										// サイズ
	int m_nLife;											// ライフ
	CModelAnime *m_apModelAnime[MAX_MODEL_MISSILE_PARTS];	// 階層モデルクラスのポインタ変数
	int m_nCounter;											// 追従カウンター
	D3DXMATRIX m_mtxWorld;									// ワールドマトリックス
	MISSILE_USER m_user;	// 使用者
	CPlayer *m_pTargetPL;	//敵プレイヤーのポインタ
	int m_nDamage;			//ダメージ数
	float m_fSpeed;			// 速さ
	float m_fHeight;		// 高さ

};

#endif 