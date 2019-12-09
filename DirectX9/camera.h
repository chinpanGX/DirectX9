/*-------------------------------------------------------

	[camera.h]
	Author : �o���đ��@

--------------------------------------------------------*/
#pragma once

#include "main.h"

//	Camera�N���X
class Camera
{
private:
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	float fDistance;
	static D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	static LPDIRECT3DDEVICE9 pDevice;
public:
	static D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
	D3DXVECTOR3 rot;
public:
	void Init();	//	����������
	void Uninit();	//	�I������
	void Update();	//	�X�V����			
	void Set();		//	�`�揈��

	//Camera* GetCamera();	//�Q�b�^�[
};

Camera* GetCamera();	//�Q�b�^�[