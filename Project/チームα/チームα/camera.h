//=============================================================================
//
// メイン処理 [camera.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CLockon;

//===========================================================
//カメラクラス
//=============================================================================
class CCamera
{
public:
	typedef enum
	{
		CAMERASTATE_NONE = 0,
		CAMERASTATE_NORMAL,	//通常
		CAMERASTATE_END,	//終了
		CAMERASTATE_MAX	//STATEの最大数
	}CAMERASTATE;
	//=========================================================================
	//パブリックメンバ関数宣言
	//=========================================================================
	CCamera();
	~CCamera();
	static CCamera*Create(void);// クリエイト
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	bool GetTargetBool(void);
	float Getθ(void);
	float Getφ(void);
	D3DXVECTOR3 GetposV(void);
	D3DXVECTOR3 GetposR(void);
	D3DXMATRIX GetMtxView(void);
	void SetTarget(bool Target);
private:
	//=========================================================================
	//プライベートメンバ関数宣言
	//=========================================================================
	void EndUpdate(D3DXVECTOR3 PlayerPos[], D3DXVECTOR3 PlayerRot[], int nWinPlayer);
	void NomalUpdate(D3DXVECTOR3 PlayerPos[], D3DXVECTOR3 PlayerRot[]);
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	D3DXVECTOR3 m_posV;						//カメラの座標
	D3DXVECTOR3 m_posVDest;					//カメラの座標（目的地）
	D3DXVECTOR3 m_posVRot;					//カメラの座標（）
	D3DXVECTOR3 m_posR;						//注視点
	D3DXVECTOR3 m_posRDest;					//注視点（目的地）
	D3DXVECTOR3 m_posRRot;					//注視点（）
	D3DXVECTOR3 m_posU;						//上方向ベクトル
	D3DXMATRIX m_mtxProjection;				//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;					//ビューマトリックス
	D3DXVECTOR3 m_rot;						//向き
	bool m_bTarget;							//ターゲット使用フラグ
	float m_fθ;							//縦回転角度
	float m_fφ;							//横回転角
	int m_nCameraNum;						//プレイヤーの番号
	float m_fDistance;						//視点～注視点の距離
	float m_fMove;							//移動量
	CAMERASTATE m_state;					//ステータス
	static int m_nCameraAll;				//カメラの数
	CLockon *m_pLockon[2];
	int m_nWinPlayer;						//Winプレイヤー番号
	float m_fDifference;					//勝利カメラ回転開始角から終了角の差
	float m_fEndVertical;					//勝利カメラ終了角度
};
#endif 