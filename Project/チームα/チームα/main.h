//=============================================================================
//
// アクション　ブロック処理 [main.h]
// Author : 山田陵太
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <thread>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)	//警告対処用（"dinput.h"の直前に定義）
#include <dinput.h>
#include "d3dx9math.h"


//=============================================================================
// ライブラリのリンク
//=============================================================================
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")	//入力処理に必要
//#pragma comment(lib, "dxguid.lib")
#pragma comment (lib,"ws2_32.lib")

//=============================================================================
// マクロ定義
//=============================================================================
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE| D3DFVF_TEX1)	//座標・法線・カラー
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) //頂点フォーマット
#define NUM_VERTEX (4)	//頂点数
#define NUM_POLYGON (2)	//ポリゴン数

//=============================================================================
//構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_3D;

typedef struct
{
	D3DXVECTOR3 pos; // 頂点座標
	float rhw;		 //	座標変換用係数（1.0fで固定）
	D3DCOLOR  col;	 // 頂点カラー
	D3DXVECTOR2 tex; // テクスチャ座標
}VERTEX_2D;
#endif