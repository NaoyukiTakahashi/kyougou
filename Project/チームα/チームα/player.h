#ifndef _PLAYER_H_
#define _PLAYER_H_
//=============================================================================
//
// プレイヤークラスヘッダー [player.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "scene.h"
#include "modelanime.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define LIFE_NUM			(2)			// 表示するライフの数
#define WIN_LOSE_NUM		(2)
#define MAX_PARTS			(10)		// パーツの数
#define MOTION_KEYSET_MAX	(32)		// キーセット最大数

#define PLAYER2_POS_X		(0.0f)		// 座標
#define PLAYER2_POS_Y		(171.0f)	// 座標
#define PLAYER2_POS_Z		(500)		// 座標

#define PLAYER_SIZE_X		(1)			// サイズ
#define PLAYER_SIZE_Y		(1)			// サイズ
#define PLAYER_SIZE_Z		(1)			// サイズ

#define MAX_MODEL_PARTS			(21)		//モデルのパーツの最大数

#define PLAYER_COLLISION_X	(200)		// 当たり判定
#define PLAYER_COLLISION_Y	(175)		// 当たり判定
#define PLAYER_COLLISION_Z	(200)		// 当たり判定

#define WEAPON_COLLISION_X	(100)		// 武器の当たり判定
#define WEAPON_COLLISION_Y	(50)		// 武器の当たり判定
#define WEAPON_COLLISION_Z	(100)		// 武器の当たり判定

#define PLAYER_RADIUS		(150)		// 半径
#define PLAYER_BOMB			(80)		// ボム撃つのに必要なゲージ数
#define PLAYER_LASER		(35)		// レーザー撃つのに必要なゲージ数

#define MAX_BOOST			(2)

#define MAP_LIMIT			(6750)		// 行ける上限数
//=============================================================================
// 前方宣言
//=============================================================================
class CScore;
class CLife;
class CCharge;
class CBoost;
class CUi;

//=============================================================================
//　モーション状態の列挙型
//=============================================================================

typedef enum
{
	MOTION_NONE = -1,
	MOTION_IDOL,	//アイドルモーション
	MOTION_WALK,	//歩行モーション
	MOTION_ATTACK,	//攻撃モーション
	MOTION_JUMP,	//ジャンプモーション
	MOTION_LANDING,	//着地モーション
	MOTION_WIN,		//勝利モーション
	MOTION_RIGHTBOOST,	//右ブースト
	MOTION_LEFTBOOST,	//左ブースト
	MOTION_DAMAGE,	//やられ
	MOTION_BEAM,	//ビーム攻撃
	MOTION_LOSE,	//負けモーション
	MOTION_MAX,		//モーション最大数
}MOTION_STATE;

//=============================================================================
//　モデルファイル情報の構造体
//=============================================================================
typedef struct
{
	char xFileName[1024];	//ファイルネーム
	D3DXVECTOR3 offsetPos;	//位置のオフセット情報
	D3DXVECTOR3 offsetRot;	//向きのオフセット情報
	int nParent;	//親情報
}MODELFILLE;

//=============================================================================
//　各要素のキー情報
//=============================================================================
typedef struct
{
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;
}KEY;

//=============================================================================
//	キー情報の構造体
//=============================================================================
typedef struct
{
	int nFrame;	//フレーム数
	KEY aKey[MAX_MODEL_PARTS];	//各パーツのキー情報
}KEY_INFO;

//=============================================================================
//　モーション情報の構造体
//=============================================================================
typedef struct
{
	bool bLoop;	//ループするかどうか
	int nNumKey;	//キー数
	KEY_INFO aKeyInfo[20];	//キー情報
}Motion_Info;

//=============================================================================
// プレイヤークラス
//=============================================================================
class CPlayer : public CScene
{
public:

	typedef enum
	{
		PLAYER_STATE_NONE = 0,		// 初期置
		PLAYER_STATE_NORMAL,		// 通常状態
		PLAYER_STATE_DAMAGE,		// ダメージ
		PLAYER_STATE_EXPLOSION,		// 爆発
		PLAYER_STATE_DRAW,			// 引き分け
		PLAYER_STATE_MAX
	}PLAYER_STATE;

	CPlayer(int nPriority = PRIORITY_PLAYER);						// コンストラクタ
	~CPlayer();						// デストラクタ

	static CPlayer*Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// クリエイト

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void UpdateMotion(void);										//モーション更新処理
	void Draw(void);												// 描画処理
	void SetMotion(MOTION_STATE motion);

	void PlayerState(void);											// プレイヤーの状態
	void PlayerControl(void);										// プレイヤーの制御
	void Walk(void);												// プレイヤーの歩く処理
	void Jump(void);												// ジャンプの処理
	void GroundLimit(void);											// 地面の制限
	void Fall(void);												// 急降下
	void Dush(void);												// ダッシュ
	void Locus(void);												// 軌跡
	void beam(void);												// ビーム
	void Missile(void);												// ミサイル
	void bomb(void);												// ボム
	void MapLimit(void);
	void Laser(void);												// レーザー
	void BlockUp(void);												// ブロックの上に乗ったとき
	void SetPos(D3DXVECTOR3 pos);
	void SetWinToLose(bool bFlag);									// 勝ち負けロゴの設定
	void MotionState(void);											// モーション状態
	void WeaponCollision(void);										// 武器の当たり判定
	void TimeLimit(void);											// タイムリミット
	void SetState(PLAYER_STATE state);								// プレイヤー情報の設定
	bool GetSetWinLose(void);
	D3DXVECTOR3 GetPos(void);										// 現在の座標情報
	D3DXVECTOR3 GetOldPos(void);									// 古い座標情報
	D3DXVECTOR3 GetRot(void);										// 角度情報
	void SetMove(D3DXVECTOR3 move);										// 移動量の設定
	D3DXVECTOR3 GetMove(void);
	CLife *GetLife(int nNumber);									// ライフの情報
	CCharge *GetCgarge(void);										// チャージのポインタ
	HRESULT ReadFile(void);
	bool GetEnd(void);												// エンド情報
	PLAYER_STATE GetState(void);									// プレイヤーの状態
	bool GetJump(void);
	bool GetDraw(void);												// 引き分けのフラグ
	bool GetArmor(void);											// 無敵フラグの情報
	bool GetWin(void);												// 勝ちの情報
private:
	CScore *pScore;							// スコアの情報
	CLife *m_pLife[LIFE_NUM];				// ライフのポインタ
	CUi *m_pWinLose[WIN_LOSE_NUM];				// 勝敗のポインタ
	CUi *m_pDraw;								// 引き分けのポインタ
	CBoost *m_pBoost[MAX_BOOST];						// ブーストのポインタ
	CCharge *m_pCharge;						// チャージのポインタ
	D3DXVECTOR3 m_pos;						// 座標
	D3DXVECTOR3 m_OldPos;					// 1フレーム前の座標
	D3DXVECTOR3 m_rot;						// 回転(現在地)
	D3DXVECTOR3 m_rotDest;					// 回転(目標値)
	D3DXVECTOR3 m_move;						// 移動
	PLAYER_STATE m_state;					// プレイヤー状態
	float m_fAngle;							// 角度
	int m_nDushFlame;							// ダッシュのフレーム
	int m_nDushInterCnt;						// ダッシュできないときのカウント
	int m_nPlayerNum;							// プレイヤーの番号
	int m_nStateCounter;						// 状態カウンター
	int m_nNumKey;								//キーの総数
	int m_nKey;									//現在キーのNo
	int m_nCountMotion;							//モーションカウンター
	int m_nMotionInterval;						//モーションのインターバル
	int m_nBombInter;							// ボムのインターバル
	int m_nMissileInter;							// ミサイルのインターバル
	int m_nLaserInter;							// レーザーのインターバル
	bool m_bJump;								// ジャンプのフラグ
	bool m_bDush;								// ダッシュの処理
	bool m_bDushInter;							// ダッシュのインターバル
	bool m_bEnd;								// 終了フラグ
	bool m_bFall;								// 急降下フラグ
	bool m_bWalk;								//移動フラグ
	bool m_bWinLose;							// 勝ち負けのロゴフラグ
	bool m_bEntered;							//移動入力があるかどうか
	bool m_bMotionPlaing;
	bool m_bDraw;								// 引き分けフラグ
	bool m_bArmor;								// 無敵時間フラグ
	bool m_bAttack;								// 攻撃フラグ
	bool m_bWin;								// 勝利のフラグ
	static int m_nPlayerAll;					// プレイヤーの数
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	CModelAnime *m_apModelAnime[MAX_MODEL_PARTS];	//モデルパーツ用のポインタ
	KEY_INFO *m_apKeyInfo;						//キー情報のポインタ
	MOTION_STATE m_MotionState;					//モーションの状態
	Motion_Info m_Motion[MOTION_MAX];			//モーション情報

};
#endif