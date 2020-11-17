#ifndef _SHADER_H_
#define _SHADER_H_

//====================================================================
//
// シェーダの処理ヘッダー	[shader.h]
// Author : Konishi Yuuto
//
//====================================================================

//====================================================================
// インクルード
//====================================================================
#include "main.h"

//====================================================================
// シェーダクラス
//====================================================================
class CShader
{
public:
	CShader();							// コンストラクタ
	~CShader();							// デストラクタ

	BOOL Init();					// 初期化処理
	void Draw(D3DXMATRIX Matrix);		// 描画処理
	void Delete(void);					// 消去

private:
	// 構造体宣言
	typedef struct _VERTEX
	{
		D3DXVECTOR3 m_Pos;
	}VERTEX;	

	LPDIRECT3DVERTEXBUFFER9 m_pBuffer;			// 頂点用バッファ
	LPD3DXEFFECT m_pShader;						// シェーダデータ
	D3DXHANDLE m_Tech;							// テクニックへのハンドル
	D3DXHANDLE m_Matrix;						// シェーダのグローバル変数用ハンドル(マトリクス)
	D3DXHANDLE m_Color;							// シェーダのグローバル変数用ハンドル(カラー)
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;		// 頂点デクラレーションへのポインタ

};

#endif