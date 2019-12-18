/*===================================================

	[SceneGame.cpp]
	Author : �o���đ�

===================================================*/

#include "SceneGame.h"
#include "enemy.h";
#include "collision.h"

void SceneGame::Init()
{
	DebugProc::Init();
	Light::Init();
	m_Camera.Init();
	m_Shadow.Init();	//	�e���g�p����I�u�W�F�N�g�̑O�ɏ���������
	m_Player.Init(D3DXVECTOR3 (0.0f,0.0f,0.0f), D3DXVECTOR3 (0.0f,0.0f,0.0f));
	m_Field.Init();
	m_Bullet.Init();
	m_Enemy.Init();
}

void SceneGame::Uninit()
{
	m_Enemy.Uninit();
	m_Bullet.Uninit();
	m_Field.Uninit();
	m_Player.Uninit();
	m_Shadow.Uninit();
	DebugProc::Uninit();
}

void SceneGame::Update()
{
	m_Player.Update();
	m_Camera.Update();
	m_Shadow.Update();	//	��
	m_Bullet.Update();
	m_Enemy.Update();
	Collision_Update();
}

void SceneGame::Draw()
{
	DebugProc::Draw();
	m_Field.Draw();
	m_Camera.Set();
	m_Shadow.Draw();
	m_Player.Draw();
	m_Bullet.Draw();
	m_Enemy.Draw();
}
