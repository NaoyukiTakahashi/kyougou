//=====================================================
//
// ビルボードクラスヘッダー [billboard.cpp]
// Author : 小西優斗
//
//=====================================================

//=====================================================
// インクルード
//=====================================================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"

//=====================================================
// コンストラクタ
//=====================================================
CBillboard::CBillboard(int nPriority) : CScene(nPriority)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置情報
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_sizeBase = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ベースのサイズ
	m_Dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 方向7
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_pTexture = NULL;							// ポリゴンのテクスチャ
	m_nLife = 0;								// 寿命
	m_bUse = false;								// 使用判定
	m_nCountAnim = 0;;							// アニメーションテクスチャ
	m_nCountAnimPattern = 0;					// アニメーションのパターン
	m_nCounterAnim = 0;							// アニメーションのカウンター
	m_nPatternAnim = 0;							// アニメーションのパターン数
	m_nLoop = -1;								// ループするか
}

//=====================================================
// デストラクタ
//=====================================================
CBillboard::~CBillboard()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBillboard::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D*pVtx = NULL;

	m_Pos = pos;
	m_size = size;
	m_sizeBase = size;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標設定の設定
	
	pVtx[0].pos = D3DXVECTOR3(- m_size.x / 2, + m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+ m_size.x / 2, + m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- m_size.x / 2, - m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+ m_size.x / 2, - m_size.y / 2, 0.0f);

	//各頂点の法線の設定（※ベクトルの大きさは１にする必要がある）

	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定（0～255の数値で設定）

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBillboard::Uninit(void)
{	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクト破棄
	SetDeathFlag();
}

//=====================================================
// 更新処理
//=====================================================
void CBillboard::Update(void)
{
	m_Pos += m_move;

	// アニメーションの設定がされたとき
	if (m_nPatternAnim != 0)
	{
		// アニメーションを更新する
		UpdateAnimation();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CBillboard::Draw(void)
{
	// デバイス情報取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
//
	//計算用のマトリクス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
	DWORD ambient;

	//現在アンビエント情報を保存
	//pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);

	//pDevice->SetRenderState(D3DRS_AMBIENT, 0xff030303);   // ちょっと白く照らす	//ライティングをOFFにする
	//pDevice->SetRenderState(D3DRS_AMBIENT, ambient);

	//pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	//pDevice->LightEnable(0, TRUE);

	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxScale,
		m_size.x / m_sizeBase.x,
		m_size.y / m_sizeBase.y,
		0.0f);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 回転の逆行列の生成
	pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
	D3DXMatrixInverse(&m_mtxWorld, NULL,
		&mtxRot);

	m_mtxWorld._41 = 0;
	m_mtxWorld._42 = 0;
	m_mtxWorld._43 = 0;

	// アルファテストを有力化
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// アルファテストの比較方法の設定(GREATERは基準値より大きい場合)
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 位置を反映、ワールドマトリクス設定、ポリゴン描画
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリクスの設定 初期化、向き、位置
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// アルファテストを無効化
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetTexture(0, NULL);

//	pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
	pDevice->LightEnable(0, TRUE);
}

//=============================================
// アニメーション情報取得
//=============================================
void CBillboard::InitAnimation(int nCounterAnim, int nPatternAnim, int nLoop)
{
	m_nCounterAnim = nCounterAnim;
	m_nPatternAnim = nPatternAnim;
	m_nLoop = nLoop;

	// 頂点情報を設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern + (float)(1.0f / m_nPatternAnim), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern + (float)(1.0f / m_nPatternAnim), 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================
// アニメーション更新関数
//=============================================
void CBillboard::UpdateAnimation(void)
{
	// 爆発のアニメーションカウントを進めて、パターンを切り替える
	m_nCountAnim++;

	// 頂点情報(テクスチャ座標)の更新
	if (m_nCountAnim >= m_nCounterAnim)	// 爆発の速さ
	{
		// アニメーションのカウントを0にする
		m_nCountAnim = 0;

		// アニメーションのパターンをカウントさせる
		m_nCountAnimPattern++;
	}

	// アニメーションが終わったら
	if (m_nCountAnimPattern >= m_nPatternAnim)
	{
		// 数値を戻しておく
		m_nCountAnimPattern = 0;

		if (m_nLoop == 0)
		{
			// 終了処理
			Uninit();
		}
	}
	else
	{
		// 頂点情報を設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標を更新
		pVtx[0].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern + (float)(1.0f / m_nPatternAnim), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((float)(1.0f / m_nPatternAnim)*(float)m_nCountAnimPattern + (float)(1.0f / m_nPatternAnim), 1.0f);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

LPDIRECT3DVERTEXBUFFER9 CBillboard::GetBuff(void)
{
	return m_pVtxBuff;
}

//=====================================================
// 位置の設定
//=====================================================
void CBillboard::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;

	VERTEX_3D*pVtx = NULL;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標設定の設定
	pVtx[0].pos = D3DXVECTOR3(- m_size.x / 2, + m_size.y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+ m_size.x / 2, + m_size.y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- m_size.x / 2, - m_size.y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+ m_size.x / 2, - m_size.y / 2, 0.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=====================================================
// 移動量設定
//=====================================================
void CBillboard::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=====================================================
// サイズ設定
//=====================================================
void CBillboard::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

void CBillboard::SetSizeBase(D3DXVECTOR3 sizeBase)
{
	m_sizeBase = sizeBase;
}

//=====================================================
// 位置の取得
//=====================================================
D3DXVECTOR3 CBillboard::GetPos(void)
{
	return m_Pos;
}

//=====================================================
// 移動量情報
//=====================================================
D3DXVECTOR3 CBillboard::GetMove(void)
{
	return m_move;
}

//=====================================================
// サイズ情報
//=====================================================
D3DXVECTOR3 CBillboard::GetSize(void)
{
	return m_size;
}

//=====================================================
// テクスチャの設定
//=====================================================
void CBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}