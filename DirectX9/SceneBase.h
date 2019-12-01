/*===================================================

	[SceneBase.h]
	Author : �o���đ�
	
===================================================*/

#pragma once

class SceneBase
{
	//	�������z�֐��w��
public:
	virtual void Init() = 0;	//	����������
	virtual void Uninit() = 0;	//	�I������
	virtual void Update() = 0;	//	�X�V����
	virtual void Draw() = 0;	//	�`�揈��
};

