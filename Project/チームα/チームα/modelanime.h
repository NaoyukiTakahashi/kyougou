//=============================================================================
//
// 階層構造用のモデル処理 [modelanime.cpp]
// Author : 山田陵太
//
//=============================================================================
#ifndef _MODELANIME_H_
#define _MODELANIME_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//階層モデルクラス
//=============================================================================
class CModelAnime
{
public:
	//=========================================================================
	//モデル情報音構造体
	//=========================================================================
	typedef struct
	{
		LPD3DXMESH pMesh;			//メッシュ情報へのポインタ
		LPD3DXBUFFER pBuffMat;		//マテリアル情報へのポインタ
		DWORD dwNumMat;				//マテリアル情報の数
	}MODEL;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CModelAnime();
	~CModelAnime();

	static CModelAnime *Create(char *xfilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	HRESULT Init(char *xfilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);;
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetParent(CModelAnime *pParent);
	void SetPosAnime(const D3DXVECTOR3 posAnime);
	D3DXVECTOR3 GetPos(void)const;
	D3DXVECTOR3 GetPosAnime(void)const;
	void SetRotAnime(const D3DXVECTOR3 rotAnime);
	D3DXVECTOR3 GetRot(void)const;
	D3DXVECTOR3 GetRotAnime(void)const;
	D3DXMATRIX GetMtxWorld(void);
	D3DXMATRIX GetOldMtxWorld(void);

private:
	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	MODEL m_model;	//モデル情報
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	D3DXVECTOR3 m_posAnime;	//アニメーション用の位置
	D3DXVECTOR3 m_rotAnime;	//アニメーション用の向き
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	CModelAnime *m_pParent;	//親情報のポインタ
	D3DXMATRIX m_OldMtxWorld;
	D3DXMATRIX m_OldMtxWorld1[5];
};

#endif 