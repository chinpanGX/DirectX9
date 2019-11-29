
#include "Box.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// ��]��

#define	BOX_WIDTH		(100.0f)						// �n�ʂ̕�(X����)
#define	BOX_DEPTH		(100.0f)						// �n�ʂ̉��s(Z����)
#define BOX_HEIGHT      (100.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBox = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffBox = NULL;
static int             g_NumIndexBox = 36;

#define NUM_BOX 2

typedef struct {
	D3DXMATRIX				g_mtxWorldBox;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posBox;				// �n�ʂ̈ʒu
	D3DXVECTOR3				g_rot1Box;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_rot2Box;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_sclBox;				// �n�ʂ̑傫��(�X�P�[��)
}BOX_T;

static BOX_T g_Box[NUM_BOX];

//blockBox g_blockBox[2];


//float g_move_x = 5.0f;
//float g_scl_x = 0.01f;
//float g_rot1_x = 0.01f;

//=============================================================================
// ����������
//=============================================================================
HRESULT Box_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexBox(pDevice);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < NUM_BOX; i++)
	{
		g_Box[i].g_posBox = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Box[i].g_rot1Box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Box[i].g_sclBox = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Box_Finalize(void)
{
	if (g_pIdxBuffBox != NULL)
	{
		g_pIdxBuffBox->Release();
		g_pIdxBuffBox = NULL;
	}

	if(g_pVtxBuffBox != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffBox->Release();
		g_pVtxBuffBox = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Box_Update(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	for (int i = 0; i < NUM_BOX; i++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Box[i].g_mtxWorldBox);
	}

	g_Box[0].g_rot1Box.y += 0.01f;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[0].g_rot1Box.y, g_Box[0].g_rot1Box.x, g_Box[0].g_rot1Box.z);
	D3DXMatrixMultiply(&g_Box[0].g_mtxWorldBox, &g_Box[0].g_mtxWorldBox, &mtxRot);


	g_Box[0].g_posBox.x = 100.0f;
	g_Box[0].g_posBox.z = 100.0f;

	D3DXMatrixTranslation(&mtxTranslate, g_Box[0].g_posBox.x, g_Box[0].g_posBox.y, g_Box[0].g_posBox.z);
	D3DXMatrixMultiply(&g_Box[0].g_mtxWorldBox, &g_Box[0].g_mtxWorldBox, &mtxTranslate);


	g_Box[1].g_rot1Box.y -= 0.05f;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot1Box.y, g_Box[1].g_rot1Box.x, g_Box[1].g_rot1Box.z);
	D3DXMatrixMultiply(&g_Box[1].g_mtxWorldBox, &g_Box[1].g_mtxWorldBox, &mtxRot);

	g_Box[1].g_posBox.x = 120.0f;
	g_Box[1].g_posBox.z = 100.0f;

	D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posBox.x, g_Box[1].g_posBox.y, g_Box[1].g_posBox.z);
	D3DXMatrixMultiply(&g_Box[1].g_mtxWorldBox, &g_Box[1].g_mtxWorldBox, &mtxTranslate);

	g_Box[1].g_rot2Box.y += 0.05f;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot2Box.y, g_Box[1].g_rot2Box.x, g_Box[1].g_rot2Box.z);
	D3DXMatrixMultiply(&g_Box[1].g_mtxWorldBox, &g_Box[1].g_mtxWorldBox, &mtxRot);

	D3DXVECTOR3 pos;
	pos.x = 100.0f;
	pos.y = 100.0f;
	pos.z = 100.0f;

	D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posBox.x, g_Box[1].g_posBox.y, g_Box[1].g_posBox.z);
	D3DXMatrixMultiply(&g_Box[1].g_mtxWorldBox, &g_Box[1].g_mtxWorldBox, &mtxTranslate);

	////�l�p�̊g��k��������
	//{
	//	/*g_sclBox.x += g_scl_x;
	//	if (g_sclBox.x > 3.0f || g_sclBox.x < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}

	//	g_sclBox.z += g_scl_x;
	//	if (g_sclBox.z > 3.0f || g_sclBox.z < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}

	//	g_sclBox.y += g_scl_x;
	//	if (g_sclBox.y > 3.0f || g_sclBox.y < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}
	//	*/
	//}
	////�X�P�[���𔽉f
	//{
	//	D3DXMatrixScaling(&mtxScl, g_sclBox.x, g_sclBox.y, g_sclBox.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxScl);
	//}	
	////�l�p�̊p�x������
	//{
	//	//g_rotBox.z = D3DXToRadian(45);
	//}
	////�l�p�̉�]
	//{
	//	g_rot1Box.y -= 0.03f;
	//	g_rot2Box.y += 0.03f;
	//}
	////���S��ύX���ĉ�]
	//{
	//	/*
	//	��]���f���ړ�������ړ����聨��]����ɕς���
	//	*/
	//}
	////�l�p�̈ړ�
	///*{
	//	g_posBox.x += g_move_x;
	//	if (g_posBox.x >220.0f|| g_posBox.x < -220.0f)
	//	{
	//	g_move_x*=-1 ;
	//	}

	//}
	//*/
	////�Œ�ʒu
	//{

	//	g_posBox.x = 300;
	//	g_posBox.y = 100;
	//	g_posBox.z = 300;

	//}
	////��]�𔽉f
	///*
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_blockBox[0].g_rot1Box.y, g_blockBox[0].g_rot1Box.x, g_rot1Box.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxRot);
	//}
	//
	////�ړ��𔽉f
	//
	//{
	//	D3DXMatrixTranslation(&mtxTranslate, g_posBox.x, g_posBox.y, g_posBox.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxTranslate);
	//}
	//
	////��]�𔽉f(���ʂ̉�])
	//
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot2Box.y, g_rot2Box.x, g_rot2Box.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxRot);
	//}
	//
	////��]�𔽉f
	//
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_blockBox[0].g_rot1Box.y, g_rot1Box.x, g_rot1Box.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxRot);
	//}*/
	///*
	////�ړ��𔽉f
	//{
	//	D3DXMatrixTranslation(&mtxTranslate, g_posBox.x, g_posBox.y, g_posBox.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxTranslate);
	//}
	//*/
	///*
	////��]�𔽉f
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot2Box.y, g_rot2Box.x, g_rot2Box.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxRot);
	//}
//	if (Keyboard_IsPress(DIK_A))
//	{
//		if (Keyboard_IsPress(DIK_W))
//		{// �����ړ�
//			g_posBox.x += sinf(-D3DX_PI * 0.75f) * VALUE_MOVE;
//			g_posBox.z -= cosf(-D3DX_PI * 0.75f) * VALUE_MOVE;
//		}
//		else if (Keyboard_IsPress(DIK_S))
//		{// ����O�ړ�
//			g_posBox.x += sinf(-D3DX_PI * 0.25f) * VALUE_MOVE;
//			g_posBox.z -= cosf(-D3DX_PI * 0.25f) * VALUE_MOVE;
//		}
//		else
//		{// ���ړ�
//			g_posBox.x += sinf(-D3DX_PI * 0.50f) * VALUE_MOVE;
//			g_posBox.z -= cosf(-D3DX_PI * 0.50f) * VALUE_MOVE;
//		}
//	}
//	else if (Keyboard_IsPress(DIK_D))
//	{
//		if (Keyboard_IsPress(DIK_W))
//		{// �E���ړ�
//			g_posBox.x += sinf(D3DX_PI * 0.75f) * VALUE_MOVE;
//			g_posBox.z -= cosf(D3DX_PI * 0.75f) * VALUE_MOVE;
//		}
//		else if (Keyboard_IsPress(DIK_S))
//		{// �E��O�ړ�
//			g_posBox.x += sinf(D3DX_PI * 0.25f) * VALUE_MOVE;
//			g_posBox.z -= cosf(D3DX_PI * 0.25f) * VALUE_MOVE;
//		}
//		else
//		{// �E�ړ�
//			g_posBox.x += sinf(D3DX_PI * 0.50f) * VALUE_MOVE;
//			g_posBox.z -= cosf(D3DX_PI * 0.50f) * VALUE_MOVE;
//		}
//	}
//	else if (Keyboard_IsPress(DIK_W))
//	{// ���ړ�
//		g_posBox.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
//		g_posBox.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
//	}
//	else if (Keyboard_IsPress(DIK_S))
//	{// ��O�ړ�
//		g_posBox.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
//		g_posBox.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
//	}
//
//#if 1
//	if (Keyboard_IsPress(DIK_Q))
//	{// Y������]
//		g_rotBox.y -= VALUE_ROTATE;
//		if(g_rotBox.y > D3DX_PI)
//		{
//			g_rotBox.y -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.y < -D3DX_PI)
//		{
//			g_rotBox.y += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_E))
//	{// Y���E��]
//		g_rotBox.y += VALUE_ROTATE;
//		if(g_rotBox.y > D3DX_PI)
//		{
//			g_rotBox.y -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.y < -D3DX_PI)
//		{
//			g_rotBox.y += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//#if 1
//	if (Keyboard_IsPress(DIK_UP))
//	{// X���E��]
//		g_rotBox.x += VALUE_ROTATE;
//		if(g_rotBox.x > D3DX_PI)
//		{
//			g_rotBox.x -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.x < -D3DX_PI)
//		{
//			g_rotBox.x += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_DOWN))
//	{// X������]
//		g_rotBox.x -= VALUE_ROTATE;
//		if(g_rotBox.x > D3DX_PI)
//		{
//			g_rotBox.x -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.x < -D3DX_PI)
//		{
//			g_rotBox.x += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//#if 1
//	if (Keyboard_IsPress(DIK_LEFT))
//	{// Z���E��]
//		g_rotBox.z += VALUE_ROTATE;
//		if(g_rotBox.z > D3DX_PI)
//		{
//			g_rotBox.z -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.z < -D3DX_PI)
//		{
//			g_rotBox.z += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_RIGHT))
//	{// Z������]
//		g_rotBox.z -= VALUE_ROTATE;
//		if(g_rotBox.z > D3DX_PI)
//		{
//			g_rotBox.z -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.z < -D3DX_PI)
//		{
//			g_rotBox.z += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//	if (Keyboard_IsPress(DIK_RETURN))
//	{// ���Z�b�g
//		g_posBox.x = 0.0f;
//		g_posBox.y = 0.0f;
//		g_posBox.z = 0.0f;
//
//		g_rotBox.x = 0.0f;
//		g_rotBox.y = 0.0f;
//		g_rotBox.z = 0.0f;
//	}

	//DebugProc_Print("*** �R�c�|���S������ ***\n");
	//DebugProc_Print("�ʒu [%f : %f : %f]\n", g_posBox.x, g_posBox.y, g_posBox.z);
	//DebugProc_Print("�O�ړ� : �v\n");
	//DebugProc_Print("��ړ� : �r\n");
	//DebugProc_Print("���ړ� : �`\n");
	//DebugProc_Print("�E�ړ� : �c\n");
	//DebugProc_Print("\n");

	//DebugProc_Print("���� [%f : %f : %f]\n", g_rotBox.x, g_rotBox.y, g_rotBox.z);
	//DebugProc_Print("X����] : �� / ��\n");
	//DebugProc_Print("Y����] : �p / �d\n");
	//DebugProc_Print("Z����] : �� / ��\n");
	//DebugProc_Print("\n");

	//DebugProc_Print("�ʒu�E�������Z�b�g : ENTER\n");
}

//=============================================================================
// �`�揈��
//=============================================================================
void Box_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	for (int i = 0; i < NUM_BOX; i++)
	{//���[���h�}�g���N�X�̐ݒ�

		//���[���h�}�g���b�N�X�̏�����
		D3DXMATRIX mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);

		pDevice->SetTransform(D3DTS_WORLD, &g_Box[i].g_mtxWorldBox);
		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pVtxBuffBox, 0, sizeof(VERTEX_3D));
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pDevice->SetIndices(g_pIdxBuffBox);
		//���_�t�H�[�}�b�g�ɐݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));
		//����������VRAM�ɂԂ����ށA���������Ȃ�
		/*pDevice->DrawPrimitive (D3DPT_TRIANGLELIST,0,NUM_POLYGON);
		*/
		//�|���S���̕`��i�C���f�b�N�X�o�b�t�@�Łj
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumIndexBox, 0, 12);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice)
{
	//�I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBox,
		NULL)))
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		g_pVtxBuffBox->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, -BOX_DEPTH);
		pVtx[1].pos = D3DXVECTOR3(BOX_WIDTH, BOX_HEIGHT, -BOX_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, -BOX_DEPTH);
		pVtx[3].pos = D3DXVECTOR3(BOX_WIDTH, -BOX_HEIGHT, -BOX_DEPTH);
		pVtx[4].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
		pVtx[5].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
		pVtx[6].pos = D3DXVECTOR3(BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
		pVtx[7].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);

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

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_pVtxBuffBox->Unlock();
	}

	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIndexBox,
												D3DUSAGE_WRITEONLY,
												D3DFMT_INDEX16,
												D3DPOOL_MANAGED,
												&g_pIdxBuffBox,
												NULL)))
	{
		return E_FAIL;
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		g_pIdxBuffBox->Lock(0, 0, (void**)&pIdx, 0);

		pIdx[0] = 0;
		pIdx[1] = 1;
		pIdx[2] = 2;
		pIdx[3] = 1;
		pIdx[4] = 3;
		pIdx[5] = 2;

		g_pIdxBuffBox->Unlock();
	}
	return S_OK;
}
