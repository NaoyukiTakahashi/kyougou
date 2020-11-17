//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 山田陵太
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"
#include "life.h"
#include "2d_explosion.h"

//=============================================================================
//モデルクラスのコンストラクタ
//=============================================================================
CModel::CModel(int nPriority) : CScene(nPriority)
{
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_nNumMat = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_type = MODEL_TYPE_NONE;
	m_pTexture = NULL;
	//memset(m_apTexture, 0, sizeof(m_apTexture));
	m_apTexture = NULL;
}

//=============================================================================
//モデルクラスのデストラクタ
//=============================================================================
CModel::~CModel()
{
}

//=============================================================================
//モデルクラスのクリエイト処理
//=============================================================================
CModel * CModel::Create(D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//モデルクラスのポインタ変数
	CModel *pModel = NULL;

	//メモリ確保
	pModel = new CModel;

	//メモリが確保できていたら
	if (pModel != NULL)
	{
		//初期化処理呼び出し
		pModel->Init(pos, size);
	}
	//メモリ確保に失敗したとき
	else
	{
		return NULL;
	}

	return pModel;
}

//=============================================================================
//モデルクラスの初期化処理
//=============================================================================
HRESULT CModel::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 位置の初期化
	m_pos = pos;

	// サイズ初期化
	m_size = size;

	return S_OK;
}

//=============================================================================
//モデルクラスの終了処理
//=============================================================================
void CModel::Uninit(void)
{
	////メッシュの破棄
	//if (m_pMesh != NULL)
	//{
	//	m_pMesh->Release();
	//	m_pMesh = NULL;
	//}
	////マテリアルの破棄
	//if (m_pBuffMat != NULL)
	//{
	//	m_pBuffMat->Release();
	//	m_pBuffMat = NULL;
	//}

	//オブジェクトの破棄
	SetDeathFlag();
}

//=============================================================================
// モデルクラスの更新処理
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
// モデルクラスの描画処理
//=============================================================================
void CModel::Draw(void)
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
	D3DMATERIAL9 matDef;	//現在のマテリアル保持用
	D3DXMATERIAL*pMat;		//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得する
	pDevice->GetMaterial(&matDef);

	

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルのアンビエントにディフューズカラーを設定
		pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//建物オブジェクトの場合
		if (m_type == MODEL_TYPE_OBJECT)
		{
			pDevice->SetTexture(0, m_apTexture[nCntMat]);
		}
		else
		{
			pDevice->SetTexture(0, m_pTexture);
		}
		

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);

		if (m_type == MODEL_TYPE_OBJECT)
		{
			pDevice->SetTexture(0, NULL);
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}
		// 透明度戻す
		pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
	}

	//保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

}

void CModel::BindModel(MODEL model)
{
	m_pMesh = model.pMesh;
	m_pBuffMat = model.pBuffMat;
	m_nNumMat = model.dwNumMat;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CModel::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

void CModel::BindTexturePointer(LPDIRECT3DTEXTURE9 *ppTexture)
{
	m_apTexture = ppTexture;
}

//=============================================================================
//モデルクラスのメッシュ情報の取得
//=============================================================================
LPD3DXMESH CModel::GetMesh(void) const
{
	return m_pMesh;
}

LPD3DXBUFFER CModel::GetBuffMat(void) const
{
	return m_pBuffMat;
}

//=============================================================================
//モデルクラスの位置情報の設定
//=============================================================================
void CModel::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
//モデルクラスの位置情報の取得
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void) const
{
	return m_pos;
}

//=============================================================================
//モデルクラスの向きの設定
//=============================================================================
void CModel::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 角度の情報
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void)
{
	return m_rot;
}

//=============================================================================
//モデルの種類
//=============================================================================
void CModel::SetType(MODEL_TYPE Mtype)
{
	m_type = Mtype;
}

//=============================================================================
// サイズの設定
//=============================================================================
void CModel::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// サイズの情報
//=============================================================================
D3DXVECTOR3 CModel::GetSize(void)
{
	return m_size;
}

LPD3DXBUFFER CModel::GetBuffMat(void)
{
	return m_pBuffMat;
}

CModel::MODEL_TYPE CModel::GetType(void)
{
	return m_type;
}

D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_mtxWorld;
}
