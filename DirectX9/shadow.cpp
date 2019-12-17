/*=============================================================================

	[shadow.cpp]
	Author : �o���đ�

=============================================================================*/

#include "main.h"
#include "shadow.h"

// �}�N����`
#define	SHADOW_SIZE_X		(50.0f)		//	�@�e�̕�
#define	SHADOW_SIZE_Z		(50.0f)		//	�e�̍���
#define SHADOW_NUM_VERTEX	(4)			//	���_��
#define SHADOW_NUM_POLYGON	(2)			//	�|���S����
#define	MAX_SHADOW			(128)		//	�e�ő吔

// �O���[�o���ϐ�
Shadow					g_aShadow[MAX_SHADOW];		// �e���[�N

//	�X�^�e�B�b�N�ϐ�
LPDIRECT3DVERTEXBUFFER9 Shadow::m_pVtxBuffShadow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

// ����������
HRESULT Shadow::Init()
{
	m_pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexShadow(m_pDevice);

	// �e�N�X�`���̓ǂݍ���
	m_texture.Load("asset/TEXTURE/shadow000.jpg", 2);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].m_pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_aShadow[nCntShadow].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aShadow[nCntShadow].m_bUse = false;
	}
	return S_OK;
}

// �I������
void Shadow::Uninit()
{
	m_texture.Unload(2);
	SAFE_RELEASE(m_pVtxBuffShadow);
}

// �X�V����
void Shadow::Update()
{

}

// �`�揈��
void Shadow::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	///	<summary>
	///	���Z�����A�T�|�[�g����Ă��Ȃ��ꍇ������B�v�����^�[�Ɠ�������
	///	���u�����g�ł�邵���Ȃ����A�^�����ȉ摜�����Z��������΁A�������Ƃ��ł���B
	/// </summary>
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	//	���ʁ@=�@�]����(DEST)�@�|�@�]����(SRC)
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//	Z��r�Ȃ�
	m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].m_bUse)
		{
			//	���[���h�}�g���N�X��������
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].m_mtxWorld);

			//	�X�P�[�����f
			D3DXMatrixScaling(&mtxScl, g_aShadow[nCntShadow].m_scl.x, g_aShadow[nCntShadow].m_scl.y, g_aShadow[nCntShadow].m_scl.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].m_mtxWorld, &g_aShadow[nCntShadow].m_mtxWorld, &mtxScl);

			//	��]
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].m_rot.y, g_aShadow[nCntShadow].m_rot.x, g_aShadow[nCntShadow].m_rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].m_mtxWorld, &g_aShadow[nCntShadow].m_mtxWorld, &mtxRot);

			//	�ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aShadow[nCntShadow].m_pos.x, g_aShadow[nCntShadow].m_pos.y, g_aShadow[nCntShadow].m_pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].m_mtxWorld, &g_aShadow[nCntShadow].m_mtxWorld, &mtxTranslate);

			//	���[���h�}�g���b�N�X�̐ݒ�
			m_pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].m_mtxWorld);

			//	���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			//	�����i���_�o�b�t�@��ݒ肷��X�g���[��ID�A��2����:���_�o�b�t�@�ւ̃|�C���^�A�X�g���[���̐擪���璸�_�f�[�^�̐擪�܂ł̃I�t�Z�b�g�A���_�f�[�^�̈���̃f�[�^�T�C�Y���o�C�g�P�ʂŎw��j
			m_pDevice->SetStreamSource(0, m_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//	���_�t�H�[�}�b�g��ݒ�
			m_pDevice->SetFVF(FVF_VERTEX3D);

			//	�e�N�X�`���̐ݒ�
			m_pDevice->SetTexture(0,m_texture.Set(2));

			//	�|���S���̕`��
			//	�����i�v���~�e�B�u�^�C�v�A�z��̓ǂݎ��J�n�ʒu�A�O�p�|���S���̐��j
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, SHADOW_NUM_POLYGON);
		}
	}

	//	�ʏ�u�����h
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	//	���ʁ@=�@�]����(SRC) + �]����(DEST)
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//	Z��r����
	m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

// �e�̍쐬
int Shadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scl)
{
	int nIdxShadow = -1;

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (!g_aShadow[nCntShadow].m_bUse)
		{
			g_aShadow[nCntShadow].m_pos = pos;
			g_aShadow[nCntShadow].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aShadow[nCntShadow].m_scl = scl;
			g_aShadow[nCntShadow].m_bUse = true;

			nIdxShadow = nCntShadow;
			break;
		}
	}

	return nIdxShadow;
}

// �e�̔j��
void Shadow::Release(int nIdxShadow)
{
	if (nIdxShadow >= 0 && nIdxShadow < MAX_SHADOW)
	{
		g_aShadow[nIdxShadow].m_bUse = false;
	}
}

// �ʒu�̐ݒ�
void Shadow::SetPosition(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].m_pos = pos;
}

// �X�P�[���̐ݒ�
void Shadow::SetScale(int nIdxShadow, D3DXVECTOR3 scl)
{
	g_aShadow[nIdxShadow].m_scl = scl;
}

// ���_���̍쐬
HRESULT Shadow::MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SHADOW_NUM_VERTEX,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX3D,							// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&m_pVtxBuffShadow,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))									// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
		pVtx[1].pos = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
		pVtx[2].pos = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);
		pVtx[3].pos = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffShadow->Unlock();
	}

	return S_OK;
}

