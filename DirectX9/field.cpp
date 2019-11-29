
#include "field.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// ��]��

#define NUM_BOX 2

#define	FIELD_WIDTH		(100.0f)						// �n�ʂ̕�(X����)
#define	FIELD_DEPTH		(100.0f)						// �n�ʂ̉��s(Z����)
#define FIELD_HEIGHT     (100.0f)
#define BLOCK_X 4
#define BLOCK_Y 2

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffField = NULL;
int                     g_NumIndexField = 36;

int g_NumVertex = (BLOCK_X + 1)*(BLOCK_Y + 1);
int g_NumIndex = ((BLOCK_X + 1) * 2) * BLOCK_Y + (BLOCK_Y - 1) * 2;//(BLOCK_Y - 1) * 2�͏h��|���S����ǉ�����
int g_NumPolygon = (BLOCK_X * 2)*BLOCK_Y + (BLOCK_Y - 1) * 4;

class blockField
{

public:

	D3DXMATRIX				g_mtxWorldField;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posField;				// �n�ʂ̈ʒu
	D3DXVECTOR3				g_rot1Field;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_rot2Field;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_sclField;				// �n�ʂ̑傫��(�X�P�[��)


};

typedef struct {
	D3DXMATRIX				g_mtxWorldField;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posField;				// �n�ʂ̈ʒu
	D3DXVECTOR3				g_rot1Field;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_rot2Field;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_sclField;				// �n�ʂ̑傫��(�X�P�[��)
}BOX_T;

BOX_T g_Box[NUM_BOX];

blockField g_blockField[2];


float g_move_x = 5.0f;
float g_scl_x = 0.01f;
float g_rot1_x = 0.01f;

//=============================================================================
// ����������
//=============================================================================
HRESULT Field_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexField(pDevice);

	 //�ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < NUM_BOX; i++)
	{
		g_Box[i].g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		/*g_Box[i].g_rot1Field = D3DXVECTOR3(0.0f, 0.0f, 0.0f);*/
		g_Box[i].g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Field_Finalize(void)
{
	if (g_pIdxBuffField != NULL)
	{//�C���f�b�N�X�o�b�t�@�J��
		g_pIdxBuffField->Release();
		g_pIdxBuffField = NULL;
	}

	if(g_pVtxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Field_Update(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	for (int i = 0; i < NUM_BOX; i++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Box[i].g_mtxWorldField);
	}

	//g_Box[0].g_rot1Field.y += 0.01f;

	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[0].g_rot1Field.y, g_Box[0].g_rot1Field.x, g_Box[0].g_rot1Field.z);
	//D3DXMatrixMultiply(&g_Box[0].g_mtxWorldField, &g_Box[0].g_mtxWorldField, &mtxRot);


	//g_Box[0].g_posField.x = 100.0f;
	//g_Box[0].g_posField.z = 100.0f;

	//D3DXMatrixTranslation(&mtxTranslate, g_Box[0].g_posField.x, g_Box[0].g_posField.y, g_Box[0].g_posField.z);
	//D3DXMatrixMultiply(&g_Box[0].g_mtxWorldField, &g_Box[0].g_mtxWorldField, &mtxTranslate);


	//g_Box[1].g_rot1Field.y -= 0.05f;

	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot1Field.y, g_Box[1].g_rot1Field.x, g_Box[1].g_rot1Field.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxRot);

	//g_Box[1].g_posField.x = 120.0f;
	//g_Box[1].g_posField.z = 100.0f;

	//D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posField.x, g_Box[1].g_posField.y, g_Box[1].g_posField.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxTranslate);

	//g_Box[1].g_rot2Field.y += 0.05f;

	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot2Field.y, g_Box[1].g_rot2Field.x, g_Box[1].g_rot2Field.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxRot);

	//D3DXVECTOR3 pos;
	//pos.x = 100.0f;
	//pos.y = 100.0f;
	//pos.z = 100.0f;

	//D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posField.x, g_Box[1].g_posField.y, g_Box[1].g_posField.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxTranslate);

	//////�l�p�̊g��k��������
	//{
	//	/*g_sclField.x += g_scl_x;
	//	if (g_sclField.x > 3.0f || g_sclField.x < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}

	//	g_sclField.z += g_scl_x;
	//	if (g_sclField.z > 3.0f || g_sclField.z < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}

	//	g_sclField.y += g_scl_x;
	//	if (g_sclField.y > 3.0f || g_sclField.y < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}
	//	*/
	//}
	////�X�P�[���𔽉f
	//{
	//	D3DXMatrixScaling(&mtxScl, g_sclField.x, g_sclField.y, g_sclField.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);
	//}	
	////�l�p�̊p�x������
	//{
	//	//g_rotField.z = D3DXToRadian(45);
	//}
	////�l�p�̉�]
	//{
	//	g_rot1Field.y -= 0.03f;
	//	g_rot2Field.y += 0.03f;
	//}
	////���S��ύX���ĉ�]
	//{
	//	/*
	//	��]���f���ړ�������ړ����聨��]����ɕς���
	//	*/
	//}
	////�l�p�̈ړ�
	///*{
	//	g_posField.x += g_move_x;
	//	if (g_posField.x >220.0f|| g_posField.x < -220.0f)
	//	{
	//	g_move_x*=-1 ;
	//	}

	//}
	//*/
	////�Œ�ʒu
	//{

	//	g_posField.x = 300;
	//	g_posField.y = 100;
	//	g_posField.z = 300;

	//}
	////��]�𔽉f
	///*
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_blockField[0].g_rot1Field.y, g_blockField[0].g_rot1Field.x, g_rot1Field.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	//}
	//
	////�ړ��𔽉f
	//
	//{
	//	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	//}
	//
	////��]�𔽉f(���ʂ̉�])
	//
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot2Field.y, g_rot2Field.x, g_rot2Field.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	//}
	//
	////��]�𔽉f
	//
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_blockField[0].g_rot1Field.y, g_rot1Field.x, g_rot1Field.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	//}*/
	///*
	////�ړ��𔽉f
	//{
	//	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	//}
	//*/
	///*
	////��]�𔽉f
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot2Field.y, g_rot2Field.x, g_rot2Field.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	//}
//	if (Keyboard_IsPress(DIK_A))
//	{
//		if (Keyboard_IsPress(DIK_W))
//		{// �����ړ�
//			g_posField.x += sinf(-D3DX_PI * 0.75f) * VALUE_MOVE;
//			g_posField.z -= cosf(-D3DX_PI * 0.75f) * VALUE_MOVE;
//		}
//		else if (Keyboard_IsPress(DIK_S))
//		{// ����O�ړ�
//			g_posField.x += sinf(-D3DX_PI * 0.25f) * VALUE_MOVE;
//			g_posField.z -= cosf(-D3DX_PI * 0.25f) * VALUE_MOVE;
//		}
//		else
//		{// ���ړ�
//			g_posField.x += sinf(-D3DX_PI * 0.50f) * VALUE_MOVE;
//			g_posField.z -= cosf(-D3DX_PI * 0.50f) * VALUE_MOVE;
//		}
//	}
//	else if (Keyboard_IsPress(DIK_D))
//	{
//		if (Keyboard_IsPress(DIK_W))
//		{// �E���ړ�
//			g_posField.x += sinf(D3DX_PI * 0.75f) * VALUE_MOVE;
//			g_posField.z -= cosf(D3DX_PI * 0.75f) * VALUE_MOVE;
//		}
//		else if (Keyboard_IsPress(DIK_S))
//		{// �E��O�ړ�
//			g_posField.x += sinf(D3DX_PI * 0.25f) * VALUE_MOVE;
//			g_posField.z -= cosf(D3DX_PI * 0.25f) * VALUE_MOVE;
//		}
//		else
//		{// �E�ړ�
//			g_posField.x += sinf(D3DX_PI * 0.50f) * VALUE_MOVE;
//			g_posField.z -= cosf(D3DX_PI * 0.50f) * VALUE_MOVE;
//		}
//	}
//	else if (Keyboard_IsPress(DIK_W))
//	{// ���ړ�
//		g_posField.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
//		g_posField.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
//	}
//	else if (Keyboard_IsPress(DIK_S))
//	{// ��O�ړ�
//		g_posField.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
//		g_posField.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
//	}
//
//#if 1
//	if (Keyboard_IsPress(DIK_Q))
//	{// Y������]
//		g_rotField.y -= VALUE_ROTATE;
//		if(g_rotField.y > D3DX_PI)
//		{
//			g_rotField.y -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.y < -D3DX_PI)
//		{
//			g_rotField.y += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_E))
//	{// Y���E��]
//		g_rotField.y += VALUE_ROTATE;
//		if(g_rotField.y > D3DX_PI)
//		{
//			g_rotField.y -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.y < -D3DX_PI)
//		{
//			g_rotField.y += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//#if 1
//	if (Keyboard_IsPress(DIK_UP))
//	{// X���E��]
//		g_rotField.x += VALUE_ROTATE;
//		if(g_rotField.x > D3DX_PI)
//		{
//			g_rotField.x -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.x < -D3DX_PI)
//		{
//			g_rotField.x += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_DOWN))
//	{// X������]
//		g_rotField.x -= VALUE_ROTATE;
//		if(g_rotField.x > D3DX_PI)
//		{
//			g_rotField.x -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.x < -D3DX_PI)
//		{
//			g_rotField.x += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//#if 1
//	if (Keyboard_IsPress(DIK_LEFT))
//	{// Z���E��]
//		g_rotField.z += VALUE_ROTATE;
//		if(g_rotField.z > D3DX_PI)
//		{
//			g_rotField.z -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.z < -D3DX_PI)
//		{
//			g_rotField.z += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_RIGHT))
//	{// Z������]
//		g_rotField.z -= VALUE_ROTATE;
//		if(g_rotField.z > D3DX_PI)
//		{
//			g_rotField.z -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.z < -D3DX_PI)
//		{
//			g_rotField.z += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//	if (Keyboard_IsPress(DIK_RETURN))
//	{// ���Z�b�g
//		g_posField.x = 0.0f;
//		g_posField.y = 0.0f;
//		g_posField.z = 0.0f;
//
//		g_rotField.x = 0.0f;
//		g_rotField.y = 0.0f;
//		g_rotField.z = 0.0f;
//	}

	//DebugProc_Print("*** �R�c�|���S������ ***\n");
	//DebugProc_Print("�ʒu [%f : %f : %f]\n", g_posField.x, g_posField.y, g_posField.z);
	//DebugProc_Print("�O�ړ� : �v\n");
	//DebugProc_Print("��ړ� : �r\n");
	//DebugProc_Print("���ړ� : �`\n");
	//DebugProc_Print("�E�ړ� : �c\n");
	//DebugProc_Print("\n");

	//DebugProc_Print("���� [%f : %f : %f]\n", g_rotField.x, g_rotField.y, g_rotField.z);
	//DebugProc_Print("X����] : �� / ��\n");
	//DebugProc_Print("Y����] : �p / �d\n");
	//DebugProc_Print("Z����] : �� / ��\n");
	//DebugProc_Print("\n");

	//DebugProc_Print("�ʒu�E�������Z�b�g : ENTER\n");
}

//=============================================================================
// �`�揈��
//=============================================================================
void Field_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	for (int i = 0; i < NUM_BOX; i++)
	{//���[���h�}�g���N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Box[i].g_mtxWorldField);
		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pDevice->SetIndices(g_pIdxBuffField);
		//���_�t�H�[�}�b�g�ɐݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));
		//����������VRAM�ɂԂ����ށA���������Ȃ�
		/*pDevice->DrawPrimitive (D3DPT_TRIANGLELIST,0,NUM_POLYGON);
		*/
		//�|���S���̕`��i�C���f�b�N�X�o�b�t�@�Łj
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 8, 0, 12);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	//�I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL)))
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		//���_�f�[�^�͈̔͂����b�N
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for (int z = 0; z < 2; z++) {
			for (int x = 0; x < 4; x++) {
				pVtx[z * 4 + x].pos = D3DXVECTOR3(x*100.0f, 0.0f, z*-100.0f);
			}
		}

		/*pVtx[0].pos = D3DXVECTOR3(-FIELD_WIDTH, FIELD_HEIGHT, -FIELD_DEPTH);
		pVtx[1].pos = D3DXVECTOR3(FIELD_WIDTH, FIELD_HEIGHT, -FIELD_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(-FIELD_WIDTH, -FIELD_HEIGHT, -FIELD_DEPTH);
		pVtx[3].pos = D3DXVECTOR3(FIELD_WIDTH, -FIELD_HEIGHT, -FIELD_DEPTH);
		pVtx[4].pos = D3DXVECTOR3(-FIELD_WIDTH, -FIELD_HEIGHT, FIELD_DEPTH);
		pVtx[5].pos = D3DXVECTOR3(-FIELD_WIDTH, -FIELD_HEIGHT, FIELD_DEPTH);
		pVtx[6].pos = D3DXVECTOR3(FIELD_WIDTH, FIELD_HEIGHT, FIELD_DEPTH);
		pVtx[7].pos = D3DXVECTOR3(-FIELD_WIDTH, FIELD_HEIGHT, FIELD_DEPTH);*/

		pVtx[0].nor = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
		pVtx[4].nor = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		pVtx[5].nor = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		pVtx[6].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[7].nor = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex.x = 0.0f;
		pVtx[0].tex.y = 0.0f;
		pVtx[1].tex.x = 1.0f;
		pVtx[1].tex.y = 0.0f;
		pVtx[2].tex.x = 2.0f;
		pVtx[2].tex.y = 0.0f;
		pVtx[3].tex.x = 3.0f;
		pVtx[3].tex.y = 0.0f;
		pVtx[4].tex.x = 0.0f;
		pVtx[4].tex.y = 1.0f;
		pVtx[5].tex.x = 1.0f;
		pVtx[5].tex.y = 1.0f;
		pVtx[6].tex.x = 3.0f;
		pVtx[6].tex.y = 1.0f;
		pVtx[7].tex.x = 2.0f;
		pVtx[7].tex.y = 1.0f;


		g_pVtxBuffField->Unlock();
	}

	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * 8,
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

		pIdx[0] = 4;
		pIdx[1] = 0;
		pIdx[2] = 5;
		pIdx[3] = 1;
		pIdx[4] = 6;
		pIdx[5] = 2;
		pIdx[6] = 7;
		pIdx[7] = 3;

		g_pIdxBuffField->Unlock();
	}
	return S_OK;
}
