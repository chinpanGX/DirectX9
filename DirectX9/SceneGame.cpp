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
	m_shadow.Init();	//	�e���g�p����I�u�W�F�N�g�̑O�ɏ���������
	m_Player.Init(D3DXVECTOR3 (0.0f,0.0f,0.0f), D3DXVECTOR3 (0.0f,10.0f,0.0f));
	m_Field.Init();
	//m_BB.Init();
	m_bullet.Init();
	m_cannon.Init();
}

void SceneGame::Uninit()
{
//	m_cannon.Uninit();
//	m_bullet.Uninit();
	//m_BB.Uninit();
//	m_Field.Uninit();
//	m_Player.Uninit();
//	m_shadow.Uninit();
//	Light::Uninit();
//	DebugProc::Uninit();
}

void SceneGame::Update()
{
	m_Player.Update();
	m_Camera.Update();
	//m_BB.Update();
	m_shadow.Update();	//	��
	m_bullet.Update();
	m_cannon.Update();
}

void SceneGame::Draw()
{
	DebugProc::Draw();
	m_Field.Draw();
	m_Camera.Set();
	m_shadow.Draw();
	m_Player.Draw();
	//m_BB.Draw();
	m_bullet.Draw();
	m_cannon.Draw();
}
