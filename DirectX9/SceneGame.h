/*===================================================

	[Scene.h]
	Author : �o���đ�

===================================================*/

#pragma once
#include "SceneBase.h"
#include "camera.h"
#include "debugproc.h"
#include "field.h"
#include "light.h"
#include "player.h"
#include "texture.h"
#include "box.h"
#include "Grid.h"

class SceneGame : public SceneBase
{
private:
	Camera	m_Camera;	//	Camera�^�C���X�^���X
	Player	m_Player;	//	Player�^�C���X�^���X
	Field	m_Field;	//	Field�^�C���X�^���X
	Grid	m_Grid;		//	Grig�^�C���X�^���X
	Box		m_Box;		//	Box��t�C���X�^���X

public:
	void Init();		//	����������
	void Uninit();		//	�I������
	void Update();		//	�X�V����
	void Draw();		//	�`�揈��
};

