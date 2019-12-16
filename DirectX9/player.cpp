/*=============================================================================

	[cpp]
	Author : �o���đ�

=============================================================================*/
#include "Player.h"
#include "camera.h"
#include "Controller.h"
#include "shadow.h"
#include "bullet.h"

// �}�N����`
#define	MODEL_CAR			"asset/MODEL/car000.x"	// �ǂݍ��ރ��f����
#define	VALUE_MOVE_MODEL	(0.5f)					// �ړ����x
#define	RATE_MOVE_MODEL		(0.2f)					// �ړ������W��
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)		// ��]���x
#define	RATE_ROTATE_MODEL	(0.2f)					// ��]�����W��

//	�X�^�e�B�b�N�ϐ�
LPDIRECT3DTEXTURE9	Player::m_pTextureModel = NULL;		// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			Player::m_pMeshModel	= NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		Player::m_pBuffMatModel	= NULL;		// �}�e���A�����ւ̃|�C���^
DWORD				Player::m_nNumMatModel;				// �}�e���A�����̑���

//	����������
HRESULT Player::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot )
{
	m_pDevice = GetD3DDevice();

	//�ʒu�E�����E�ړ��̏����ݒ�
	m_pos = pos;
	m_rot = rot;
	m_rotDest = rot;
	m_dir = 3;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,D3DXMESH_SYSTEMMEM,m_pDevice,NULL,&m_pBuffMatModel,NULL,&m_nNumMatModel,&m_pMeshModel)))
	{
		return E_FAIL;
	}

	//	�e�̍쐬
	m_IdxShadow = m_shadow.Create(m_pos, D3DXVECTOR3(0.7f,0.7f,0.7f));

	return S_OK;
}

void Player::Uninit()
{
	m_shadow.Release(m_IdxShadow);
	SAFE_RELEASE(m_pTextureModel);
	SAFE_RELEASE(m_pBuffMatModel);
	SAFE_RELEASE(m_pMeshModel);
}

void Player::Update()
{
	//	�����x�N�g���̏�����
	D3DXVECTOR3 dir = D3DXVECTOR3(0.0f,0.0f,0.0f); 

	// �J�����̎擾
	Camera *m_pCamera = GetCamera();
	float fDiffRotY;

	//	�O�ړ�
	if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0, LEFTSTICK_UP))
	{
		dir.z -= -1;
		m_dir = 0;
		m_move.x += sinf(D3DX_PI * 1.0f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_move.z -= cosf(D3DX_PI * 1.0f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_rotDest.y = m_pCamera->rot.y + D3DX_PI * 1.0f;
	}
	//	��ړ�
	else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
	{
		dir.z += -1;
		m_dir = 3;
		m_move.x += sinf(D3DX_PI * 0.0f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_move.z -= cosf(D3DX_PI * 0.0f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_rotDest.y = m_pCamera->rot.y + D3DX_PI * 0.0f;
	}
	//	���ړ�
	else if (KeyBoard::IsPress(DIK_A) || GamePad::IsPress(0,LEFTSTICK_LEFT))
	{
		dir.x += -1;
		m_dir = 1;
		m_move.x += sinf(-D3DX_PI * 0.50f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_move.z -= cosf(-D3DX_PI * 0.50f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_rotDest.y = m_pCamera->rot.y + D3DX_PI * 0.50f;
	}
	//	�E�ړ�
	else if (KeyBoard::IsPress(DIK_D) || GamePad::IsPress(0, LEFTSTICK_RIGHT))
	{
		dir.x -= -1;
		m_dir = 2;
		m_move.x += sinf(D3DX_PI * 0.50f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_move.z -= cosf(D3DX_PI * 0.50f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_rotDest.y = m_pCamera->rot.y - D3DX_PI * 0.50f;
	}

	// �ړI�̊p�x�܂ł̍���
	fDiffRotY = m_rotDest.y - m_rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	m_rot.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	//	�e�𔭎�
    if (Mouse::LeftTrigger())
	{
		///	<summary>
		///	���͂�����Ă��Ȃ��ꍇ�A�����Ă���������甭�˕������쐬
		/// </summary>
		if (D3DXVec3Length(&dir) < 0.01f)
		{
			switch (m_dir)
			{
			case 0:	//	�O����
				dir.z = 1.0f;
				break;
			case 1:	//	������
				dir.x = -1.0f;
				break;
			case 2:	//	�E����
				dir.x = 1.0f;
				break;
			case 3:	//	�����
				dir.z = -1.0f;
				break;
			}
		}
		m_bullet.Create(m_pos.x, 6.5f, m_pos.z, D3DXVECTOR3(dir));
		dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	/// �ʒu�ړ�
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;


	// �ړ��ʂɊ�����������
	m_move.x += (0.0f - m_move.x) * RATE_MOVE_MODEL;
	m_move.z += (0.0f - m_move.z) * RATE_MOVE_MODEL;

	// �͈̓`�F�b�N
#if 0
	if (g_m_pos.x < -310.0f)
	{
		g_m_pos.x = -310.0f;
	}
	if (g_m_pos.x > 310.0f)
	{
		g_m_pos.x = 310.0f;
	}
	if (g_m_pos.z < -310.0f)
	{
		g_m_pos.z = -310.0f;
	}
	if (g_m_pos.z > 310.0f)
	{
		g_m_pos.z = 310.0f;
	}
#endif

	if (KeyBoard::IsPress(DIK_RETURN))
	{// ���Z�b�g
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	D3DXVECTOR3 pos = m_pos;
	pos.y = 0.0f;	//	�e�͍��W���Œ肵�Ă���->�e�̓W�����v���Ȃ�
	m_shadow.SetPosition(m_IdxShadow, pos);
}

void Player::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldModel);

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorldModel, &m_mtxWorldModel, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorldModel, &m_mtxWorldModel, &mtxTranslate);


	//���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldModel);

	//���݂̃}�e���A�����擾
	m_pDevice->GetMaterial(&matDef);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMatModel; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		m_pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
		//�e�N�X�`���̐ݒ�
		m_pDevice->SetTexture(0, m_pTextureModel);
		//�`��
		m_pMeshModel->DrawSubset(nCntMat);

	}
	//�}�e���A�����f�t�H���g�ɖ߂�
	m_pDevice->SetMaterial(&matDef);
}
