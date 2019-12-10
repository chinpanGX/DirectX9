/*=======================================================

	[BulletManager.cpp]
	Author : �o���đ�

=========================================================*/

#include "BulletManager.h"

#define	BULLET_NUM_VERTEX	4
#define	BULLET_NUM_POYGON	2

//	�X�^�e�B�b�N�ϐ�
LPDIRECT3DVERTEXBUFFER9	BulletManager::m_pVtxBuffBullet;		//	���_�o�b�t�@�ւ̃|�C���^

HRESULT BulletManager::Init()
{
	m_pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexBullet(m_pDevice);

	/// <summary>
	///	�e�N�X�`���̃��[�h
	/// </summary>
	///	<argument> ("�t�@�C���p�X��",�i�[�z��ԍ�) </argument>
	m_texture.Load("asset/texture/bullet000.png", 1);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		m_aBullet[nCntBullet].m_position = D3DXVECTOR3(0.0f, 18.0f, 0.0f);
		m_aBullet[nCntBullet].m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_aBullet[nCntBullet].m_velocity = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_aBullet[nCntBullet].m_bEnableGravity = true;
		m_aBullet[nCntBullet].m_fSizeX = SIZE_X;
		m_aBullet[nCntBullet].m_fSizeY = SIZE_Y;
		m_aBullet[nCntBullet].m_life = 0;
		m_aBullet[nCntBullet].m_bUse = false;
		m_shadow.Create(m_aBullet[nCntBullet].m_position, m_aBullet[nCntBullet].m_scale);
	}
	m_posBase = D3DXVECTOR3(0.0f,18.0f,0.0f);
	m_roty = 0.0f;
	m_spd = 0.0f;

	return S_OK;
}

void BulletManager::Uninit()
{
	SAFE_RELEASE(m_pVtxBuffBullet);
	//m_texture.Unload(1);
}

void BulletManager::Update()
{
	m_camera = GetCamera();
	m_posBase.x = 0;
	m_posBase.z = 0;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//	�g�p��
		if (m_aBullet[nCntBullet].m_bUse == true)
		{
			m_aBullet[nCntBullet].m_position.x += m_aBullet[nCntBullet].m_velocity.x;
			m_aBullet[nCntBullet].m_position.z += m_aBullet[nCntBullet].m_velocity.z;
			m_aBullet[nCntBullet].m_position.y += m_aBullet[nCntBullet].m_velocity.y;
			
			D3DXVECTOR3 pos = m_aBullet[nCntBullet].m_position;
			pos.y = 0.0f;	//	�e�͍��W���Œ肵�Ă���->�e�̓W�����v���Ȃ�
			m_shadow.SetPosition(m_IdxShadow, pos);

			//	���n
			if (m_aBullet[nCntBullet].m_position.y <= m_aBullet[nCntBullet].m_fSizeY / 2)
			{
				m_aBullet[nCntBullet].m_position.y = m_aBullet[nCntBullet].m_fSizeY / 2;
				m_aBullet[nCntBullet].m_velocity.y = -m_aBullet[nCntBullet].m_velocity.y * 0.75f;
			}

			m_aBullet[nCntBullet].m_velocity.x += (0.0f - m_aBullet[nCntBullet].m_velocity.x) * 0.015f;
			m_aBullet[nCntBullet].m_velocity.y -= 0.25f;
			m_aBullet[nCntBullet].m_velocity.z += (0.0f - m_aBullet[nCntBullet].m_velocity.z) * 0.015f;
			
			m_aBullet[nCntBullet].m_life--;	//	���C�t�̃f�N�������g
			if (m_aBullet[nCntBullet].m_life <= 0)
			{
				m_aBullet[nCntBullet].m_bUse = false;
			}
		}
	}

	//	�e�̔���
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 move;
		float fAngle, fLength;
		int nLife;
		float fSize;

		pos = m_posBase;
		fAngle = (float)(rand() % 628 - 314) / 100.0f;
		fLength = rand() % (int)(5.0f * 200) / 100.0f - 5.0f;
		move.x = sinf(fAngle) * fLength;
		move.y = rand() % 300 / 100.0f + 10.0f;
		move.z = cosf(fAngle) * fLength;
		nLife = rand() % 100 + 150;
		fSize = (float)(rand() % 30 + 20);
		pos.y = fSize / 2;
		SetPosition(pos, move, fSize, fSize, nLife);	// �r���{�[�h�̐ݒ�
	}
}

void BulletManager::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//	���e�X�g��L���ɂ���
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//	���C���e�B���O�𖳌��ɂ���
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (m_aBullet[nCntBullet].m_bUse)
		{

			//	���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&m_mtxWorld);

			//	�r���[�}�g���N�X���擾
			m_pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//	�]�u�s��
			m_mtxWorld._11 = mtxView._11;
			m_mtxWorld._12 = mtxView._21;
			m_mtxWorld._13 = mtxView._31;
			m_mtxWorld._21 = mtxView._12;
			m_mtxWorld._22 = mtxView._22;
			m_mtxWorld._23 = mtxView._32;
			m_mtxWorld._31 = mtxView._13;
			m_mtxWorld._32 = mtxView._23;
			m_mtxWorld._33 = mtxView._33;

			//	�X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, m_aBullet[nCntBullet].m_scale.x, m_aBullet[nCntBullet].m_scale.y, m_aBullet[nCntBullet].m_scale.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

			//	�ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, m_aBullet[nCntBullet].m_position.x, m_aBullet[nCntBullet].m_position.y, m_aBullet[nCntBullet].m_position.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

			//	���[���h�}�g���b�N�X�̐ݒ�
			m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

			//	���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			//	�����i���_�o�b�t�@��ݒ肷��X�g���[��ID�A��2����:���_�o�b�t�@�ւ̃|�C���^�A�X�g���[���̐擪���璸�_�f�[�^�̐擪�܂ł̃I�t�Z�b�g�A���_�f�[�^�̈���̃f�[�^�T�C�Y���o�C�g�P�ʂŎw��j
			m_pDevice->SetStreamSource(0, m_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//	���_�t�H�[�}�b�g��ݒ�
			m_pDevice->SetFVF(FVF_VERTEX3D);

			//	�e�N�X�`���̐ݒ�
			m_pDevice->SetTexture(0, m_texture.Set(1));

			//	�|���S���̕`��
			//	�����i�v���~�e�B�u�^�C�v�A�z��̓ǂݎ��J�n�ʒu�A�O�p�|���S���̐��j
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBullet * 4), BULLET_NUM_POYGON);
		}
	}
	//	���C���e�B���O��L���ɂ���
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//	���e�X�g�𖳌��ɂ���
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT BulletManager::MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BULLET_NUM_VERTEX * MAX_BULLET,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&m_pVtxBuffBullet,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		
		VERTEX_3D *pVtx;
		
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
		
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-20.0f / 2, 20.0f / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20.0f / 2, 20.0f / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-20.0f / 2, -20.0f / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20.0f / 2, -20.0f / 2, 0.0f); 
		
		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		
		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffBullet->Unlock();
	}
	return S_OK;
}

void BulletManager::SetVertexBullet(int nIdx, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdx * 4);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffBullet->Unlock();
	}
}

int BulletManager::SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 velocity, float fSizeX, float fSizeY, int nLife)
{
	int nIdxBullet = -1;
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!m_aBullet[nCntBullet].m_bUse)	//	�󂫏�Ԃ̔z��ԍ���T��
		{
			m_aBullet[nCntBullet].m_position = pos;
			m_aBullet[nCntBullet].m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			m_aBullet[nCntBullet].m_velocity = velocity;
			m_aBullet[nCntBullet].m_fSizeX = fSizeX;
			m_aBullet[nCntBullet].m_fSizeY = fSizeY;
			m_aBullet[nCntBullet].m_life = nLife;
			m_aBullet[nCntBullet].m_bUse = true;
			SetVertexBullet(nCntBullet, fSizeX, fSizeY);	//	���_���W�̎w��
			m_IdxShadow = m_shadow.Create(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			nIdxBullet = nCntBullet;	//	�������z��ԍ�����
			break;
		}
	}
	return nIdxBullet;	//	�z��ԍ������^�[��
}
