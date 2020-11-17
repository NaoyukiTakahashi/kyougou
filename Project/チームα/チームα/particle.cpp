//=============================================================================
//
// スパーククラス [particle.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "particle.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_apTexture[MAX_SPARK_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CParticle * CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col,
	int nAngle, int nBaseAngle, int nDistance, int nLife, int nSpeed)
{
	// インスタンス生成
	CParticle *pParticle = new CParticle;

	if (pParticle != NULL)
	{
		// 距離の設定
		int nDis = rand() % nDistance + rand() % nDistance - rand() % nDistance - rand() % nDistance;

		// ランダムで出現を決める
		D3DXVECTOR3 TargetPos = D3DXVECTOR3(pos.x + nDis * cosf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))),
			pos.y + nDis * sinf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))), pos.z + nDis * sinf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))));

		// 初期化処理
		pParticle->Init(TargetPos, size);

		// 移動量
		D3DXVECTOR3 move = D3DXVECTOR3(cosf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))) * nSpeed,
			sinf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))) * +nSpeed,
			sinf(D3DXToRadian(nBaseAngle + (rand() % nAngle - rand() % nAngle))) * nSpeed);

		// 移動量
		pParticle->SetMove(move);

		// テクスチャのの設定
		pParticle->BindTexture(m_apTexture[0]);

		// ライフの設定
		pParticle->SetLife(nLife);
	}

	return pParticle;
}


//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle()
{
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 初期化処理
	CBillboard::Init(pos, size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	// 2Dポリゴン終了処理
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	// ライフ減算
	m_nLife--;

	// 2Dポリゴン更新処理
	CBillboard::Update();

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
void CParticle::Draw(void)
{
	// 描画処理
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// かぶさらないようにする　(Zバッファ)
	pDevice->SetRenderState(D3DRS_ZENABLE, false);

	// 加算合成を行う
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// aデスティネーションカラー

	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient.r = 1.0f;
	material.Ambient.g = 0.5f;
	material.Ambient.b = 0.0f;
	material.Ambient.a = 1.0f;
	pDevice->SetMaterial(&material);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	// 描画処理
	CBillboard::Draw();

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// aデスティネーションカラー

	// Zバッファ戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, true);
}

//=============================================================================
// カラーの設定
//=============================================================================
void CParticle::SetColor(D3DXCOLOR col)
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
void CParticle::SetLife(int nLife)
{
	m_nLife = nLife - rand() % 30;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CParticle::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/spark.png",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CParticle::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_SPARK_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}