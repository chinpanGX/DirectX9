//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
//
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_CAR			"asset/MODEL/car001.x"	// �ǂݍ��ރ��f����
#define	VALUE_MOVE_MODEL	(0.50f)					// �ړ����x
#define	RATE_MOVE_MODEL		(0.20f)					// �ړ������W��
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)		// ��]���x
#define	RATE_ROTATE_MODEL	(0.20f)					// ��]�����W��

//	�O���[�o���ϐ�
Enemy g_Enemy[MAX_ENEMY];

// �X�^�e�B�b�N�ϐ�
LPDIRECT3DTEXTURE9	Enemy::m_pTextureModel;	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			Enemy::m_pMeshModel;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		Enemy::m_pBuffMatModel;	// �}�e���A�����ւ̃|�C���^
DWORD				Enemy::m_nNumMatModel;		// �}�e���A�����̑���


HRESULT Enemy::Init()
{
	m_pDevice = GetD3DDevice();

	m_pTextureModel = NULL;
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;

	float offset = (50.0f * MAX_ENEMY / 2) - 25.0f;

	// �ʒu�E�����E�ړ��ʂ̏����ݒ�
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].m_pos = D3DXVECTOR3(50.0f*i - offset, 0.0f, 50.0f);
		g_Enemy[i].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�����蔻��p�X�t�B�A�̔��a
		//g_Enemy[i].col_circle.r = 10.0f;

		//	AABB    
		g_Enemy[i].m_aabb.cx = 0.0f;
		g_Enemy[i].m_aabb.cy = 0.0f;
		g_Enemy[i].m_aabb.cz = 0.0f;
		g_Enemy[i].m_aabb.sx = 10.0f;
		g_Enemy[i].m_aabb.sy = 10.0f;
		g_Enemy[i].m_aabb.sz = 10.0f;

		//�e�̍쐬
		//g_Enemy[i].idxShadow = m_Shadow.Create(g_Enemy[i].posModel, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		g_Enemy[i].m_bUse = true;
	}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(m_pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,	// �t�@�C���̖��O
		&g_pTextureModel);	// �ǂݍ��ރ������[
#endif

	// X�t�@�C���̓ǂݍ���
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

	return S_OK;
}

void Enemy::Uninit()
{
	//�e�̉������
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//m_Shadow.Release(g_Enemy[i].idxShadow);
	}
	
	SAFE_RELEASE(m_pTextureModel);
	SAFE_RELEASE(m_pMeshModel);
	SAFE_RELEASE(m_pBuffMatModel);
}

void Enemy::Update()
{
	//�����蔻��p���W�X�V
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//�L���t���O���`�F�b�N
		if (g_Enemy[i].m_bUse == false)
			continue;
		/*
		g_Enemy[i].col_circle.cx = g_Enemy[i].posModel.x;
		g_Enemy[i].col_circle.cy = g_Enemy[i].posModel.y;
		g_Enemy[i].col_circle.cz = g_Enemy[i].posModel.z;
		]*/
		//	AABB
		g_Enemy[i].m_aabb.cx = g_Enemy[i].m_pos.x;
		g_Enemy[i].m_aabb.cy = g_Enemy[i].m_pos.y;
		g_Enemy[i].m_aabb.cz = g_Enemy[i].m_pos.z;
	}
}

void Enemy::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//�L���t���O���`�F�b�N
		if (g_Enemy[i].m_bUse == false)
			continue;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Enemy[i].m_mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[i].m_rot.y, g_Enemy[i].m_rot.x, g_Enemy[i].m_rot.z);
		D3DXMatrixMultiply(&g_Enemy[i].m_mtxWorld, &g_Enemy[i].m_mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy[i].m_pos.x, g_Enemy[i].m_pos.y, g_Enemy[i].m_pos.z);
		D3DXMatrixMultiply(&g_Enemy[i].m_mtxWorld, &g_Enemy[i].m_mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		m_pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[i].m_mtxWorld);

		// ���݂̃}�e���A�����擾
		m_pDevice->GetMaterial(&matDef);

		// �}�e���A�����ɑ΂���|�C���^���擾
		pD3DXMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();


		for (int nCntMat = 0; nCntMat < (int)m_nNumMatModel; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			m_pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			m_pDevice->SetTexture(0, m_pTextureModel);

			// �`��
			m_pMeshModel->DrawSubset(nCntMat);
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		m_pDevice->SetMaterial(&matDef);
	}
}

bool Enemy::IsEnable(int index)
{
	return false;
}

CIRCLE * Enemy::GetCircleCollision(int index)
{
	return &g_Enemy[index].m_circle;
}

AABB * Enemy::GetAABBCollision(int index)
{
	return &g_Enemy[index].m_aabb;
}

Enemy * Enemy::GetEnemy()
{
	return &(g_Enemy[0]);
}
