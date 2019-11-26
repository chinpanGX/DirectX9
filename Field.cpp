/*==========================================================

	[Field.cpp]
	Author : �o���đ�

===========================================================*/

#include "main.h"
#include "Field.h"
#include "texture.h"
#include <time.h>

#define BLOCK_X	4	//
#define BLOCK_Z	2
//	�X�^�e�B�b�N�ϐ�

//!	���_�o�b�t�@�c���_�����i�[�����������̈� -> VRAM�i�O���t�B�b�N�{�[�h�̃������j�ɂ�����
LPDIRECT3DVERTEXBUFFER9 Field::g_pVtxBuffField = NULL;	//	���_�o�b�t�@�ւ̃|�C���^

//!	�C���f�b�N�X�o�b�t�@�c�|���S�����쐬���邽�߂̒��_�ԍ����X�g�@->�@���_���ƃ|���S�����쐬���钸�_�𕪗����邽�߂Ɏg��
//!	���f���t�@�C���Ȃǂ̃|���S�����b�V�����쐬����Ƃ��ɕK�v
LPDIRECT3DINDEXBUFFER9	Field::g_pIdxBuffField = NULL;	//	�C���f�b�N�X�o�b�t�@

D3DXMATRIX				Field::g_mtxWorldField;			//	���[���h�}�g���b�N�X
D3DXVECTOR3				Field::g_posField;				//	�n�ʂ̈ʒu
D3DXVECTOR3				Field::g_rotField;				//	�n�ʂ̌���(��])
D3DXVECTOR3				Field::g_sclField;				//	�n�ʂ̑傫��	
LPDIRECT3DDEVICE9		Field::pDevice;					//  �f�o�C�X�擾�p�ϐ�

int g_NumVertex;	//	���_�̐�			
int g_NumIndex;		//	�C���f�b�N�X�̐�
int g_NumPolygon;	//	�|���S���̐�

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

	g_NumVertex	 = (BLOCK_X + 1) * (BLOCK_Z + 1);	//	X�����̒��_���@���@Z�����̒��_��
	g_NumIndex	 = ((BLOCK_X + 1) * 2) * BLOCK_Z + (BLOCK_Z - 1) * 2;		//	(BLOCK_X + 1)��2�{���AZ�������K�v + �k�ރ|���S����
	g_NumPolygon = (BLOCK_X * 2) * BLOCK_Z + (BLOCK_Z - 1) * 4;

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
	//	SetStreamSource	(��P����:���_�o�b�t�@��ݒ肷��X�g���[���ԍ����w��A��Q����:���_�o�b�t�@�ւ̃|�C���^���w��A��R����:���_�f�[�^�̈���̃f�[�^�T�C�Y���o�C�g�P�ʂŎw��)
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@�̃Z�b�g
	pDevice->SetIndices(g_pIdxBuffField);

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD03));

	//----------------------------------------------------------------------------------------------
	//	DrawPrimitiveUp��DrawRimitive�̈Ⴂ
	//"	DrawPrimitiveUp�̐���	->	�|���Ȃǒ��_�����Ȃ��I�u�W�F�N�g�Ŏg��
	//!	�P�DDrawPrimitiveUp�����_����ʏ�̃������ɒu�� -> �������x���Ȃ�
	//!	�Q�DLock��Unlock���g��Ȃ��Ă���	

	//"	DrawPrimitive�̐���		->	���_���������f���t�@�C���Ȃǂ�`�悷��Ƃ��Ɏg��
	//!	�P�D���_���VRAM��ɒu�����@-> �������x������
	//!	�Q�DLock��Unlock���K�v�ɂȂ�	-> �p�t�H�[�}���X���ቺ ->�Ȃ�ׂ����Ȃ�������
	//----------------------------------------------------------------------------------------------

	//	�����ō�������_�o�b�t�@��`��i�������̊m�ہA��������Ȃ���΂Ȃ�Ȃ��@-> ���p����ƒx���Ȃ�j
	//DrawPrimitive (��1�����F�v���~�e�B�u���w��A��2�����F�`����J�n���钸�_���w��A��3�����F�g�p����v���~�e�B�u�̐����w��)
	//pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, POLYGON_NUM);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVertex, 0, 12);

}


//	���_�쐬�֐�
HRESULT Field::MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	//�I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer
		(sizeof(VERTEX_3D) * BLOCK_X * BLOCK_Z,		//Length		 -	���_�o�b�t�@�̃T�C�Y���o�C�g�P�ʂŎw��
		D3DUSAGE_WRITEONLY, 						//Usage			 -	�K�v�̉����ă��\�[�X�̐�����@��\���t���O���w��
		FVF_VERTEX_3D, 								//FVF			 -	�p�����钸�_�t�H�[�}�b�g�̃t���O�̑g�ݍ��킹���w��
		D3DPOOL_MANAGED, 							//Pool			 -	D3DPOOL �񋓌^�̃����o�ŁA�ǂ̃��������g�p���邩���w��
		&g_pVtxBuffField, 							//ppVertexBuffer -	�쐬�������_�o�b�t�@���󂯂�|�C���^�̃|�C���^���w��
		NULL)))
	{
		return E_FAIL;
		//	���]�b�g���s�̃G���[�l	�萔�Ɖ��
		//	D3DERR_INVALIDCALL		->	���\�b�h�̌Ăяo���������ł���
		//	D3DERR_OUTOFVIDEOMEMORY	->	�\���ȃf�B�X�v���C�����������݂��Ȃ�
		//	E_OUTOFMEMORY			->	�\���ȃ����������蓖�Ă邱�Ƃ��ł��Ȃ�
	}

	{
		VERTEX_3D *pVtx;

		//���_�f�[�^�̎w��͈͂����b�N���A�o�b�t�@�ւ̃|�C���^���擾���܂�
		//	OffsetToLock - ���b�N���钸�_�f�[�^�̃I�t�Z�b�g���o�C�g�P�ʂŎw�肵�܂�
		//	SizeToLock - ���b�N����͈͂��o�C�g�P�ʂŎw�肵�܂�
		//	ppbData - �擾���钸�_�f�[�^�ւ̃|�C���^�̃|�C���^���w�肵�܂�
		//	Flags - ���b�N�̕��@��\���t���O���w�肵�܂�
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		for (int z = 0; z < BLOCK_Z; z++)
		{
			for (int x = 0; x < BLOCK_X; x++)
			{
				pVtx[z * 4 + x].pos = D3DXVECTOR3(x * 100.0f, 0.0f, z * -100.0f);
				pVtx[z * 4 + x].tex = D3DXVECTOR2(x, z);
			}
		}

		for (int i = 0; i < BLOCK_X + 1 * BLOCK_Z + 1; i++)
		{
			pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		
		//���_�f�[�^��Unlock����
		g_pVtxBuffField->Unlock();

		if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * BLOCK_X * BLOCK_Z, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffField, NULL)))
		{
			return E_FAIL;
		}
	}
	//-----------------------------------------------------------------------------
	//	�C���f�b�N�X�o�b�t�@�̃����b�g
	//!	�P�D���_�o�b�t�@�̍팸	->	���_�o�b�t�@���g���ƒ��_�̏d������������
	//"			���_	�P�D�������̖��ʌ����B
	//"					�Q�D���_�̕ύX�����ɂ���
	//!	�Q�D���_�v�Z�̐�������A�������x�̌���	
	//------------------------------------------------------------------------------
	//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
	{
		WORD *pIdx;

		g_pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		for (int z = 0; BLOCK_Z * 2; z++)
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

