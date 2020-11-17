//====================================================================
//
// シェーダの処理ヘッダー	[shader.cpp]
// Author : Konishi Yuuto
//
//====================================================================

//====================================================================
// インクルード
//====================================================================
#include "shader.h"
#include "renderer.h"
#include "manager.h"

//=================================================================================================
// コンストラクタ
//=================================================================================================
CShader::CShader()
{
}

//=================================================================================================
// デストラクタ
//=================================================================================================
CShader::~CShader()
{
}

//=================================================================================================
// 初期化処理
//=================================================================================================
BOOL CShader::Init()
{
	// Rendererクラスからデバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点のフォーマット宣言
	D3DVERTEXELEMENT9 vertex_elements[] =
	{
		{
			0,
			0,
			D3DDECLTYPE_FLOAT3,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
		},
		D3DDECL_END()
	};

	pDevice->CreateVertexDeclaration(vertex_elements, &m_pDecl);

	// 頂点の作成
	if (FAILED(pDevice->CreateVertexBuffer(
		4 * sizeof(VERTEX),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pBuffer,
		NULL)))
	{
		return false;
	}

	VERTEX *tmp_vertex;

	// 頂点を編集するためにロック
	m_pBuffer->Lock(0, 0, (void**)&tmp_vertex, 0);

	// 頂点座標の変更
	tmp_vertex[0].m_Pos = D3DXVECTOR3(-100.0f, 50.0f, -100.0f);
	tmp_vertex[1].m_Pos = D3DXVECTOR3(100.0f, 50.0f, -100.0f);
	tmp_vertex[2].m_Pos = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	tmp_vertex[3].m_Pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);

	// 頂点のロックを解除
	m_pBuffer->Unlock();

	// シェーダの読み込み
	LPD3DXBUFFER error = NULL;

	D3DXCreateEffectFromFile(pDevice,
		TEXT("Shader.hlsl"),
		0,
		0,
		D3DXSHADER_DEBUG,
		0,
		&m_pShader,
		&error);

	if (m_pShader == NULL)
	{
		return false;
	}

	// テクニックのハンドル取得
	m_Tech = m_pShader->GetTechniqueByName("StandardDraw");			// テクニック取得

	// グローバル変数のハンドル取得
	m_Matrix = m_pShader->GetParameterByName(NULL, "m_Matrix");		// パラメータ取得
	m_Color = m_pShader->GetParameterByName(NULL, "m_Color");

	return true;
}

//=================================================================================================
// 描画処理
//=================================================================================================
void CShader::Draw(D3DXMATRIX Matrix)
{
	// Rendererクラスからデバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点の流し込み
	pDevice->SetStreamSource(0, m_pBuffer, 0, sizeof(VERTEX));

	// 色変更
	D3DXVECTOR4 color;
	color.x = 1.0f;
	color.y = 1.0f;
	color.z = 1.0f;
	color.w = 1.0f;

	// 頂点フォーマットの設定
	pDevice->SetVertexDeclaration(m_pDecl);

	// テクニックの設定
	m_pShader->SetTechnique(m_Tech);

	// パラメータ(グローバル変数の設定)
	m_pShader->SetMatrix(m_Matrix, &Matrix);		// マテリアル
	m_pShader->SetVector(m_Color, &color);			// 頂点

	// パラメータ更新
	m_pShader->CommitChanges();			// GPU側のデータを更新する

	// シェーダ起動
	m_pShader->Begin(0, 0);		// 引数：シェーダのパス数、デバイスステートの保存設定フラグ

	// パス起動
	m_pShader->BeginPass(0);	// 引数：起動したシェーダのパス数の数

	// ポリゴン描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	// パス終了
	m_pShader->EndPass();

	// シェーダ終了
	m_pShader->End();

}

//=================================================================================================
// 消去処理
//=================================================================================================
void CShader::Delete(void)
{
	if (m_pBuffer != NULL)
	{
		m_pBuffer->Release();
		m_pBuffer = NULL;
	}

	if (m_pDecl != NULL)
	{
		m_pDecl->Release();
		m_pDecl = NULL;
	}

	if (m_pShader != NULL)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

}
