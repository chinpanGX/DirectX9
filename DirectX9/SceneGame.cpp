/*===================================================

	[SceneGame.cpp]
	Author : �o���đ�

===================================================*/

#include "SceneGame.h"

void SceneGame::Init()
{
	DebugProc::Init();
	Light::Init();
	m_Camera.Init();
	Shadow_Initialize();	//	�e���g�p����I�u�W�F�N�g�̑O�ɏ���������
	m_Player.Init(D3DXVECTOR3 (0.0f,0.0f,0.0f), D3DXVECTOR3 (0.0f,10.0f,0.0f));
	m_Field.Init();
	Bullet_Initialize();
}

void SceneGame::Uninit()
{
	Bullet_Finalize();
	m_Field.Uninit();
	m_Player.Uninit();
	Shadow_Finalize();
	Light::Uninit();
	DebugProc::Uninit();
}

void SceneGame::Update()
{
	m_Player.Update();
	m_Camera.Update();
	Shadow_Update();	//	��
	Bullet_Update();
}

void SceneGame::Draw()
{
	DebugProc::Draw();
	m_Field.Draw();
	m_Camera.Set();
	Shadow_Draw();
	m_Player.Draw();
	Bullet_Draw();
}
