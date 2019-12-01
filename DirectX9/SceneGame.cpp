/*===================================================

	[Scene.cpp]
	Author : èoçá„ƒëæ

===================================================*/

#include "SceneGame.h"

void SceneGame::Init()
{
	DebugProc::Init();
	Texture::Load();
	Light::Init();
	m_Camera.Init();
	m_Player.Init(D3DXVECTOR3 (0.0f,0.0f,0.0f), D3DXVECTOR3 (0.0f,0.0f,0.0f));
	m_Field.Init();
	m_Grid.Init();
	m_Box.Init();
}

void SceneGame::Uninit()
{
	DebugProc::Uninit();
	Texture::Unload();
	Light::Uninit();
	m_Player.Uninit();
	m_Field.Uninit();
	m_Box.Uninit();
}

void SceneGame::Update()
{
	m_Player.Update();
	m_Camera.Update();
}

void SceneGame::Draw()
{
	DebugProc::Draw();
	m_Camera.Set();
	m_Player.Draw();
	m_Field.Draw();
	m_Grid.Draw();
	m_Box.Draw();
}
