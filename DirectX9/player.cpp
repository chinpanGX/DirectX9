/*=============================================================================

	[cpp]
	Author : �o���đ�

=============================================================================*/
#include "Player.h"
#include "camera.h"
#include "Controller.h"
#include "shadow.h"

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


HRESULT Player::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pDevice = GetD3DDevice();

	//�ʒu�E�����E�ړ��̏����ݒ�
	posModel = pos;
	rotModel = rot;
	rotDestModel = rot;
	moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,
		D3DXMESH_SYSTEMMEM,
		m_pDevice,
		NULL,
		&m_pBuffMatModel,
		NULL,
		&m_nNumMatModel,
		&m_pMeshModel)))
	{
		return E_FAIL;
	}

	//	�e�̍쐬
	m_IdxShadow = m_shadow.Create(posModel, 0.1f, 0.1f);

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
	Camera *pCamera;
	float fDiffRotY;

	// �J�����̎擾
	pCamera = GetCamera();

	if (KeyBoard::IsPress(DIK_A) || GamePad::IsPress(0,LEFTSTICK_LEFT))
	{
		if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0,LEFTSTICK_UP))
		{// �����ړ�
			moveModel.x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
		{// ����O�ړ�
			moveModel.x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(-D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		else
		{// ���ړ�
			moveModel.x += sinf(-D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(-D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.50f;
		}
	}
	else if (KeyBoard::IsPress(DIK_D) || GamePad::IsPress(0, LEFTSTICK_RIGHT))
	{
		if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0, LEFTSTICK_UP))
		{// �E���ړ�
			moveModel.x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
		{// �E��O�ړ�
			moveModel.x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		else
		{// �E�ړ�
			moveModel.x += sinf(D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.50f;
		}
	}
	else if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0, LEFTSTICK_UP))
	{// �O�ړ�
		moveModel.x += sinf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		moveModel.z -= cosf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;

		rotDestModel.y = pCamera->rot.y + D3DX_PI * 1.0f;
	}
	else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
	{// ��ړ�
		moveModel.x += sinf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		moveModel.z -= cosf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;

		rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.0f;
	}

	if (KeyBoard::IsPress(DIK_Q) || GamePad::IsPress(0,PS4RIGHTSTICK_LEFT))
	{// ����]
		rotDestModel.y -= VALUE_ROTATE_MODEL;
		if (rotDestModel.y < -D3DX_PI)
		{
			rotDestModel.y += D3DX_PI * 2.0f;
		}
	}
	if (KeyBoard::IsPress(DIK_E) || GamePad::IsPress(0,PS4RIGHTSTICK_RIGHT))
	{// �E��]
		rotDestModel.y += VALUE_ROTATE_MODEL;
		if (rotDestModel.y > D3DX_PI)
		{
			rotDestModel.y -= D3DX_PI * 2.0f;
		}
	}

	// �ړI�̊p�x�܂ł̍���
	fDiffRotY = rotDestModel.y - rotModel.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	rotModel.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (rotModel.y > D3DX_PI)
	{
		rotModel.y -= D3DX_PI * 2.0f;
	}
	if (rotModel.y < -D3DX_PI)
	{
		rotModel.y += D3DX_PI * 2.0f;
	}

	/// �ʒu�ړ�
	posModel.x += moveModel.x;
	posModel.z += moveModel.z;

	// �ړ��ʂɊ�����������
	moveModel.x += (0.0f - moveModel.x) * RATE_MOVE_MODEL;
	moveModel.z += (0.0f - moveModel.z) * RATE_MOVE_MODEL;

#if 0
	// �͈̓`�F�b�N
	if (g_posModel.x < -310.0f)
	{
		g_posModel.x = -310.0f;
	}
	if (g_posModel.x > 310.0f)
	{
		g_posModel.x = 310.0f;
	}
	if (g_posModel.z < -310.0f)
	{
		g_posModel.z = -310.0f;
	}
	if (g_posModel.z > 310.0f)
	{
		g_posModel.z = 310.0f;
	}
#endif

	if (KeyBoard::IsPress(DIK_RETURN))
	{// ���Z�b�g
		posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	D3DXVECTOR3 pos = posModel;
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
	D3DXMatrixIdentity(&mtxWorldModel);

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotModel.y, rotModel.x, rotModel.z);
	D3DXMatrixMultiply(&mtxWorldModel, &mtxWorldModel, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, posModel.x, moveModel.y, posModel.z);
	D3DXMatrixMultiply(&mtxWorldModel, &mtxWorldModel, &mtxTranslate);


	//���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &mtxWorldModel);

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

D3DXVECTOR3 Player::GetPos()
{
	return posModel;
}
