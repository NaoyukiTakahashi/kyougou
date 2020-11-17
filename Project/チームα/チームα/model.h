//=============================================================================
//
// モデル処理 [model.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "scene.h"

//=============================================================================
//モデルクラス
//=============================================================================
class CModel : public CScene
{
public:
	typedef struct
	{
		LPD3DXMESH pMesh;			//メッシュ情報へのポインタ
		LPD3DXBUFFER pBuffMat;		//マテリアル情報へのポインタ
		DWORD dwNumMat;				//マテリアル情報の数
	}MODEL;

	typedef enum
	{
		MODEL_TYPE_NONE = 0,	// 初期値
		MODEL_TYPE_PLAYER1,		// プレイヤー
		MODEL_TYPE_PLAYER2,		// プレイヤー
		MODEL_TYPE_BEAM,		// ビーム
		MODEL_TYPE_OBJECT,		// 建物
		MODEL_TYPE_MAX
	}MODEL_TYPE;

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CModel(int nPriority = PRIORITY_0);
	~CModel();

	static CModel *Create(D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindModel(MODEL model);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void BindTexturePointer(LPDIRECT3DTEXTURE9 *ppTexture);
	LPD3DXMESH GetMesh(void)const;
	LPD3DXBUFFER GetBuffMat(void)const;	//

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void)const;

	void SetRot(const D3DXVECTOR3 rot);		// 座標の情報
	D3DXVECTOR3 GetRot(void);
	void SetType(MODEL_TYPE Mtype);
	void SetSize(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void);
	LPD3DXBUFFER GetBuffMat(void);
	MODEL_TYPE GetType(void);
	D3DXMATRIX GetMtxWorld(void);
private:
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================

	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DTEXTURE9 *m_apTexture;
	LPD3DXMESH m_pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;	// マテリアル情報へのポインタ
	DWORD m_nNumMat;			// マテリアル情報の数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_size;			// サイズ
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	MODEL_TYPE m_type;			// 種類
};
#endif 