//=================================================================================
//
// レーザークラス [beam.cpp]
// Author : Konishi Yuuto
//
//=================================================================================

//=================================================================================
// インクルード
//=================================================================================
#include "beam.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "shock.h"

//=================================================================================
// マクロ定義
//=================================================================================
#define BEAM_LIFE	(70)			// ビームのライフ

//=================================================================================
// static初期化
//=================================================================================
LPDIRECT3DTEXTURE9 CBeam::m_apTexture[MAX_BEAM_TEXTURE] = {};
LPD3DXMESH CBeam::m_pMesh = NULL;
LPD3DXBUFFER CBeam::m_pBuffMat = NULL;		//マテリアル情報へのポインタ
DWORD CBeam::m_nNumMat = 0;					//マテリアル情報の数

//=================================================================================
// インスタンス生成
//=================================================================================
CBeam * CBeam::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLET2_USER user)
{
	// メモリ確保
	CBeam *pBeam = new CBeam;

	if (pBeam != NULL)
	{
		// 初期化処理
		pBeam->Init(pos, move, size, user);		// 初期化情報
		pBeam->SetMove(move);					// 移動量
		pBeam->SetLife(BEAM_LIFE);				// ライフの情報
	}

	return pBeam;
}

//=================================================================================
// コンストラクタ
//=================================================================================
CBeam::CBeam()
{

}

//=================================================================================
// デストラクタ
//=================================================================================
CBeam::~CBeam()
{
}

//=================================================================================
// 初期化処理
//=================================================================================
HRESULT CBeam::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLET2_USER user)
{
	MODEL model;

	model.dwNumMat = m_nNumMat;
	model.pBuffMat = m_pBuffMat;
	model.pMesh = m_pMesh;

	//モデル情報を設定
	BindModel(model);
	BindTexture(m_apTexture[0]);

	// 初期化処理
	CBullet2::Init(pos, size, user, BEAM_SPEED);		// 初期化情報
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き

	return S_OK;
}

//=================================================================================
// 終了処理
//=================================================================================
void CBeam::Uninit(void)
{
	// 終了処理
	CBullet2::Uninit();
}

//=================================================================================
// 更新処理
//=================================================================================
void CBeam::Update(void)
{
	// 更新処理
	CBullet2::Update();

	D3DXVECTOR3 pos = GetPos();

	// エフェクト生成
	CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f), EFFECT_LIFE);
}

//=================================================================================
// 描画処理
//=================================================================================
void CBeam::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//マテリアルデータへのポインタ
	D3DXMATERIAL*pMat;

	LPD3DXBUFFER pBuffMat = GetBuffMat();

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	}

	// モデルの描画
	CBullet2::Draw();
}

//=================================================================================
// テクスチャロード
//=================================================================================
HRESULT CBeam::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/Explosion004.jpg",
		&m_apTexture[0]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/beam.x",
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
void CBeam::UnLoad(void)
{
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

	for (int nCount = 0; nCount < MAX_BEAM_TEXTURE; nCount++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}