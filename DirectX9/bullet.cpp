//=============================================================================
//
// �e���� [bullet.cpp]
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

// �}�N����`
#define	BULLET_SIZE_X		(20.0f)							// �r���{�[�h�̕�
#define	BULLET_SIZE_Y		(20.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_BULLET	(5.0f)							// �ړ����x
#define BULLET_NUM_VERTEX	4
#define BULLET_NUM_POLYGON	2 

//	�O���[�o���ϐ�
Bullet g_Bullet[MAX_BULLET];

//	�X�^�e�B�b�N�ϐ�
LPDIRECT3DVERTEXBUFFER9	Bullet::m_pVtxBuffBullet = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

//	�e�̏���������
HRESULT Bullet::Init()
{
	m_pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexBullet(m_pDevice);

	// �e�N�X�`���̓ǂݍ���
	m_texture.Load("asset/TEXTURE/bullet000.png" ,1);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_Bullet[nCntBullet].m_pos = D3DXVECTOR3(0.0f, 18.0f, 0.0f);
		g_Bullet[nCntBullet].m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[nCntBullet].m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�X�t�B�A  
		/*
		g_Bullet[nCntBullet].col_circle.cx = 0.0f;
		g_Bullet[nCntBullet].col_circle.cy = 0.0f;
		g_Bullet[nCntBullet].col_circle.cz = 0.0f;
		g_Bullet[nCntBullet].col_circle.r = 10.0f; 
		*/
		//	AABB    
		g_Bullet[nCntBullet].col_aabb.cx = 0.0f;
	    g_Bullet[nCntBullet].col_aabb.cy = 0.0f;
		g_Bullet[nCntBullet].col_aabb.cz = 0.0f;
		g_Bullet[nCntBullet].col_aabb.sx = 10.0f;
		g_Bullet[nCntBullet].col_aabb.sy = 10.0f;
		g_Bullet[nCntBullet].col_aabb.sz = 10.0f;

		g_Bullet[nCntBullet].m_bUse = false;
	}

	return S_OK;
}

//	�e�̏I������
void Bullet::Uninit()
{
	m_texture.Unload(1);
	SAFE_RELEASE(m_pVtxBuffBullet);
}

//	�e�̍X�V����
void Bullet::Update()
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_Bullet[nCntBullet].m_bUse == true)
		{
			//���Ԃ��o�߂�����
			g_Bullet[nCntBullet].m_frame++;

			//100�t���[���o�߂ŏ���
			if (g_Bullet[nCntBullet].m_frame > 100)
			{
				//�e�̉������
				m_shadow.Release(g_Bullet[nCntBullet].m_nIdxShadowBullet);
				g_Bullet[nCntBullet].m_bUse = false;
			}

			//���W�̍X�V����
			g_Bullet[nCntBullet].m_pos += g_Bullet[nCntBullet].m_move * VALUE_MOVE_BULLET;

			/*
			//�����蔻��p���W�X�V
			g_Bullet[nCntBullet].col_circle.cx = g_Bullet[nCntBullet].m_pos.x;
			g_Bullet[nCntBullet].col_circle.cy = g_Bullet[nCntBullet].m_pos.y;
			g_Bullet[nCntBullet].col_circle.cz = g_Bullet[nCntBullet].m_pos.z; 
			*/
			//	AABB
			g_Bullet[nCntBullet].col_aabb.cx = g_Bullet[nCntBullet].m_pos.x;
			g_Bullet[nCntBullet].col_aabb.cy = g_Bullet[nCntBullet].m_pos.y;
			g_Bullet[nCntBullet].col_aabb.cz = g_Bullet[nCntBullet].m_pos.z;

			//�e�̈ʒu��e�ɍ��킹��
			D3DXVECTOR3 pos = g_Bullet[nCntBullet].m_pos;
			pos.y = 0.0f;
			m_shadow.SetPosition(g_Bullet[nCntBullet].m_nIdxShadowBullet, pos);
		}
	}
}

//	�e�̕`�揈��
void Bullet::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

#if 1	// �ǂ���Ƀr���{�[�h��`�悵�Ă���̂ŁA�����őΏ�
	// ���e�X�g��L����
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
#endif

	// ���C�e�B���O�𖳌��ɂ���
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//�L���t���O�`�F�b�N
		if (g_Bullet[nCntBullet].m_bUse == false)
			continue;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Bullet[nCntBullet].m_mtxWorldBullet);

		// �r���[�}�g���b�N�X���擾
		m_pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//	�]�u�s��
		g_Bullet[nCntBullet].m_mtxWorldBullet._11 = mtxView._11;
		g_Bullet[nCntBullet].m_mtxWorldBullet._12 = mtxView._21;
		g_Bullet[nCntBullet].m_mtxWorldBullet._13 = mtxView._31;
		g_Bullet[nCntBullet].m_mtxWorldBullet._21 = mtxView._12;	// ��
		g_Bullet[nCntBullet].m_mtxWorldBullet._22 = mtxView._22;	// ���������R�����g�A�E�g����Ɛ��������͖���
		g_Bullet[nCntBullet].m_mtxWorldBullet._23 = mtxView._32;	// ��
		g_Bullet[nCntBullet].m_mtxWorldBullet._31 = mtxView._13;
		g_Bullet[nCntBullet].m_mtxWorldBullet._32 = mtxView._23;
		g_Bullet[nCntBullet].m_mtxWorldBullet._33 = mtxView._33;

		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScale, g_Bullet[nCntBullet].m_scl.x, g_Bullet[nCntBullet].m_scl.y, g_Bullet[nCntBullet].m_scl.z);
		D3DXMatrixMultiply(&g_Bullet[nCntBullet].m_mtxWorldBullet, &g_Bullet[nCntBullet].m_mtxWorldBullet, &mtxScale);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Bullet[nCntBullet].m_pos.x, g_Bullet[nCntBullet].m_pos.y, g_Bullet[nCntBullet].m_pos.z);
		D3DXMatrixMultiply(&g_Bullet[nCntBullet].m_mtxWorldBullet, &g_Bullet[nCntBullet].m_mtxWorldBullet, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		m_pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[nCntBullet].m_mtxWorldBullet);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		m_pDevice->SetStreamSource(0, m_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pDevice->SetFVF(FVF_VERTEX3D);

		// �e�N�X�`���̐ݒ�
		m_pDevice->SetTexture(0, m_texture.Set(1));

		// �|���S���̕`��
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, BULLET_NUM_POLYGON);

	}

	// ���C���e�B���O��L���ɂ���
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//	�e�̐���
void Bullet::Create(float x, float y, float z, D3DXVECTOR3 dir)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_Bullet[nCntBullet].m_bUse == true)
		{
			continue;
		}
		// �e�̍��W�Ɉ����̒l��������
		g_Bullet[nCntBullet].m_pos.x = x;
		g_Bullet[nCntBullet].m_pos.y = y;
		g_Bullet[nCntBullet].m_pos.z = z;

		// �e�̈ړ��������w�肷��
		D3DXVec3Normalize(&dir, &dir);
		g_Bullet[nCntBullet].m_move.x = dir.x;
		g_Bullet[nCntBullet].m_move.y = dir.y;
		g_Bullet[nCntBullet].m_move.z = dir.z;

		// �L���t���[����������
		g_Bullet[nCntBullet].m_frame = 0;

		//�e���쐬����
		g_Bullet[nCntBullet].m_nIdxShadowBullet = m_shadow.Create(g_Bullet[nCntBullet].m_pos, D3DXVECTOR3(0.7f, 0.7f, 0.7f));

		// �e��L���ɂ���
		g_Bullet[nCntBullet].m_bUse = true;

		break;
	}
	
}

//	���_���̐���
HRESULT Bullet::MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BULLET_NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
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
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);

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

//	���_���W�̐ݒ�
void Bullet::SetVertexBullet(float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffBullet->Unlock();
	}
}

bool Bullet::IsEnable(int index)
{
	return g_Bullet[index].m_bUse;
}

CIRCLE * Bullet::GetCollision(int index)
{
	return &g_Bullet[index].col_circle;
}

AABB * Bullet::GetCollisionBox(int index)
{
	return &g_Bullet[index].col_aabb;
}

Bullet * Bullet::GetBullet(void)
{
	return &(g_Bullet[0]);
}
