/*=================================================

	[texture.h]
	Author : �o���đ�

=================================================*/

#pragma once
#include "main.h"

//	Texture�N���X
class  Texture
{
private:
	LPDIRECT3DTEXTURE9	m_pTexture[5];	//	�e�N�X�`�����i�[����ϐ�
public:
	void Load(const char *filename,int i);		//	�e�N�X�`���̃��[�h		�i�e�N�X�`�����A�i�[����z��ԍ��j
	void Unload(int i);							//	�e�N�X�`���̃A�����[�h	�i�e�N�X�`�����A�A�����[�h����z��ԍ��j
	LPDIRECT3DTEXTURE9 Set(int i);				//	�e�X�N�`���̃Z�b�g	�i�Z�b�g����z��ԍ��j
};
