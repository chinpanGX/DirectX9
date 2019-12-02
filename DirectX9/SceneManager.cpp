/*===========================================

	[SceneManager.cpp]
	Author : �o���đ�

============================================*/

#include "main.h"
#include "SceneManager.h"

//	�X�^�e�B�b�N�ϐ�
SceneBase			*SceneManager::m_Scene;	//	�V�[���̃C���X�^���X���i�[
LPDIRECT3DDEVICE9	SceneManager::m_pDevice;	//	�f�o�C�X

//	����������
void SceneManager::Init()
{
	m_pDevice = GetD3DDevice();
	//	�������̊m��
	m_Scene = new SceneGame;	//	SceneGame�̃C���X�^���X
	m_Scene->Init();			//	Init()���Ăяo��
}

//	�I������
void SceneManager::Uninit()
{
	m_Scene->Uninit();	//	Uninit()���Ăяo��
	
	//	�������̉��
	delete m_Scene;		
}

//	�X�V����
void SceneManager::Update()
{
	m_Scene->Update();	//	Update()���Ăяo��
}

//	�`�揈��
void SceneManager::Draw()
{
	m_Scene->Draw();	//	Draw()���Ăяo��
}
