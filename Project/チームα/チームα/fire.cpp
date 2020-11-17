//=============================================================================
//
// 炎クラス [fire.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "fire.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SCALE_DOWN		(0.1f)		// 縮小の値
#define ALPHA_NUM		(0.05f)		// 透明度の値

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CFire::m_apTexture[MAX_FIRE_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CFire * CFire::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,
	 int nLife)
{
	// インスタンス生成
	CFire *pFire = new CFire;

	if (pFire != NULL)
	{
		// 距離の設定
		int nDis = 
			rand() % FIRE_DISTANCE + rand() % FIRE_DISTANCE - rand() % FIRE_DISTANCE - rand() % FIRE_DISTANCE;

		// ランダムで出現を決める
		D3DXVECTOR3 TargetPos = D3DXVECTOR3(pos.x + nDis ,
			pos.y + nDis, pos.z + nDis);

		// 初期化処理
		pFire->Init(TargetPos, size);

		// 速度の設定
		int nSpeed = 0;
		nSpeed += rand() % 4 + rand() % 4 + rand() % 4;

		// 移動量
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, nSpeed, 0.0f);

		// 移動量
		pFire->SetMove(move);

		// テクスチャのの設定
		pFire->BindTexture(m_apTexture[0]);

		// ライフの設定
		pFire->SetLife(nLife);
	}

	return pFire;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CFire::CFire()
{
	m_nLife = 0;
	m_fScale = 0.0f;
	m_fScaleNum = 0.0f;
	m_fAlpha = 0.0f;
	m_fAlphaNum = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFire::~CFire()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFire::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 初期化処理
	CBillboard::Init(pos, size);

	// 縮小
	m_fScaleNum = SCALE_DOWN;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFire::Uninit(void)
{
	// 2Dポリゴン終了処理
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFire::Update(void)
{
	// ライフ減算
	m_nLife--;

	// 2Dポリゴン更新処理
	CBillboard::Update();

	// 縮小の処理
	ScaleDown();

	// 透明度
	//Alpha();

	// ライフが0になったら
	if (m_nLife <= 0)
	{
		// 終了処理
		Uninit();

		return;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFire::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// かぶさらないようにする　(Zバッファ)
	pDevice->SetRenderState(D3DRS_ZENABLE, false);

	// 加算合成を行う
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// aデスティネーションカラー

	D3DMATERIAL9 material, OldMaterial;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(1.0f, 0.4f, 0.04f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	pDevice->GetMaterial(&OldMaterial);
	pDevice->SetMaterial(&material);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	DWORD ambient;
	pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	pDevice->LightEnable(0, FALSE);

	// 2Dポリゴン描画処理
	CBillboard::Draw();

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// aデスティネーションカラー

	pDevice->SetRenderState(D3DRS_ZENABLE, true);		// Zバッファ戻す
	pDevice->SetRenderState(D3DRS_AMBIENT, ambient);	// アンビエントを元に戻す
	pDevice->SetMaterial(&OldMaterial);					// マテリアルを元に戻す
	pDevice->LightEnable(0, TRUE);						// ライトを戻す
}

//=============================================================================
// カラーの設定
//=============================================================================
void CFire::SetColor(D3DXCOLOR col)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 左上頂点の色	透明度255
	pVtx[1].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 右上頂点の色	透明度255
	pVtx[2].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 左下頂点の色	透明度255
	pVtx[3].col = D3DCOLOR_RGBA(255, 100, 0, 255);	// 右下頂点の色	透明度255

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// ライフの設定
//=============================================================================
void CFire::SetLife(int nLife)
{
	m_nLife = nLife - rand() % 30;
}

//=============================================================================
// 縮小
//=============================================================================
void CFire::ScaleDown(void)
{
	// サイズ受け取り
	D3DXVECTOR3 size = GetSize();

	size.x -= m_fScaleNum;
	size.y -= m_fScaleNum;

	// サイズ受け渡し
	SetSize(size);
}

//=============================================================================
// 透明度
//=============================================================================
void CFire::Alpha(void)
{

}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CFire::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/fire111.jpg",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CFire::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_FIRE_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}