//=====================================================
//
// 2D爆発クラスヘッダー [2d_explosion.cpp]
// Author : Konishi Yuuto
//
//=====================================================

//=====================================================
// インクルード
//=====================================================
#include "2d_explosion.h"
#include "manager.h"
#include "renderer.h"

//=====================================================
// マクロ定義
//=====================================================
#define EXPLOSION_ANIME_SPEED_2D	(3)
#define EXPLOSION_ANIME_PATTERN_2D	(25)

//=====================================================
// static初期化s
//=====================================================
LPDIRECT3DTEXTURE9 C2dExplosion::m_pTexture = NULL;			// ポリゴンのテクスチャ

//=====================================================
// ポリゴン生成
//=====================================================
C2dExplosion * C2dExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// メモリ確保
	C2dExplosion *p2dExplosion = new C2dExplosion;

	if (p2dExplosion != NULL)
	{
		// 初期化処理
		p2dExplosion->Init(pos, size);
	}

	return p2dExplosion;
}

//=====================================================
// コンストラクタ
//=====================================================
C2dExplosion::C2dExplosion()
{

}

//=====================================================
// デストラクタ
//=====================================================
C2dExplosion::~C2dExplosion()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT C2dExplosion::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// 初期化処理
	CBillboard::Init(pos, size);

	// テクスチャ設定
	BindTexture(m_pTexture);

	// アニメーション設定
	InitAnimation(EXPLOSION_ANIME_SPEED_2D, EXPLOSION_ANIME_PATTERN_2D, 0);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void C2dExplosion::Uninit(void)
{
	// 終了処理
	CBillboard::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void C2dExplosion::Update(void)
{
	// 更新処理
	CBillboard::Update();
}

//=====================================================
// 描画処理
//=====================================================
void C2dExplosion::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// かぶさらないようにする　(Zバッファ)
//	pDevice->SetRenderState(D3DRS_ZENABLE, false);

	// 加算合成を行う
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// aデスティネーションカラー

	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.a = 1.0f;
	pDevice->SetMaterial(&material);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	DWORD ambient;
	pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	pDevice->LightEnable(0, FALSE);

	// アルファテストを有力化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// アルファテスト基準値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 125);

	// 2Dポリゴン描画処理
	CBillboard::Draw();

	// アルファテストを有力化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// アルファテスト基準値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// aデスティネーションカラー

	// Zバッファ戻す
//	pDevice->SetRenderState(D3DRS_ZENABLE, true);

	pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
	pDevice->LightEnable(0, TRUE);

}

//=====================================================
// テクスチャロード
//=====================================================
HRESULT C2dExplosion::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/2dExplosion001.png", &m_pTexture);
	return S_OK;
}

//=====================================================
// テクスチャアンロード
//=====================================================
void C2dExplosion::UnLoad(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}