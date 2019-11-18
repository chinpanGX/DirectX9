/*==========================================================

	[Field.cpp]
	Author : �o���đ�

===========================================================*/

#include "main.h"
#include "Field.h"
#include "texture.h"

//	�}�N����`
#define POLYGON_NUM 2
#define VERTEX_NUM 6

//	�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 Field::g_pVtxBuffField = NULL;	//	���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9	Field::g_pIdxBuffField = NULL;			//	�C���f�b�N�X�o�b�t�@
D3DXMATRIX				Field::g_mtxWorldField;			//	���[���h�}�g���b�N�X
D3DXVECTOR3				Field::g_posField;				//	�n�ʂ̈ʒu
D3DXVECTOR3				Field::g_rotField;				//	�n�ʂ̌���(��])
D3DXVECTOR3				Field::g_sclField;				//	�n�ʂ̑傫��	
LPDIRECT3DDEVICE9		Field::pDevice;					//  �f�o�C�X�擾�p�ϐ�

//	�t�B�[���h�̏�����
HRESULT Field::Init()
{
	pDevice = GetD3DDevice();
	// ���_���̍쐬
	MakeVertexField(pDevice);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	return S_OK;
}

//	�t�B�[���h�̏I������
void Field::Uninit()
{
	DEVICE_RELEASE(g_pVtxBuffField);
	DEVICE_RELEASE(g_pIdxBuffField);
}

//	�t�B�[���h�̕`��
void Field::Draw()
{
	pDevice = GetD3DDevice();

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	
	//	���[���h�ϊ��i���[���h�}�g���N�X�̏������j
	D3DXMatrixIdentity(&g_mtxWorldField);
	D3DXMatrixScaling(&mtxScl, g_sclField.x, g_sclField.y, g_sclField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);	//�@������2�Ԗڂ�3�Ԗڂ̌v�Z���ʂ�1�Ԗڂɓ����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.x, g_rotField.y, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	D3DXMatrixTranslation(&mtxTranslate, g_posField.y, g_posField.x, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	
	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	//	���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD03));

	//	�����ō�������_�o�b�t�@��`��i�������̊m�ہA��������Ȃ���΂Ȃ�Ȃ����x���Ȃ�j
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, POLYGON_NUM);

}


//	���_�쐬�֐�
HRESULT Field::MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	//�I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffField, NULL)))
	{
		return E_FAIL;
	}

	{
		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		//���_�o�b�t�@�͈̔͂̃��b�N���A���_�o�b�t�@�ւ̃|�C���^�����炤
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		for (int z = 0; z < 2; z++)
		{
			for (int x = 0; x < 4; x++)
			{
				pVtx[z * 4 + x].pos = D3DXVECTOR3(x * 100.0f, 0.0f, z * -100.0f);
			}
		}

		for (int i = 0; i < 8; i++)
		{
			pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//	�@���x�N�g���̐ݒ�
			pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//	���_�J���[�̐ݒ�
		}

		//�e�N�X�`�����W
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(2.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(3.0f, 0.0f);
			pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[5].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[6].tex = D3DXVECTOR2(2.0f, 1.0f);
			pVtx[7].tex = D3DXVECTOR2(3.0f, 1.0f);

		}
		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffField->Unlock();
	}

	return S_OK;
}

