/*==============================================
	
	[Camera.h]
	Author : �o���đ�

==============================================*/

#pragma once
#include "main.h"

//�@�}�N����`
#define	CAM_POS_V_X		(0.0f)											// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_V_Y		(100.0f)										// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_V_Z		(-200.0f)										// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X		(0.0f)											// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y		(0.0f)											// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z		(0.0f)											// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE		(D3DXToRadian(70.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l	�l���߂��ƕ`�悵�Ȃ�
#define	VIEW_FAR_Z		(10000.0f)										// �r���[���ʂ�FarZ�l	�l��艓���ƕ`�悵�Ȃ�
#define MOVE_CAMERA		(1.0f)
#define ROTATE_CAMERA	(D3DX_PI * 0.01f)
#define CAMERADISTANCE	(1.0f)

//# Camera
/* �f�o�b�O�p�J���� */
class Camera
{
private:
	//�����o�ϐ�
	static D3DXVECTOR3			posV;			//	���_�i�J�����̈ʒu�j
	static D3DXVECTOR3			posR;			//	�����_�i�J�����͂ǂ������Ă���̂��j
	static D3DXVECTOR3			vecU;			//	������x�N�g���i�J�����̏�����͂����v���X�j���W�́i�O�C�P�C�O�j
	static D3DXVECTOR3			rot;
	static float				fDistance;
	static D3DXMATRIX			mtxProjection;	//	�v���W�F�N�V�����}�g���b�N�X
	static D3DXMATRIX			mtxView;		//	�r���[�}�g���b�N�X
	static LPDIRECT3DDEVICE9	pDevice;		//�@�f�o�C�X�̎擾�p�ϐ�

public:
	//�@�����o�֐�
	static void Init();			//	�J�����̏�����
	static void Uninit();		//	�J�����̏I������]
	static void Update();		//	�J�����̍X�V
	static void Set();			//	�J�����̐ݒ�
};

