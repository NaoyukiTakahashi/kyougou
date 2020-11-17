//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "effect.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SCALE_DOWN_NUM		(0.003f)			// エフェクトの減算

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[MAX_EFFECT_TEXTURE] = {};

//=============================================================================
// インスタンス生成
//=============================================================================
CEffect * CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife)
{
	// インスタンス生成
	CEffect *pEffect = new CEffect;

	if (pEffect != NULL)
	{
		// 初期化処理
		pEffect->Init(pos, size);				// 初期化情報
		pEffect->SetMove(move);					// 移動量
		pEffect->BindTexture(m_apTexture[0]);	// テクスチャ設定
		pEffect->SetColor(col);					// 色の設定
		pEffect->m_nLife = nLife;				// ライフ
		pEffect->m_col = col;
	}

	return pEffect;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CEffect::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/effect003.png",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CEffect::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_EFFECT_TEXTURE; nCount++)
	{
		// テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bScaleDown = false;
	m_pVtxBuff = NULL;
	m_fScale = 1.0f;
	m_fScaleNum = 0.0f;
	m_nLife = 0;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 初期化処理
	CBillboard::Init(pos, size);
	m_Pos = pos;
	m_fScaleNum = SCALE_DOWN_NUM;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	// 2Dポリゴン終了処理
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	m_nLife--;

	// 2Dポリゴン更新処理
	CBillboard::Update();

	// スケール
	ScaleDown();

	// 小さくなりきったら
	if (m_nLife <= 0)
	{
		// 終了処理
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// かぶさらないようにする　(Zバッファ)
	//pDevice->SetRenderState(D3DRS_ZENABLE, false);

	// 加算合成を行う
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// aデスティネーションカラー

	D3DMATERIAL9 material, OldMaterial;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.1f, 0.3f, 1.0f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	pDevice->GetMaterial(&OldMaterial);
	pDevice->GetMaterial(&OldMaterial);
	pDevice->SetMaterial(&material);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	DWORD ambient;
	pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	pDevice->LightEnable(0, FALSE);

	// アルファテストを有力化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// アルファテスト基準値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 75);

	// 2Dポリゴン描画処理
	CBillboard::Draw();

	// アルファテスト基準値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// アルファテストを有力化
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// aデスティネーションカラー


	// Zバッファ戻す
	//pDevice->SetRenderState(D3DRS_ZENABLE, true);

	pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
	pDevice->SetMaterial(&OldMaterial);					// マテリアルを元に戻す
	pDevice->LightEnable(0, TRUE);
}

//=============================================================================
// ポリゴン縮小
//=============================================================================
void CEffect::ScaleDown()
{
	if (m_fScale >= 0.0f)
	{
		// 縮小量を加算
		m_fScale -= m_fScaleNum;

		// サイズの設定
		D3DXVECTOR3 size = D3DXVECTOR3(GetSize().x * m_fScale, GetSize().y * m_fScale, 0.0f);

		// サイズ
		SetSize(size);

		D3DXVECTOR3 pos = GetPos();
		SetPos(pos);
	}
	else
	{
		m_fScale = 1.0f;
	}
}

//=============================================================================
// 色の設定
//=============================================================================
void CEffect::SetColor(D3DXCOLOR col)
{
	LPDIRECT3DVERTEXBUFFER9 pBuff = GetBuff();

	VERTEX_3D *pVtx;

	//頂点バッファをロック
	pBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定（0～255の数値で設定）
	pVtx[0].col = D3DCOLOR_RGBA(20, 20, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(20, 20, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(20, 20, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(20, 20, 255, 255);

	//頂点バッファのアンロック
	pBuff->Unlock();
}

//=============================================
// 弾の設定
//=============================================
void CEffect::SetMove(D3DXVECTOR3 move)
{
	// 変数に代入
	m_move = move;
}