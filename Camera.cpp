/*======================================================

	[Camera.cpp]
	Author : �o���đ�

======================================================*/

#include "main.h"
#include "Camera.h"
#include "input.h"
#include "debugproc.h"
  
D3DXVECTOR3			Camera::posV;
D3DXVECTOR3			Camera::posR;
D3DXVECTOR3			Camera::vecU;
D3DXVECTOR3			Camera::move;
D3DXVECTOR3			Camera::rot;
float				Camera::fDistance;
D3DXMATRIX			Camera::mtxProjection;
D3DXMATRIX			Camera::mtxView;
LPDIRECT3DDEVICE9	Camera::pDevice;

//#	�J�����̏�����
void Camera::Init()
{
	posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);	//�J�����ʒu
	posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);	//�����_
	vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						//�J�����̏�x�N�g��
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move = D3DXVECTOR3(5.0f, 5.0f, 5.0f);

	float x, z;
	x = posV.x - posR.x;
	z = posV.z - posR.z;
	fDistance = sqrtf(x * x + z * z);
}

//#	�J�����̏I������
void Camera::Uninit()
{

}

//# �J�����̍X�V����
void Camera::Update()
{
	/*
	if (Input::KB_IsPress(DIK_A))
	{
		if (Input::KB_IsPress(DIK_W))
		{
			posV.x += sinf(-D3DX_PI * 0.75f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(-D3DX_PI * 0.75f - rot.y) * MOVE_CAMERA;
		}
		else if (Input::KB_IsPress(DIK_S))
		{
			posV.x += sinf(-D3DX_PI * 0.25f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(-D3DX_PI * 0.25f - rot.y) * MOVE_CAMERA;
		}
		else
		{
			posV.x += sinf(-D3DX_PI * 0.50f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(-D3DX_PI * 0.50f - rot.y) * MOVE_CAMERA;
		}
		posR.x = posV.x + sinf(rot.y) * fDistance;
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}
	
	else if (Input::KB_IsPress(DIK_D))
	{
		if (Input::KB_IsPress(DIK_W))
		{
			posV.x += sinf(D3DX_PI * 0.75f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(D3DX_PI * 0.75f - rot.y) * MOVE_CAMERA;
		}
		else if (Input::KB_IsPress(DIK_S))
		{
			posV.x += sinf(D3DX_PI * 0.25f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(D3DX_PI * 0.25f - rot.y) * MOVE_CAMERA;
		}
		else
		{
			posV.x += sinf(D3DX_PI * 0.50f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(D3DX_PI * 0.50f - rot.y) * MOVE_CAMERA;
		}
		posR.x = posV.x + sinf(rot.y) * fDistance; 
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}
	
	if (Input::KB_IsPress(DIK_W))
	{
		posV.x += sinf(D3DX_PI * 1.0f - rot.y) * MOVE_CAMERA;
		posV.z -= cosf(D3DX_PI * 1.0f - rot.y) * MOVE_CAMERA;

		posR.x = posV.x + sinf(rot.y) * fDistance;
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}
	
	if (Input::KB_IsPress(DIK_S))
	{
		posV.x += sinf(D3DX_PI * 0.0f - rot.y) * MOVE_CAMERA;
		posV.z -= cosf(D3DX_PI * 0.0f - rot.y) * MOVE_CAMERA;

		posR.x = posV.x + sinf(rot.y) * fDistance;
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}
	*/
	if (Input::KB_IsPress(DIK_S))
	{
		posV.z += move.z;
		posR.z += move.z;
	}

	if (Input::KB_IsPress(DIK_W))
	{
		posR.z -= move.z;
		posV.z -= move.z;
	}

	if (Input::KB_IsPress(DIK_A))
	{
		posV.x += move.x;
		posR.x += move.x;
	}

	if (Input::KB_IsPress(DIK_D))
	{
		posV.x -= move.x;
		posR.x -= move.x;
	}
	if (Input::KB_IsPress(DIK_Z))
	{
		rot.y += ROTATE_CAMERA;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		posR.x = posV.x - sinf(rot.y) * fDistance;
		posR.z = posV.z - cosf(rot.y) * fDistance;
	}
	
	if (Input::KB_IsPress(DIK_C))
	{
		rot.y -= ROTATE_CAMERA;
		if (rot.y < D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		posR.x = posV.x - sinf(rot.y) * fDistance;
		posR.z = posV.z - cosf(rot.y) * fDistance;
	}

	if (Input::KB_IsPress(DIK_Y))
	{
		posV.y += MOVE_CAMERA;
	}
	if (Input::KB_IsPress(DIK_N))
	{
		posV.y -= MOVE_CAMERA;
	}

	if (Input::KB_IsPress(DIK_E))
	{
		rot.y += ROTATE_CAMERA;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		posR.x = posV.x - sinf(rot.y) * fDistance;
		posR.z = posV.z - cosf(rot.y) * fDistance;
	}
	if (Input::KB_IsPress(DIK_Q))
	{
		rot.y -= ROTATE_CAMERA;
		if (rot.y < D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		posR.x = posV.x + sinf(rot.y) * fDistance;
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}

	if (Input::KB_IsPress(DIK_T))
	{
		posR.y += MOVE_CAMERA;
	}
	if (Input::KB_IsPress(DIK_B))
	{
		posR.y -= MOVE_CAMERA;
	}


	if (Input::KB_IsPress(DIK_U))
	{
		fDistance -= CAMERADISTANCE;
		if (fDistance < 100.0f)
		{
			fDistance = 100.0f;
		}
		posV.x = posR.x + sinf(rot.y) * fDistance;
		posV.z = posR.z + cosf(rot.y) * fDistance;
	}

	if (Input::KB_IsPress(DIK_M))
	{
		fDistance += CAMERADISTANCE;
		if (fDistance > 1000.0f)
		{
			fDistance = 1000.0f;
		}
		posV.x = posR.x + sinf(rot.y) * fDistance;
		posV.z = posR.z + cosf(rot.y) * fDistance;
	}


	if (Input::KB_IsPress(DIK_SPACE))
	{
		posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);	//	�J�����ʒu
		posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);	//	�����_
		vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						//	�J�����̏�x�N�g��

		float x, z;
		x = posV.x - posR.x;
		z = posV.z - posR.z;
		fDistance = sqrtf(x * x + z * z);
	}

	DebugProc_Print((char*)"�J�����̎��_ [%f : %f : %f]\n", posV.x, posV.y, posV.z);
	DebugProc_Print((char*)"�J�����̒����_ [%f : %f : %f]\n", posR.x, posR.y, posR.z);
	DebugProc_Print((char*)"�J�����̌��� [%f]\n", rot.y);
	DebugProc_Print((char*)"�J�����̋���[%f]\n", fDistance);
	DebugProc_Print((char*)"\n");
}

//#	�J�����̐ݒ�
void Camera::Set()
{
	//�f�o�C�X�̎擾
	pDevice = GetD3DDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,				// �r���[���ʂ̎���p
		VIEW_ASPECT,			// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,			// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);			// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);	//	�P�ʍs��:�s��̏���������Ƃ��Ɏg��

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&mtxView,
		&posV,		// �J�����̎��_
		&posR,		// �J�����̒����_
		&vecU);	// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
}
