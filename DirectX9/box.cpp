/*===============================================

	[box.cpp]
	Author : �o���đ�

================================================*/

#include "Box.h"
#include "texture.h"
#include "debugproc.h"

// �}�N����`
#define	VALUE_MOVE		(5.0f)				//	�ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)	//	��]��
#define	BOX_WIDTH		(100.0f)			//	��(X����)�̑傫��
#define	BOX_DEPTH		(100.0f)			//	���s(Z����)�傫��
#define BOX_HEIGHT      (100.0f)			//	����(Y����)�傫��		
#define NUM_BOX 2							//	BOX�̐�

//	�O���[�o���ϐ�
Box m_Box[NUM_BOX];
VERTEX_3D pVtx[4];

// �X�^�e�B�b�N�ϐ�
LPDIRECT3DVERTEXBUFFER9 Box::m_pVtxBuffBox = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9  Box::m_pIdxBuffBox = NULL;
int						Box::m_NumVertexBox = 36;
D3DXVECTOR3				Box::m_posBox;				// �n�ʂ̈ʒu
D3DXVECTOR3				Box::m_rot1Box;				// �n�ʂ̌���(��])
D3DXVECTOR3				Box::m_rot2Box;				// �n�ʂ̌���(��])
D3DXVECTOR3				Box::m_sclBox;				// �n�ʂ̑傫��(�X�P�[��)
LPDIRECT3DDEVICE9		Box::m_pDevice;

//	����������
HRESULT Box::Init()
{
	m_pDevice = GetD3DDevice();

	pVtx[0].pos = D3DXVECTOR3(-100, 0.0f, 100);
	pVtx[1].pos = D3DXVECTOR3(100, 0.0f, 100);
	pVtx[2].pos = D3DXVECTOR3(-100, 0.0f, -100);
	pVtx[3].pos = D3DXVECTOR3(100, 0.0f, -100);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���̍쐬
	MakeVertexBox(m_pDevice);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < NUM_BOX; i++)
	{
		m_Box[i].m_posBox = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Box[i].m_rot1Box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Box[i].m_rot2Box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Box[i].m_sclBox = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	return S_OK;
}

//	�I������
void Box::Uninit()
{
	SAFE_RELEASE(m_pIdxBuffBox);
	SAFE_RELEASE(m_pVtxBuffBox);
}

//	�X�V����
void Box::Update()
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	for (int i = 0; i < NUM_BOX; i++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_Box[i].m_mtxWorldBox);
	}

	m_Box[0].m_rot1Box.y += 0.01f;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Box[0].m_rot1Box.y, m_Box[0].m_rot1Box.x, m_Box[0].m_rot1Box.z);
	D3DXMatrixMultiply(&m_Box[0].m_mtxWorldBox, &m_Box[0].m_mtxWorldBox, &mtxRot);


	m_Box[0].m_posBox.x = 100.0f;
	m_Box[0].m_posBox.z = 100.0f;

	D3DXMatrixTranslation(&mtxTranslate, m_Box[0].m_posBox.x, m_Box[0].m_posBox.y, m_Box[0].m_posBox.z);
	D3DXMatrixMultiply(&m_Box[0].m_mtxWorldBox, &m_Box[0].m_mtxWorldBox, &mtxTranslate);


	m_Box[1].m_rot1Box.y -= 0.05f;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Box[1].m_rot1Box.y, m_Box[1].m_rot1Box.x, m_Box[1].m_rot1Box.z);
	D3DXMatrixMultiply(&m_Box[1].m_mtxWorldBox, &m_Box[1].m_mtxWorldBox, &mtxRot);

	m_Box[1].m_posBox.x = 120.0f;
	m_Box[1].m_posBox.z = 100.0f;

	D3DXMatrixTranslation(&mtxTranslate, m_Box[1].m_posBox.x, m_Box[1].m_posBox.y, m_Box[1].m_posBox.z);
	D3DXMatrixMultiply(&m_Box[1].m_mtxWorldBox, &m_Box[1].m_mtxWorldBox, &mtxTranslate);

	m_Box[1].m_rot2Box.y += 0.05f;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Box[1].m_rot2Box.y, m_Box[1].m_rot2Box.x, m_Box[1].m_rot2Box.z);
	D3DXMatrixMultiply(&m_Box[1].m_mtxWorldBox, &m_Box[1].m_mtxWorldBox, &mtxRot);

	D3DXVECTOR3 pos;
	pos.x = 100.0f;
	pos.y = 100.0f;
	pos.z = 100.0f;

	D3DXMatrixTranslation(&mtxTranslate, m_Box[1].m_posBox.x, m_Box[1].m_posBox.y, m_Box[1].m_posBox.z);
	D3DXMatrixMultiply(&m_Box[1].m_mtxWorldBox, &m_Box[1].m_mtxWorldBox, &mtxTranslate);
}

//	�`�揈��
void Box::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X�̏�����

	//���[���h�}�g���N�X�̐ݒ�
	for (int i = 0; i < 6; i++)
	{
		D3DXMatrixIdentity(&mtxWorld);
		DrawPolygon(i);
		m_pDevice->SetStreamSource(0, m_pVtxBuffBox, 0, sizeof(VERTEX_3D));					//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		m_pDevice->SetIndices(m_pIdxBuffBox);												//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		m_pDevice->SetFVF(FVF_VERTEX3D);													//���_�t�H�[�}�b�g�ɐݒ�
		m_pDevice->SetTexture(0, Texture::GetTexture(TEXTURE_INDEX_FIELD01));				//�e�N�X�`���̐ݒ�
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_NumVertexBox, 0, 12);	//�|���S���̕`��
	}
}

HRESULT Box::MakeVertexBox(LPDIRECT3DDEVICE9 pDevice)
{
	//�I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 36,D3DUSAGE_WRITEONLY,FVF_VERTEX3D,D3DPOOL_MANAGED,&m_pVtxBuffBox,NULL)))
	{
		return E_FAIL;
	}

	{
		//���_�o�b�t�@�̒��g�𖄂߂�
		void *pVtx2;

		m_pVtxBuffBox->Lock(0, 0, (void**)&pVtx2, 0);
		memcpy(pVtx2, pVtx, sizeof(VERTEX_3D) * 4);
		m_pVtxBuffBox->Unlock();

		m_pVtxBuffBox->Unlock();
	}

	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * m_NumVertexBox,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIdxBuffBox,NULL)))
	{
		return E_FAIL;
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		m_pIdxBuffBox->Lock(0, 0, (void**)&pIdx, 0);

		//�O��
		pIdx[0] = 0;
		pIdx[1] = 1;
		pIdx[2] = 2;
		pIdx[3] = 1;
		pIdx[4] = 3;
		pIdx[5] = 2;
		//���
		pIdx[6] = 7;
		pIdx[7] = 6;
		pIdx[8] = 0;
		pIdx[9] = 6;
		pIdx[10] = 1;
		pIdx[11] = 0;
		//����
		pIdx[12] = 5;
		pIdx[13] = 4;
		pIdx[14] = 7;
		pIdx[15] = 4;
		pIdx[16] = 6;
		pIdx[17] = 7;
		//���
		pIdx[18] = 2;
		pIdx[19] = 3;
		pIdx[20] = 5;
		pIdx[21] = 3;
		pIdx[22] = 4;
		pIdx[23] = 5;
		//�E��
		pIdx[24] = 1;
		pIdx[25] = 6;
		pIdx[26] = 3;
		pIdx[27] = 6;
		pIdx[28] = 4;
		pIdx[29] = 3;
		//����
		pIdx[30] = 7;
		pIdx[31] = 0;
		pIdx[32] = 5;
		pIdx[33] = 0;
		pIdx[34] = 2;
		pIdx[35] = 5;

		m_pIdxBuffBox->Unlock();
	}
	return S_OK;
}

void Box::DrawPolygon(int i)
{
	D3DXMATRIX mtxRot, mtxTranslate;

	switch (i)
	{
	case 0:
		D3DXMatrixRotationYawPitchRoll(&m_mtxWorldBox, m_rot1Box.y, m_rot1Box.x, m_rot1Box.z);
		break;
	case 1:
		D3DXMatrixTranslation(&mtxTranslate, 0.0f, -10.0f, 10.0f);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot1Box.y, m_rot1Box.x + 0.5f * D3DX_PI, m_rot1Box.z);
		D3DXMatrixMultiply(&m_mtxWorldBox, &mtxRot, &mtxTranslate);
		break;
	case 2:
		D3DXMatrixTranslation(&mtxTranslate, 0.0f, -20.0f, 0.0f);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot1Box.y, m_rot1Box.x + D3DX_PI, m_rot1Box.z);
		D3DXMatrixMultiply(&m_mtxWorldBox, &mtxRot, &mtxTranslate);
		break;
	case 3:
		D3DXMatrixTranslation(&mtxTranslate, 0.0f, -10.0f, -10.0f);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot1Box.y, m_rot1Box.x - 0.5f * D3DX_PI, m_rot1Box.z);
		D3DXMatrixMultiply(&m_mtxWorldBox, &mtxRot, &mtxTranslate);
		break;
	case 4:
		D3DXMatrixTranslation(&mtxTranslate, 10.0f, -10.0f, 0.0f);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot1Box.y, m_rot1Box.x, m_rot1Box.z - 0.5f * D3DX_PI);
		D3DXMatrixMultiply(&m_mtxWorldBox, &mtxRot, &mtxTranslate);
		break;
	case 5:
		D3DXMatrixTranslation(&mtxTranslate, -10.0f, -10.0f, 0.0f);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot1Box.y, m_rot1Box.x, m_rot1Box.z + 0.5f * D3DX_PI);
		D3DXMatrixMultiply(&m_mtxWorldBox, &mtxRot, &mtxTranslate);
		break;
	default:
		break;
	}
	return;
}