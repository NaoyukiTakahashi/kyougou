//================================================
//
// 数字表示処理 [number.h]
// Author : 佐藤颯紀
//
//================================================

#include "number.h"
#include "manager.h"
#include "renderer.h"

//================================================
//静的メンバ変数宣言
//================================================
LPDIRECT3DTEXTURE9 CNumber::m_apTexture[NUMBER_TYPE] = {};
//================================================
//クリエイト処理
//================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, NUMBERTYPE type)
{
	CNumber *pNumber;
	pNumber = new CNumber;
	pNumber->Init(pos, size,type);

	return pNumber;

}

//================================================
//画像の読み込み
//================================================
HRESULT CNumber::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Number.png", //ファイルの読み込み
		&m_apTexture[NUMBERTYPE_GAME]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Number002.png", //ファイルの読み込み
		&m_apTexture[NUMBERTYPE_CONTINUE]);

	return S_OK;
}

//================================================
//テクスチャの破棄
//================================================
void CNumber::Unload(void)
{
	for (int nCount = 0; nCount < NUMBER_TYPE; nCount++)
	{
		//テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
	
}

//====================================================
//コンストラクタ
//====================================================
CNumber::CNumber()
{
	m_nNumber = 0;
}

//====================================================
//デストラクタ
//====================================================
CNumber::~CNumber()
{

}

//================================================
//初期化処理
//================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, NUMBERTYPE type)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	m_pos = pos;	//位置の取得
	m_size = size;	//サイズの取得
	m_type = type;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点データ範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//位置とサイズの設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (-m_size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2), m_pos.y + (m_size.y / 2), 0.0f);

	//rhwの設定値は1,0で固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0~255の数値で設定)
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //r,g,b,aは0~255の範囲で決めること
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //r:レッド g:グリーン　b:ブルー
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //aは、透明度を表している
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャーの配置設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	pVtx += 4;
	//頂点データをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;

}

//================================================
//終了処理
//================================================
void CNumber::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//================================================
//更新処理
//================================================
void CNumber::Update(void)
{

}

//================================================
//描画処理
//================================================
void CNumber::Draw(void)
{
	for (int nCount = 0; nCount < NUMBER_TYPE; nCount++)
	{
		LPDIRECT3DDEVICE9 pDevice;
		pDevice = CManager::GetRenderer()->GetDevice();

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetTexture(0, m_apTexture[m_type]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP, //プリミティブの種類
			0,
			NUM_POLYGON);
	}
}

//=====================================================
//セット処理
//=====================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	m_nNumber = nNumber;

	//頂点データ範囲をロックし、頂点バッファへのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_nNumber*0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_nNumber*0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nNumber*0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_nNumber*0.1f + 0.1f, 1.0f);

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}