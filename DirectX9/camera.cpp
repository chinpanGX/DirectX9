
#include "camera.h"
#include "debugproc.h"
#include "Controller.h"

// �}�N����`
#define	CAM_POS_V_X		(0.0f)											//	�J�����̎��_�����ʒu(X���W)
#define	CAM_POS_V_Y		(200.0f)										//	�J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_V_Z		(-300.0f)										//	�J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X		(0.0f)											//	�J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y		(0.0f)											//	�J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z		(0.0f)											//	�J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							//	�r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	//	�r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z		(10.0f)											//	�r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(10000.0f)										//	�r���[���ʂ�FarZ�l
#define VALUE_MOVE_CAMERA (2.5f)										//	�J�����̈ړ���
#define VALUE_ROTATE_CAMERA (D3DX_PI * 0.01f)							//	�J�����̉�]��
#define VALUE_ADUJST_DINSTANCE   (2.0f)									//	���_�ƒ����_�Ԃ̈ړ���

//	�O���[�o���ϐ�
Camera g_camera;		// �J�������

//	�X�^�e�B�b�N�ϐ�affa
LPDIRECT3DDEVICE9	Camera::pDevice;
D3DXMATRIX			Camera::mtxProjection;
D3DXMATRIX			Camera::mtxView;

//	�J�����̏�����
void Camera::Init()
{
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_camera.fDistance = 2.0f;
	float fVecX, fVecZ;
	fVecX = g_camera.posV.x - g_camera.posR.x;
	fVecZ = g_camera.posV.z - g_camera.posR.z;
	g_camera.fDistance = sqrtf(fVecX*fVecX + fVecZ * fVecZ);
}

//	�J�����̏I������
void Camera::Uninit()
{
}

//	�J�����̍X�V����
void Camera::Update()
{

	if (KeyBoard::IsPress(DIK_A) || GamePad::IsPress(0,LEFTSTICK_LEFT))
	{
		//���ړ�
		g_camera.posV.x += sinf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (KeyBoard::IsPress(DIK_D) || GamePad::IsPress(0,LEFTSTICK_RIGHT))
	{
		//�E�ړ�
		g_camera.posV.x += sinf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0, LEFTSTICK_UP))
	{
		//�O�ړ�
		g_camera.posV.x += sinf(-D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(-D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
	{
		//���ړ�
		g_camera.posV.x += sinf(-D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(-D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
#if 0
	if (KeyBoard::IsPress(DIK_Z))
	{//���_����u���v
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (KeyBoard::IsPress(DIK_C))
	{//���_����u�E�v
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y < D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	if (KeyBoard::IsPress(DIK_Y))
	{//���_�ړ��u��v
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if (KeyBoard::IsPress(DIK_N))
	{//���_�ړ��u���v
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}

	if (KeyBoard::IsPress(DIK_Q))
	{//�����_����u���v
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (KeyBoard::IsPress(DIK_E))
	{//�����_����u�E�v
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y < D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (KeyBoard::IsPress(DIK_T))
	{//�����_�ړ��u��v
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if (KeyBoard::IsPress(DIK_B))
	{//�����_�ړ��u���v
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}
	if (KeyBoard::IsPress(DIK_U))
	{//���_�ƒ����_�̋������k�߂�
		g_camera.fDistance -= VALUE_ADUJST_DINSTANCE;
		if (g_camera.fDistance < 300.0f)
		{
			g_camera.fDistance = 300.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (KeyBoard::IsPress(DIK_M))
	{//���_�ƒ����_�̋�����L�΂�
		g_camera.fDistance += VALUE_ADUJST_DINSTANCE;
		if (g_camera.fDistance > 900.0f)
		{
			g_camera.fDistance = 900.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	if (KeyBoard::IsTrigger(DIK_RETURN))
	{//���Z�b�g
		g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
		g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_Z, CAM_POS_R_Z);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		float fVecX, fVecZ;
		fVecX = g_camera.posV.x - g_camera.posR.x;
		fVecZ = g_camera.posV.z - g_camera.posR.z;
		g_camera.fDistance = sqrtf(fVecX*fVecX + fVecZ * fVecZ);
	}
#endif
	//	������̕`��
	DebugProc::Print((char*)"[�J�����̎��_:(%f:%f:%f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
	DebugProc::Print((char*)"[�J�����̒����_:(%f:%f:%f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
	DebugProc::Print((char*)"[�J�����̌���:(%f)]\n", g_camera.rot.y);
	DebugProc::Print((char*)"[�J�����̋���:(%f)]\n", g_camera.fDistance);
	DebugProc::Print((char*)"\n");
}   

void Camera::Set()
{
	pDevice = GetD3DDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		VIEW_ANGLE,				// �r���[���ʂ̎���p
		VIEW_ASPECT,			// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,			// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);			// �r���[���ʂ�FarZ�l

// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,		// �J�����̎��_
		&g_camera.posR,		// �J�����̒����_
		&g_camera.vecU);	// �J�����̏�����x�N�g��

// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

Camera* GetCamera()
{
	return &g_camera;
}
