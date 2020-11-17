//=============================================================================
//
// メイン処理 [scene3D.h]
// Author : 山田陵太
//
//=============================================================================
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================

//=============================================================================
//3Dポリゴンクラスのコンストラクタ
//=============================================================================
CScene3D::CScene3D(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
//3Dポリゴンクラスのデストラクタ
//=============================================================================
CScene3D::~CScene3D()
{
}

//=============================================================================
//3Dポリゴンクラスのクリエイト処理
//=============================================================================
CScene3D * CScene3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//3Dポリゴンクラスのポインタ変数
	CScene3D *pScene3D = NULL;

	//メモリの確保
	pScene3D = new CScene3D;

	//メモリが確保できていたら
	if (pScene3D != NULL)
	{
		//初期化処理呼び出し
		pScene3D->Init(pos, size);
	}
	else
	{
		return NULL;
	}

	return pScene3D;
}

//=============================================================================
//3Dポリゴンクラスの初期化処理
//=============================================================================
HRESULT CScene3D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
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

	m_pos = pos;
	m_size = size;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標設定の設定
	pVtx[0].pos = D3DXVECTOR3(- (size.x / 2), + (size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+ (size.x / 2), + (size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- (size.x / 2), - (size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+ (size.x / 2), - (size.y / 2), 0.0f);

	//各頂点の法線の設定（※ベクトルの大きさは１にする必要がある）
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
//3Dポリゴンクラスの終了処理
//=============================================================================
void CScene3D::Uninit(void)
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
//3Dポリゴンクラスの更新処理
//=============================================================================
void CScene3D::Update(void)
{

}

//=============================================================================
//3Dポリゴンクラスの描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用のマトリクス

	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

void CScene3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CScene3D::SetPosision(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標設定の設定
	pVtx[0].pos = D3DXVECTOR3(- (m_size.x / 2), + (m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+ (m_size.x / 2), + (m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- (m_size.x / 2), - (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+ (m_size.x / 2), - (m_size.y / 2), 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色の設定
//=============================================================================
void CScene3D::SetColor(D3DXCOLOR col)
{
	VERTEX_3D*pVtx = NULL;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < 4; nCount++)
	{
		//頂点カラーの設定（0～255の数値で設定）
		pVtx[nCount].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色の設定
//=============================================================================
void CScene3D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

LPDIRECT3DVERTEXBUFFER9 CScene3D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

D3DXVECTOR3 CScene3D::GetPos(void)
{
	return m_pos;
}
