/*=============================================================================

	[shadow.cpp]
	Author : �o���đ�

=============================================================================*/

#include "main.h"
#include "shadow.h"

// �}�N����`
#define	SHADOW_SIZE_X		(50.0f)							// �e�̕�
#define	SHADOW_SIZE_Z		(50.0f)							// �e�̍���
#define	MAX_SHADOW			(128)							// �e�ő吔

// �O���[�o���ϐ�
Shadow					g_aShadow[MAX_SHADOW];		// �e���[�N
//	�X�^�e�B�b�N�ϐ�
LPDIRECT3DVERTEXBUFFER9  Shadow::m_pVtxBuffShadow;	// ���_�o�b�t�@�ւ̃|�C���^

//	����������
HRESULT Shadow::Init()
{
	m_pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexShadow(m_pDevice);

	// �e�N�X�`���̓ǂݍ���
	m_texture.Load("asset/texture/shadow000.jpg", 4);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].m_pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_aShadow[nCntShadow].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].m_bUse = false;
	}

	return S_OK;
}

//	����������
void Shadow::Uninit()
{
	SAFE_RELEASE(m_pVtxBuffShadow);
	m_texture.Unload(4);
}

void Shadow::Update()
{
}

void Shadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	Shadow *pShadow;

	///	<summary>
	///	���Z����
	///	���̓T�|�[�g����Ă��Ȃ�
	///	���u�����g�ł�邵���Ȃ����A�^�����ȉ摜�����Z��������΁A�������Ƃ��ł���B
	/// </summary>
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	//	���ʁ@=�@�]����(DEST)�@�|�@�]����(SRC)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//	Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	pShadow = &g_aShadow[0];
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pShadow)
	{
		if (pShadow->m_bUse)
		{
			//	���[���h�}�g���N�X��������
			D3DXMatrixIdentity(&pShadow->m_mtxWorld);

			//	��]
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->m_rot.y, pShadow->m_rot.x, pShadow->m_rot.z);
			D3DXMatrixMultiply(&pShadow->m_mtxWorld, &pShadow->m_mtxWorld, &mtxRot);

			//	�ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pShadow->m_pos.x, pShadow->m_pos.y, pShadow->m_pos.z);
			D3DXMatrixMultiply(&pShadow->m_mtxWorld, &pShadow->m_mtxWorld, &mtxTranslate);

			//	���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->m_mtxWorld);

			//	���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			//	�����i���_�o�b�t�@��ݒ肷��X�g���[��ID�A��2����:���_�o�b�t�@�ւ̃|�C���^�A�X�g���[���̐擪���璸�_�f�[�^�̐擪�܂ł̃I�t�Z�b�g�A���_�f�[�^�̈���̃f�[�^�T�C�Y���o�C�g�P�ʂŎw��j
			pDevice->SetStreamSource(0, m_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//	���_�t�H�[�}�b�g��ݒ�
			pDevice->SetFVF(FVF_VERTEX3D);

			//	�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_texture.Set(4));

			//	�|���S���̕`��
			//	�����i�v���~�e�B�u�^�C�v�A�z��̓ǂݎ��J�n�ʒu�A�O�p�|���S���̐��j
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntShadow * 4), 4);
		}
	}

	//	�ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	//	���ʁ@=�@�]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//	Z��r����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//	�쐬����
int Shadow::Create(D3DXVECTOR3 m_pos, float fSizeX, float fSizeZ)
{
	int nIdxShadow = -1;

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (!g_aShadow[nCntShadow].m_bUse)	//	�z��̋󂫂�������
		{
			g_aShadow[nCntShadow].m_pos = m_pos;
			g_aShadow[nCntShadow].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aShadow[nCntShadow].m_bUse = true;

			SetVertex(nCntShadow, fSizeX, fSizeZ);

			nIdxShadow = nCntShadow;	//	�󂫂��������z��ԍ���ۑ�
			break;
		}
	}

	return nIdxShadow;	//	�󂫂��������z��ԍ���Ԃ�
}

//	�����[�X����
void Shadow::Release(int nIdxShadow)
{
	if (nIdxShadow >= 0 && nIdxShadow < MAX_SHADOW)
	{
		g_aShadow[nIdxShadow].m_bUse = false;
	}
}

//	�ʒu�̐ݒ�
void Shadow::SetPosition(int nIdxShadow, D3DXVECTOR3 m_pos)
{
	g_aShadow[nIdxShadow].m_pos = m_pos;
}

// ���_�J���[�̐ݒ�
void Shadow::SetColor(int nIdxShadow, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxShadow * 4);

		// ���_�J���[�̐ݒ�
		pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = col;

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffShadow->Unlock();
	}
}

//	���_���W�̐ݒ�
void Shadow::SetVertex(int nIdxShadow, float fSizeX, float fSizeZ)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxShadow * 4);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, 0.0f, fSizeZ / 2);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, 0.0f, fSizeZ / 2);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, 0.0f, -fSizeZ / 2);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, 0.0f, -fSizeZ / 2);

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffShadow->Unlock();
	}
}

// ���_���̍쐬
HRESULT Shadow::MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX3D,					// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,				// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&m_pVtxBuffShadow,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))							// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx += 4)
		{
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
		}

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffShadow->Unlock();
	}

	return S_OK;
}

