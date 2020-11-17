//=============================================================================
//
// メッシュフィールドクラス [meshfield.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "meshfield.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// static初期化
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshField::m_apTexture[MAX_MESHFIELD_TEXTURE] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshField::CMeshField()
{
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き（回転）
	D3DXMatrixIdentity(&m_mtxWorld);
	m_nNumVertex = 0;							// 総頂点数
	m_nNumIndex = 0;							// 総インデックス数
	m_nNumPolygon = 0;							// 総ポリゴン数
	m_fOne_SizeW = 0.0f;
	m_fOne_SizeH = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{
}

//=============================================================================
// ポリゴン生成
//=============================================================================
CMeshField * CMeshField::Create(void)
{
	CMeshField *pMeshField = new CMeshField;

	if (pMeshField != NULL)
	{
		pMeshField->Init();
	}

	return pMeshField;
}

//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CMeshField::Load(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/stone_00124.jpg",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// テクスチャアンロード
//=============================================================================
void CMeshField::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_MESHFIELD_TEXTURE; nCount++)
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
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(void)
{
	// Rendererクラスからデバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	VERTEX_3D *pVtx;

	int nCount;

	int nCntIndex;
	int nCntIndex01 = 0;

	int nCntH;

	int nCntV;

	m_nNumVertex = (FIELD_WIDTH + 1) * (FIELD_HEIGHT + 1);
	m_nNumIndex = (FIELD_WIDTH + 1) * FIELD_HEIGHT * 2 + (FIELD_HEIGHT - 1) * 2;
	m_nNumPolygon = FIELD_WIDTH * FIELD_HEIGHT * 2 + (FIELD_HEIGHT - 1) * 4;

	m_fOne_SizeW = FIELD_WIDTH_SIZE * 2 / FIELD_WIDTH;
	m_fOne_SizeH = FIELD_HEIGHT_SIZE * 2 / FIELD_HEIGHT;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntV = 0; nCntV < FIELD_HEIGHT + 1; nCntV++)
	{
		for (nCntH = 0; nCntH < FIELD_WIDTH + 1; nCntH++)
		{
			// 頂点の設定
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].pos = D3DXVECTOR3(-FIELD_WIDTH_SIZE + (nCntH * m_fOne_SizeW), 0.0f, FIELD_HEIGHT_SIZE - (nCntV * m_fOne_SizeH));

			// 法線ベクトルの設定
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色の設定
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].tex = D3DXVECTOR2(float(nCntH), float(nCntV));
		}
	}

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの設定
	WORD *pIdx;

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (nCount = 0, nCntIndex = 0; nCount < m_nNumIndex; nCount += 2)
	{
		if (((nCount + 2) % (((FIELD_WIDTH + 1) * 2) + 2)) == 0 && nCount != 0)
		{
			// インデックスの設定
			pIdx[nCount] = nCntIndex - 1;

			// インデックスの設定
			pIdx[nCount + 1] = FIELD_WIDTH + nCntIndex + 1;
		}
		else
		{
			// インデックスの設定
			pIdx[nCount] = FIELD_WIDTH + nCntIndex + 1;

			// インデックスの設定
			pIdx[nCount + 1] = nCntIndex;

			nCntIndex++;
		}

	}
	// 頂点データをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの開放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshField::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	// Rendererクラスからデバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	D3DXMatrixMultiply(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	D3DXMatrixMultiply(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームにバインド
	pDevice->SetIndices(m_pIdxBuff);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// 頂点フォーマットの設定
	pDevice->SetTexture(0, m_apTexture[0]);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	// 頂点フォーマットの設定
	pDevice->SetTexture(0, NULL);

}