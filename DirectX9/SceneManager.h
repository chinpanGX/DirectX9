/*==========================================

	[SceneManager.h]
	Author : 出合翔太

============================================*/

#pragma once
#include "main.h"
#include "SceneGame.h"

class SceneManager
{
private:
	static SceneBase			*m_Scene;	//	シーンのインスタンス
	static LPDIRECT3DDEVICE9	m_pDevice;	//	デバイス
public:
	static void Init();						//	初期化処理
	static void Uninit();					//	終了処理
	static void Update();					//	更新処理
	static void Draw();						//	描画処理
};

