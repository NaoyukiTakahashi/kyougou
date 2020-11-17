//=================================================================================
//
// 爆発クラス [explosion.cpp]
// Author : Konishi Yuuto
//
//=================================================================================

//=================================================================================
// インクルード
//=================================================================================
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"

//=================================================================================
// マクロ定義
//=================================================================================
#define EXPLOSION_LIFE			(100)		// 体力
#define EXPLOSION_SCALE_NUM		(0.05f)		// スケールの値
#define EXPLOSION_SCALE_LIMIT	(5.0f)		// スケールの最大量
#define EXPLOSION_ADD_ROT		(0.1f)		// 角度の加算量
#define EXPLOSION_SUB_ALPHA		(0.005f)	// 透明度の減算量

//=================================================================================
// static初期化
//=================================================================================
LPDIRECT3DTEXTURE9 CExplosion::m_apTexture[MAX_EXPLOSION_TEXTURE] = {};
LPD3DXMESH CExplosion::m_pMesh = NULL;
LPD3DXBUFFER CExplosion::m_pBuffMat = NULL;			//マテリアル情報へのポインタ
DWORD CExplosion::m_nNumMat = 0;					//マテリアル情報の数

//=================================================================================
// インスタンス生成
//=================================================================================
CExplosion * CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size)
{
	// メモリ確保
	CExplosion *pExplosion = new CExplosion;

	if (pExplosion != NULL)
	{
		// 初期化処理
		pExplosion->Init(pos, move, size);		// 初期化情報
	}

	return pExplosion;
}

//=================================================================================
// コンストラクタ
//=================================================================================
CExplosion::CExplosion()
{
	m_fScale = 0.0f;		// 拡縮用
	m_fScaleNum = 0.0f;		// 拡縮用の値
	m_FirstSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAddRotNum = 0.0f;
	m_fAlphaNum = 0.0f;
}

//=================================================================================
// デストラクタ
//=================================================================================
CExplosion::~CExplosion()
{
}

//=================================================================================
// 初期化処理
//=================================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size)
{
	MODEL model;

	model.dwNumMat = m_nNumMat;
	model.pBuffMat = m_pBuffMat;
	model.pMesh = m_pMesh;

	//モデル情報を設定
	BindModel(model);
	BindTexture(m_apTexture[0]);

	// 初期化処理
	CModel::Init(pos, size);				// 初期化情報
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き

	// 最初のサイズ取得
	m_FirstSize = size;

	// 拡縮の倍率
	m_fScale = 1.0f;

	// スケールを増やす値
	m_fScaleNum = EXPLOSION_SCALE_NUM;

	return S_OK;
}

//=================================================================================
// 終了処理
//=================================================================================
void CExplosion::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//=================================================================================
// 更新処理
//=================================================================================
void CExplosion::Update(void)
{
	// 更新処理
	CModel::Update();

	// 回転の加算
	AddRot();

	// 拡大
	ScaleUp();
}

//=================================================================================
// 描画処理
//=================================================================================
void CExplosion::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 透明度加算
	m_fAlphaNum += EXPLOSION_SUB_ALPHA;

	D3DXMATERIAL*pMat;		//マテリアルデータへのポインタ

	LPD3DXBUFFER pBuffMat = GetBuffMat();

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// 自己発光
		pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		// 透明
		pMat[nCntMat].MatD3D.Diffuse.a -= m_fAlphaNum;
	}

	// モデルの描画
	CModel::Draw();
}

//=================================================================================
// 拡大の処理
//=================================================================================
void CExplosion::ScaleUp(void)
{
	// スケールの加算
	m_fScale += m_fScaleNum;

	// サイズの取得
	D3DXVECTOR3 size =
		D3DXVECTOR3(m_FirstSize.x * m_fScale, m_FirstSize.y * m_fScale, m_FirstSize.z * m_fScale);

	// サイズを渡す
	SetSize(size);

	// 終了フラグ
	if (m_fScale >= EXPLOSION_SCALE_LIMIT)
	{
		// 終了処理
		Uninit();

		return;
	}
}

//=================================================================================
// 角度を加算
//=================================================================================
void CExplosion::AddRot(void)
{
	m_fAddRotNum += EXPLOSION_ADD_ROT;

	// 角度の変化
	D3DXVECTOR3 rot = D3DXVECTOR3(GetRot().x, GetRot().y + D3DXToRadian(m_fAddRotNum), GetRot().z);

	// 角度を渡す
	SetRot(rot);
}

//=================================================================================
// テクスチャロード
//=================================================================================
HRESULT CExplosion::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/Explosion004.jpg",
		&m_apTexture[0]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/Explosion.x",
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
void CExplosion::UnLoad(void)
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

	for (int nCount = 0; nCount < MAX_EXPLOSION_TEXTURE; nCount++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}