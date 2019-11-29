#include "field.h"

#include "input.h"
#include "texture.h"
#include "debugproc.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define VALUE_MOVE  (5.0f)       // �ړ���
#define VALUE_ROTATE (D3DX_PI * 0.02f)    // ��]��

#define FIELD_WIDTH  (100.0f)      // �n�ʂ̕�(X����)
#define FIELD_HEIGHT (100.0f)      // �n�ʂ̕�(X����)
#define FIELD_DEPTH  (100.0f)      // �n�ʂ̉��s(Z����)



#define SQUARE_LENGTH (100)
#define SQUARE_NUM  (SQUARE_LENGTH + 1)
#define TYOTEN (pow(SQUARE_NUM,2)*2)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL; // ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffField = NULL; // ���_�o�b�t�@�ւ̃|�C���^
int      g_NumIndexField = 18;

D3DXMATRIX    g_mtxWorldField;  // ���[���h�}�g���b�N�X
D3DXVECTOR3    g_posField;    // �n�ʂ̈ʒu
D3DXVECTOR3    g_posField1;   // �n�ʂ̈ʒu
D3DXVECTOR3    g_rotField;    // �n�ʂ̌���(��])
D3DXVECTOR3    g_rotField1;   // �n�ʂ̌���(��])
D3DXVECTOR3    g_sclField;    // �n�ʂ̑傫��(�X�P�[��)

static float g_NumVertex;
static float g_NumIndex;
static float g_NumPolygon;

//=============================================================================
// ����������
//=============================================================================
HRESULT Field_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexField(pDevice);
	//MakeVertexField1(pDevice1);
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posField = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//MoveBox = 1.0f;
	//Select = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Field_Finalize(void)
{
	if (g_pVtxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}

	if (g_pIdxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pIdxBuffField->Release();
		g_pIdxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Field_Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void Field_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldField);

	//�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_sclField.x, g_sclField.y, g_sclField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.x, g_rotField.y, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	//
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(g_pIdxBuffField);
	//�C���f�b�N�X�o�b�t�@�̃Z�b�g
	pDevice->SetFVF(FVF_VERTEX3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, TYOTEN, 0, TYOTEN - 4);


	DebugProc_Print((char*)"*** �R�c�|���S������ ***\n");
	DebugProc_Print((char*)"�ʒu [%f : %f : %f]\n", g_posField.x, g_posField.y, g_posField.z);
	DebugProc_Print((char*)"�O�ړ� : �v\n");
	DebugProc_Print((char*)"��ړ� : �r\n");
	DebugProc_Print((char*)"���ړ� : �`\n");
	DebugProc_Print((char*)"�E�ړ� : �c\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"���� [%f : %f : %f]\n", g_rotField.x, g_rotField.y, g_rotField.z);
	DebugProc_Print((char*)"X����] : �� / ��\n");
	DebugProc_Print((char*)"Y����] : �p / �d\n");
	DebugProc_Print((char*)"Z����] : �� / ��\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"�ʒu�E�������Z�b�g : ENTER\n");
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * TYOTEN / 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL)))
	{
		return E_FAIL;
	}


	{
		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		//VRAM��̏��������Ă��邩��A���_���������悤�ɂȂ�
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for (int z = 0; z < SQUARE_NUM; z++) {
			for (int x = 0; x < SQUARE_NUM; x++) {

				pVtx[z * SQUARE_NUM + x].pos = D3DXVECTOR3(x * 10.0f - 500.0f, 0.0f, z * -10.0f + 400.0f);

				pVtx[z * SQUARE_NUM + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				pVtx[z * SQUARE_NUM + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx[z * SQUARE_NUM + x].tex.x = (float)x;
				pVtx[z * SQUARE_NUM + x].tex.y = (float)z;
			}
		}


		//������VRAM�֋A��
		g_pVtxBuffField->Unlock();
	}

	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * TYOTEN,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffField,
		NULL)))
	{
		return E_FAIL;
	}
	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		g_pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		//for�����킩��₷�����邽�߂̕ϐ�
		static int cnt1 = SQUARE_NUM;
		static int cnt2 = 0;

		//�k�ރ|���S���̃J�E���g
		static int cnt3 = 1;

		for (int i = 0; i < TYOTEN; i++)
		{
			//�k�ރ|���S����������
			if (i == ((SQUARE_LENGTH + 1) * 2 + 2) * cnt3 - 2 && i != 0)
			{
				pIdx[i] = pIdx[i - 1];  //�O�̒��_�ɔ킹��
				pIdx[i + 1] = cnt1;   //��̒��_
				pIdx[i + 2] = cnt1;   //�O�̒��_�ɔ킹��
				i += 2;      //���_�]���ɑ�������̂�+2
				cnt3++;      //�k�ރ|���S���̉񐔂𑝂₷
			}
			//�k�ރ|���S���������Ȃ��Ƃ�(i������������)
			if (i % 2 == 0)
			{
				pIdx[i] = cnt1;
				cnt1++;
			}
			else
			{
				pIdx[i] = cnt2;
				cnt2++;
			}
		}

		/*pIdx[0] = 6;
		pIdx[1] = 0;
		pIdx[2] = 7;
		pIdx[3] = 1;
		pIdx[4] = 8;
		pIdx[5] = 2;
		pIdx[6] = 9;
		pIdx[7] = 3;
		pIdx[8] = 10;
		pIdx[9] = 4;
		pIdx[10] = 11;
		pIdx[11] = 5;
		pIdx[12] = 5;
		pIdx[13] = 12;
		pIdx[14] = 12;
		pIdx[15] = 6;
		pIdx[16] = 13;
		pIdx[17] = 7;
		pIdx[18] = 14;
		pIdx[19] = 8;
		pIdx[20] = 15;
		pIdx[21] = 9;
		pIdx[22] = 16;
		pIdx[23] = 10;
		pIdx[24] = 18;
		pIdx[25] = 11;*/

		//������VRAM�֋A��
		g_pVtxBuffField->Unlock();
	}
	return S_OK;
}