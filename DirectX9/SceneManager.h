/*==========================================

	[SceneManager.h]
	Author : �o���đ�

============================================*/

#pragma once
#include "main.h"
#include "SceneGame.h"

class SceneManager
{
private:
	static SceneBase			*m_Scene;	//	�V�[���̃C���X�^���X
	static LPDIRECT3DDEVICE9	m_pDevice;	//	�f�o�C�X
public:
	static void Init();						//	����������
	static void Uninit();					//	�I������
	static void Update();					//	�X�V����
	static void Draw();						//	�`�揈��
};

