/*===================================================

	[SceneGame.h]
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
#include "billboard.h"
#include "shadow.h"
#include "bullet.h"

class SceneGame : public SceneBase
{
private:
	Camera			m_Camera;
	Player			m_Player;
	Field			m_Field;
	Bullet			m_Bullet;
	Shadow			m_Shadow;
	Enemy			m_Enemy;

public:
	void Init();		//	����������
	void Uninit();		//	�I������
	void Update();		//	�X�V����
	void Draw();		//	�`�揈��
};

