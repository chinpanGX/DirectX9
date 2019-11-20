/*==========================================================

	[Field.cpp]
	Author : �o���đ�

===========================================================*/

#include "main.h"
#include "Field.h"
#include "texture.h"
#include <time.h>

#define VERTEX_X	100
#define VERTEX_Z	100
//	�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 Field::g_pVtxBuffField = NULL;	//	���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9	Field::g_pIdxBuffField = NULL;			//	�C���f�b�N�X�o�b�t�@
D3DXMATRIX				Field::g_mtxWorldField;			//	���[���h�}�g���b�N�X
D3DXVECTOR3				Field::g_posField;				//	�n�ʂ̈ʒu
D3DXVECTOR3				Field::g_rotField;				//	�n�ʂ̌���(��])
D3DXVECTOR3				Field::g_sclField;				//	�n�ʂ̑傫��	
LPDIRECT3DDEVICE9		Field::pDevice;					//  �f�o�C�X�擾�p�ϐ�

int g_NumVertexField;

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

	g_NumVertexField = (VERTEX_X + 1) * (VERTEX_Z + 1);

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
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);					//�@������2�Ԗڂ�3�Ԗڂ̌v�Z���ʂ�1�Ԗڂɓ����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.x, g_rotField.y, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	D3DXMatrixTranslation(&mtxTranslate, g_posField.y, g_posField.x, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	
	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	//	���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@�̃Z�b�g
	pDevice->SetIndices(g_pIdxBuffField);

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD03));

	//	�����ō�������_�o�b�t�@��`��i�������̊m�ہA��������Ȃ���΂Ȃ�Ȃ����x���Ȃ�j
	//pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, POLYGON_NUM);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVertexField, 0, 12);

}


//	���_�쐬�֐�
HRESULT Field::MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	//�I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_X * VERTEX_Z, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffField, NULL)))
	{
		return E_FAIL;
	}

	{
		VERTEX_3D *pVtx;

		//���_�f�[�^�͈̔͂�Lock����
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		for (int z = 0; z < VERTEX_Z; z++)
		{
			for (int x = 0; x < VERTEX_X; x++)
			{
				pVtx[z * 4 + x].pos = D3DXVECTOR3(x * 100.0f, 0.0f, z * -100.0f);
				pVtx[z * 4 + x].tex = D3DXVECTOR2(x, z);
			}
		}

		for (int i = 0; i < 10000; i++)
		{
			pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		
		//���_�f�[�^��Unlock����
		g_pVtxBuffField->Unlock();

		if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * VERTEX_X * VERTEX_Z, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffField, NULL)))
		{
			return E_FAIL;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
	{
		WORD *pIdx;

		g_pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		for (int z = 0; VERTEX_Z * 2; z++)
		{

		}

		//{
		//	pIdx[0] = 4;
		//	pIdx[1] = 0;
		//	pIdx[2] = 5;
		//	pIdx[3] = 1;
		//	pIdx[4] = 6;
		//	pIdx[5] = 2;
		//	pIdx[6] = 7;
		//	pIdx[7] = 3;
		//}


		g_pIdxBuffField->Unlock();
	}


	return S_OK;
}

