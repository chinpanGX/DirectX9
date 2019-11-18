
#include "light.h"
#include "main.h"

//!	���C�g�̎�ށ@p110

#define	NUM_LIGHT		(3)		// ���C�g�̐�(9�R�܂�)

// �O���[�o���ϐ�
D3DLIGHT9 g_aLight[NUM_LIGHT];		//	���C�g���
LPDIRECT3DDEVICE9 Light::pDevice;	//	�f�o�C�X�̎擾
D3DXVECTOR3 Light::vecDir;			//	���C�g�̕���

// ���C�g�̏���������
void Light::Init()
{
	pDevice = GetD3DDevice();		//�f�o�C�X�̎擾
	
	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�	(�f�B���N�V���i�����C�g)
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ����
	g_aLight[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.20f, -0.80f, 0.40f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, &g_aLight[0]);

	// ���C�g��L����
	pDevice->LightEnable(0, TRUE);


	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	g_aLight[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// ����
	g_aLight[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

    // ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(1, &g_aLight[1]);

	// ���C�g��L����
	pDevice->LightEnable(1, TRUE);	//���C�g�̐�

	// ���C�e�B���O���[�h�L���c�V�F�[�f�B���O����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

// ���C�g�̏I������
void Light::Uninit()
{
	DEVICE_RELEASE(pDevice);
}

// ���C�g�̍X�V����
void Light::Update()
{
}


