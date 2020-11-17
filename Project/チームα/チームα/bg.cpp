//=================================================================================
//
// 背景クラス [bg.cpp]
// Author : Konishi Yuuto
//
//=================================================================================

//=================================================================================
// インクルード
//=================================================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"

//=================================================================================
// static初期化
//=================================================================================
LPDIRECT3DTEXTURE9 CBg::m_apTexture[MAX_BG_TEXTURE] = {};
LPD3DXMESH CBg::m_pMesh = NULL;
LPD3DXBUFFER CBg::m_pBuffMat = NULL;	//マテリアル情報へのポインタ
DWORD CBg::m_nNumMat = 0;	//マテリアル情報の数

//=================================================================================
// インスタンス生成
//=================================================================================
CBg * CBg::Create(void)
{
	CBg *pBg = new CBg;

	if (pBg != NULL)
	{
		// 初期化処理
		pBg->Init();
	}

	return pBg;
}

//=================================================================================
// テクスチャロード
//=================================================================================
HRESULT CBg::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/sky.jpg",
		&m_apTexture[0]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/dome2.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//=================================================================================
// テクスチャアンロード
//=================================================================================
void CBg::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BG_TEXTURE; nCount++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}

	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	//マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//=================================================================================
// コンストラクタ
//=================================================================================
CBg::CBg()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
}

//=================================================================================
// デストラクタ
//=================================================================================
CBg::~CBg()
{
}

//=================================================================================
// 初期化処理
//=================================================================================
HRESULT CBg::Init(void)
{
	MODEL model;

	model.dwNumMat = m_nNumMat;
	model.pBuffMat = m_pBuffMat;
	model.pMesh = m_pMesh;

	//モデル情報を設定
	CModel::BindModel(model);

	m_pos = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(BG_SIZE_X, BG_SIZE_Y, BG_SIZE_Z);

	// 座標情報を与える
	CModel::SetPos(m_pos);

	return S_OK;
}

//=================================================================================
// 終了処理
//=================================================================================
void CBg::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//=================================================================================
// 更新処理
//=================================================================================
void CBg::Update(void)
{
}

//=================================================================================
// 描画処理
//=================================================================================
void CBg::Draw(void)
{
	// Rendererクラスからデバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	
	D3DMATERIAL9 matDef;					// 現在のマテリアル保持用
	D3DXMATERIAL* pMat;						// マテリアルデータへのポインタ
	DWORD ambient;

	pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	pDevice->LightEnable(0, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//// スケールを反映
	D3DXMatrixScaling(&mtxScale,
		m_size.x,
		m_size.y,
		m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, m_apTexture[0]);

	// 現在のマテリアルを取得する
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Ambient.r = 1.0f;
		pMat[nCntMat].MatD3D.Ambient.g = 1.0f;
		pMat[nCntMat].MatD3D.Ambient.b = 1.0f;
		pMat[nCntMat].MatD3D.Ambient.a = 1.0f;

		// テクスチャの設定
		pDevice->SetTexture(0, m_apTexture[0]);

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// モデルパーツ
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
	pDevice->LightEnable(0, TRUE);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
}