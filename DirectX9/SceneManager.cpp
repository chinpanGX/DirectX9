/*===========================================

	[SceneManager.cpp]
	Author : 出合翔太

============================================*/

#include "main.h"
#include "SceneManager.h"

//	スタティック変数
SceneBase			*SceneManager::m_Scene;	//	シーンのインスタンスを格納
LPDIRECT3DDEVICE9	SceneManager::m_pDevice;	//	デバイス

//	初期化処理
void SceneManager::Init()
{
	m_pDevice = GetD3DDevice();
	//	メモリの確保
	m_Scene = new SceneGame;	//	SceneGameのインスタンス
	m_Scene->Init();			//	Init()を呼び出す
}

//	終了処理
void SceneManager::Uninit()
{
	m_Scene->Uninit();	//	Uninit()を呼び出す
	
	//	メモリの解放
	delete m_Scene;		
}

//	更新処理
void SceneManager::Update()
{
	m_Scene->Update();	//	Update()を呼び出す
}

//	描画処理
void SceneManager::Draw()
{
	m_Scene->Draw();	//	Draw()を呼び出す
}
