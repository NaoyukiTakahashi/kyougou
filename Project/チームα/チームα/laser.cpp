//=================================================================================
//
// レーザークラス [laser.cpp]
// Author : Konishi Yuuto
//
//=================================================================================

//=================================================================================
// インクルード
//=================================================================================
#include "laser.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "shock.h"

//=================================================================================
// マクロ定義
//=================================================================================
#define LASER_LIFE			(100)			// ビームのライフ
#define LASER_FLASH_NUM		(0.1f)			// 点滅の値
#define LASER_SCALE_NUM		(0.1f)			// 拡縮の値
#define LASER_SCALE_LIMIT	(5.0f)			// 拡大の最大値

//=================================================================================
// static初期化
//=================================================================================
LPDIRECT3DTEXTURE9 CLaser::m_apTexture[MAX_LASER_TEXTURE] = {};
LPD3DXMESH CLaser::m_pMesh = NULL;
LPD3DXBUFFER CLaser::m_pBuffMat = NULL;		//マテリアル情報へのポインタ
DWORD CLaser::m_nNumMat = 0;				//マテリアル情報の数

//=================================================================================
// インスタンス生成
//=================================================================================
CLaser * CLaser::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR3 size, BULLET2_USER user)
{
	// メモリ確保
	CLaser *pLaser = new CLaser;

	if (pLaser != NULL)
	{
		// 初期化処理
		pLaser->Init(pos, move, size, user);	// 初期化情報
		pLaser->SetMove(move);					// 移動量
		pLaser->SetLife(LASER_LIFE);			// ライフの情報
		pLaser->SetRot(rot);					// 角度
		pLaser->SetRot(D3DXVECTOR3(rot.x, rot.y + D3DXToRadian(180), rot.z));		// 角度
	}

	return pLaser;
}

//=================================================================================
// コンストラクタ
//=================================================================================
CLaser::CLaser()
{
	m_fFlash = 0.5f;
	m_fFlashNum = LASER_FLASH_NUM;
	m_fScale = 1.0f;					// 拡大用
	m_fScaleNum = LASER_SCALE_NUM;		// 拡大用の値
	m_fSizeZ = 0.0f;
}

//=================================================================================
// デストラクタ
//=================================================================================
CLaser::~CLaser()
{
}

//=================================================================================
// 初期化処理
//=================================================================================
HRESULT CLaser::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLET2_USER user)
{
	MODEL model;

	model.dwNumMat = m_nNumMat;
	model.pBuffMat = m_pBuffMat;
	model.pMesh = m_pMesh;

	//モデル情報を設定
	BindModel(model);
	BindTexture(m_apTexture[0]);

	// 初期化処理
	CBullet2::Init(pos, size, user, LASER_SPEED);		// 初期化情報
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// 向き
	m_fSizeZ = size.z;									// サイズの取得
	SetType(BULLET2_TYPE_LASER);							// タイプの設定

	return S_OK;
}

//=================================================================================
// 終了処理
//=================================================================================
void CLaser::Uninit(void)
{
	// 終了処理
	CBullet2::Uninit();
}

//=================================================================================
// 更新処理
//=================================================================================
void CLaser::Update(void)
{
	// 更新処理
	CBullet2::Update();

	D3DXVECTOR3 pos = GetPos();

	// 拡縮
	Scale();

	// エフェクト生成
	CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(EFFECT_LASER_SIZE_X, EFFECT_LASER_SIZE_X, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), EFFECT_LASER_LIFE);

}

//=================================================================================
// 描画処理
//=================================================================================
void CLaser::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_fFlash += m_fFlashNum;

	if (m_fFlash <= 0.5f)
	{
		m_fFlashNum *= -1;
	}
	
	if (m_fFlash >= 1.0f)
	{
		m_fFlashNum *= -1;
	}

	//マテリアルデータへのポインタ
	D3DXMATERIAL*pMat;

	LPD3DXBUFFER pBuffMat = GetBuffMat();

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(m_fFlash, m_fFlash, m_fFlash, 1.0f);
	}

	// モデルの描画
	CBullet2::Draw();
}

//=================================================================================
// 拡縮処理
//=================================================================================
void CLaser::Scale(void)
{
	// 拡縮の値加算
	m_fScale += m_fScaleNum;

	if (m_fScale <= LASER_SCALE_LIMIT)
	{
		// サイズ取得
		D3DXVECTOR3 size = GetSize();

		// サイズの設定
		SetSize(D3DXVECTOR3(size.x, size.y, m_fSizeZ *m_fScale));
	}
	else
	{
		// サイズ取得
		D3DXVECTOR3 size = D3DXVECTOR3(GetSize());

		// サイズの設定
		SetSize(D3DXVECTOR3(size));

	}
}

//=================================================================================
// テクスチャロード
//=================================================================================
HRESULT CLaser::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/razer.png",
		&m_apTexture[0]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/razer.x",
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
void CLaser::UnLoad(void)
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

	for (int nCount = 0; nCount < MAX_LASER_TEXTURE; nCount++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}