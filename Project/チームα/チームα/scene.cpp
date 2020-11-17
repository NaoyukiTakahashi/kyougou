//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "scene.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CScene * CScene::m_pTop[CScene::PRIORITY_MAX] = {};
CScene * CScene::m_pCur[CScene::PRIORITY_MAX] = {};

bool CScene::m_bPause = false;

//=============================================================================
//オブジェクトクラスのデフォルトコンストラクタ
//=============================================================================
CScene::CScene(int nPriority)
{
	//プライオリティの設定
	m_nPriority = nPriority;

	//死亡フラグをfalseに設定する
	m_bDeath = false;

	//先頭オブジェクトが確保されていないとき
	if (!m_pTop[m_nPriority])
	{
		//先頭オブジェクトに自身のポインタを設定する
		m_pTop[m_nPriority] = this;

		//先頭のため、前情報をNULLでクリアする
		m_pPrev = NULL;
	}

	//現在オブジェクトが確保されていなかった場合
	if (!m_pCur[m_nPriority])
	{
		//現在オブジェクトへ自身のポインタを設定
		m_pCur[m_nPriority] = this;
	}

	//現在オブジェクトの次のオブジェクト情報に自分の情報を入れる
	m_pCur[m_nPriority]->m_pNext = this;

	//現在オブジェクトが自分だった場合
	if (m_pCur[m_nPriority] == this)
	{
		//前情報にNULLを入れる
		m_pPrev = NULL;
	}
	else
	{
		//前情報に現在オブジェクトを入れる
		m_pPrev = m_pCur[m_nPriority];
	}

	//現在オブジェクトに自身のポインタ情報に上書きする
	m_pCur[m_nPriority] = this;

	//自身の次情報をクリアする
	m_pNext = NULL;

	m_ObjType = OBJTYPE_NONE;
}

//=============================================================================
//オブジェクトクラスのデフォルトデストラクタ
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
//全ての終了処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//先頭オブジェクトの情報を取得
		CScene *pScene = m_pTop[nCntPriority];

		while (pScene)
		{
			//次情報の一時保存
			CScene *pNext = pScene->m_pNext;

			//終了処理呼び出し
			pScene->Uninit();

			//リストの再接続
			pScene->ReConnectList();

			//オブジェクト情報を次のものへ移行する
			pScene = pNext;
		}
	}
}

//=============================================================================
//全ての更新処理
//=============================================================================
void CScene::AllUpdate(void)
{
	 //ポーズしているか　bool pause = m_pause   ->>  pause =true ->> if (type == OBJTYPE_PAUSE)
	
		//更新処理
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//先頭オブジェクトの情報を取得
			CScene *pScene = m_pTop[nCntPriority];

			while (pScene)
			{
				//次情報の一時保存
				CScene *pNext = pScene->m_pNext;
				if (m_bPause == false || pScene->GetObjType() == OBJTYPE_PAUSE)
				{


					if (!pScene->m_bDeath)
					{
						//更新処理呼び出し
						pScene->Update();
					}
				}
				//オブジェクト情報を次のものへ移行
				pScene = pNext;

			}
		}
	
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//先頭オブジェクトの情報を取得
		CScene *pScene = m_pTop[nCntPriority];

		while (pScene)
		{
			//次情報の一時保存
			CScene *pNext = pScene->m_pNext;

			if (pScene->m_bDeath == true)
			{
				//リストの再接続
				pScene->ReConnectList();
			}

			//オブジェクト情報を次のものへ移行
			pScene = pNext;
		}
	}

	////最大ポリゴン数分繰り返す
	//for (int nCntScene = 0; nCntScene < MAX_NUM; nCntScene++)
	//{
	//	CScene *pScene = GetScene(nCntScene);

	//	if (pScene != NULL)
	//	{
	//		// それぞれのタイプ
	//		OBJTYPE type = pScene->GetObjType();

	//		// ポーズしているか　bool pause = m_pause   ->>  pause =true ->> if (type == OBJTYPE_PAUSE)
	//		if (m_bPause == false || type == OBJTYPE_PAUSE)
	//		{
	//			//メモリが確保できていたら
	//			if (m_apScene[nCntScene] != NULL)
	//			{
	//				//更新処理呼び出し
	//				m_apScene[nCntScene]->Update();
	//			}
	//		}
	//	}
	//}
}


//=============================================================================
//全ての描画処理
//=============================================================================
void CScene::AllDraw(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//先頭オブジェクトの情報を取得
		CScene *pScene = m_pTop[nCntPriority];

		while (pScene)
		{
			if (!pScene->m_bDeath)
			{
				//描画処理の呼び出し
				pScene->Draw();
			}
			//オブジェクト情報を次のものへ移行する
			pScene = pScene->m_pNext;
		}
	}
}

//=============================================================================
//削除処理
//=============================================================================
void CScene::SetDeathFlag(void)
{
	//死亡フラグをtrueにする
	m_bDeath = true;
}

void CScene::ReConnectList(void)
{
	//自身が先頭オブジェクトだった場合
	if (this == m_pTop[m_nPriority])
	{
		//自身が持つ次のオブジェクト情報をを先頭に指定する
		m_pTop[m_nPriority] = m_pNext;
	}

	//自身が現在オブジェクトだった場合
	if (this == m_pCur[m_nPriority])
	{
		//自身が持つ前のオブジェクト情報を現在に指定する
		m_pCur[m_nPriority] = m_pPrev;
	}

	//前情報を持っているなら
	if (m_pPrev)
	{
		//前のオブジェクトの次情報に自身の次情報を渡す
		m_pPrev->m_pNext = m_pNext;
	}

	//次情報を持っているなら
	if (m_pNext)
	{
		//次のオブジェクトの前情報に自身の前情報を渡す
		m_pNext->m_pPrev = m_pPrev;
	}

	//メモリの削除
	delete this;
}

//=============================================================================
//オブジェクトタイプ設定処理
//=============================================================================
void CScene::SetObjType(const OBJTYPE objtype)
{
	//メモリが確保できていたら
	this->m_ObjType = objtype;
}

//=============================================================================
//オブジェクトタイプ情報取得処理
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)const
{
	return this->m_ObjType;
}

CScene * CScene::GetNext(void)
{
	return m_pNext;
}

//=============================================================================
//先頭情報の取得処理
//=============================================================================
CScene * CScene::GetTop(int nNum)
{
	return m_pTop[nNum];
}

//=============================================================================
//ポーズ情報の取得
//=============================================================================
void CScene::SetPause(bool Pause)
{
	m_bPause = Pause;
}