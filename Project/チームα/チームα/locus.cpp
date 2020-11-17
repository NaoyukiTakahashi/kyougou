//=============================================================================
//
// 軌跡の処理 [locus.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "locus.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CLocus::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CLocus::CLocus()
{
	m_nLife = 0;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CLocus::~CLocus()
{

}

//=============================================================================
// クリエイト
//=============================================================================
CLocus * CLocus::Create(D3DXVECTOR3 Origin, D3DXVECTOR3 Top, D3DXVECTOR3 OldOrigin, D3DXVECTOR3 OldTop,
	D3DXVECTOR3 rot, D3DXVECTOR3 size, int nLife)
{
	//ポインタ変数
	CLocus *pLocus = NULL;

	//メモリの確保
	pLocus = new CLocus;

	//メモリが確保できていたら
	if (pLocus != NULL)
	{
		//初期化処理呼び出し
		pLocus->Init(Origin, Top, OldOrigin, OldTop);
		pLocus->m_rot = rot;
		pLocus->m_nLife = nLife;
	}
	else
	{
		return NULL;
	}

	return pLocus;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CLocus::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/SwordLocus003.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CLocus::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLocus::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLocus::Init(D3DXVECTOR3 Origin, D3DXVECTOR3 Top, D3DXVECTOR3 OldOrigin, D3DXVECTOR3 OldTop)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D*pVtx = NULL;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	////頂点座標設定の設定
	//pVtx[0].pos = OldOrigin;
	//pVtx[1].pos = OldTop;
	//pVtx[2].pos = Origin;
	//pVtx[3].pos = Top;

	//頂点座標設定の設定
	pVtx[0].pos = OldTop;
	pVtx[1].pos = Top;
	pVtx[2].pos = OldOrigin;
	pVtx[3].pos = Origin;

	D3DXVECTOR3 size;

	//各頂点の法線の設定（※ベクトルの大きさは１にする必要がある）
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//頂点カラーの設定（0～255の数値で設定）
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//各頂点の法線の設定（※ベクトルの大きさは１にする必要がある）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLocus::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクト破棄
	SetDeathFlag();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLocus::Update(void)
{
	m_nLife--;

	if (m_nLife <= 0)
	{
		// 終了処理
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLocus::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用のマトリクス

	pDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//透明度を使うか
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//ディスティネーションカラーの指定

																   // アルファテスト基準値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	D3DMATERIAL9 material, OldMaterial;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	pDevice->GetMaterial(&OldMaterial);
	pDevice->SetMaterial(&material);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	DWORD ambient;
	pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	D3DXMATRIX EffectMtx;

	D3DXMatrixIdentity(&EffectMtx);

	pDevice->SetTexture(0, m_pTexture);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &EffectMtx);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
	pDevice->SetMaterial(&OldMaterial);					// マテリアルを元に戻す

	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	pDevice->SetTexture(0, NULL);
}