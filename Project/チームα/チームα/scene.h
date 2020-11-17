//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================

#define MAX_NUM 2056	//最大数

//=============================================================================
//オブジェクトクラス
//=============================================================================
class CScene
{
public:
	//=========================================================================
	//描画の優先順位の列挙型
	//=========================================================================
	typedef enum
	{
		PRIORITY_0 = 0,
		PRIORITY_1,
		PRIORITY_PLAYER,
		PRIORITY_MAX	//優先順位の最大数
	}PRIORITY;
	//=========================================================================
	//列挙型
	//=========================================================================
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,	//プレイヤータイプ
		OBJTYPE_SPHERE,	//球タイプ
		OBJTYPE_BULLET,	//バレットタイプ
		OBJTYPE_MODEL,	// モデル
		OBJTYPE_PAUSE,
		OBJTYPE_MAX	//オブジェクトタイプの最大数
	}OBJTYPE;	//オブジェクトタイプ

	//=========================================================================
	//メンバ関数宣言
	//=========================================================================
	CScene(int nPriority = PRIORITY_0);
	virtual ~CScene();
	static void ReleaseAll(void);
	static void AllUpdate(void);
	static void AllDraw(void);

	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetObjType(const OBJTYPE objtype);
	OBJTYPE GetObjType(void)const;
	CScene *GetNext(void);
	static CScene *GetTop(int nNum);
	static void SetPause(bool Pause);

protected:

	void SetDeathFlag(void);
private:
	void ReConnectList(void);
	//=========================================================================
	//メンバ変数宣言
	//=========================================================================
	static CScene *m_pTop[PRIORITY_MAX];	//先頭のオブジェクトへのポインタ
	static CScene *m_pCur[PRIORITY_MAX];	//現在のオブジェクトへのポインタ
	CScene *m_pPrev;	//前のオブジェクトへのポインタ
	CScene *m_pNext;	//次のオブジェクトへのポインタ
	int m_nPriority;	//描画の優先順位
	bool m_bDeath;	//死亡フラグ

	OBJTYPE m_ObjType;	//オブジェクトの種類
	static bool m_bPause;
};

#endif