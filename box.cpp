
#include "Box.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLYGON_NUM 2
#define VERTEX_NUM 6

#define	BOX_WIDTH		(100.0f)						// �n�ʂ̕�(X����)
#define	BOX_DEPTH		(100.0f)						// �n�ʂ̉��s(Z����)
#define	BOX_HEIGHT	(100.0f)							// �n�ʂ̍���(Y����)

#define NUM_CUBE		(2)								//�����̂̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBox = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffBox = NULL;

//�������\���̂ɂ��ĕ`�悵���������̂̕��������₷

struct CUBE_T
{
	D3DXMATRIX				g_mtxWorldBox;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posBox;			// �n�ʂ̈ʒu
	D3DXVECTOR3				g_rot1Box;			// �n�ʂ̌���(��])
	D3DXVECTOR3				g_rot2Box;			// �n�ʂ̌���(��])
	D3DXVECTOR3				g_sclBox;			// �n�ʂ̑傫��(�X�P�[��)
};

static CUBE_T g_cube[NUM_CUBE];

int g_NumIndexBox = 36;

static float move = 2.0f;
static float scl = 0.005f;


//=============================================================================
// ����������
//=============================================================================
HRESULT Box_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexBox(pDevice);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < NUM_CUBE; i++)
	{
		g_cube[i].g_posBox = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cube[i].g_rot1Box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cube[i].g_rot2Box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cube[i].g_sclBox = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Box_Finalize(void)
{
	if (g_pIdxBuffBox != NULL)
	{// �C���f�b�N�X�o�b�t�@�̊J��
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
//
//	DebugProc_Print((char*)"*** �R�c�|���S������ ***\n");
//	DebugProc_Print((char*)"�ʒu [%f : %f : %f]\n", g_posBox.x, g_posBox.y, g_posBox.z);
//	DebugProc_Print((char*)"�O�ړ� : �v\n");
//	DebugProc_Print((char*)"��ړ� : �r\n");
//	DebugProc_Print((char*)"���ړ� : �`\n");
//	DebugProc_Print((char*)"�E�ړ� : �c\n");
//	DebugProc_Print((char*)"\n");
//
//	DebugProc_Print((char*)"���� [%f : %f : %f]\n", g_rotBox.x, g_rotBox.y, g_rotBox.z);
//	DebugProc_Print((char*)"X����] : �� / ��\n");
//	DebugProc_Print((char*)"Y����] : �p / �d\n");
//	DebugProc_Print((char*)"Z����] : �� / ��\n");
//	DebugProc_Print((char*)"\n");
//
//	DebugProc_Print((char*)"�ʒu�E�������Z�b�g : ENTER\n");

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		//�����玝���Ă���

	for (int i = 0; i < NUM_CUBE; i++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_cube[i].g_mtxWorldBox);

		//�X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, g_cube[i].g_sclBox.x, g_cube[i].g_sclBox.y, g_cube[i].g_sclBox.z);
		D3DXMatrixMultiply(&g_cube[i].g_mtxWorldBox, &g_cube[i].g_mtxWorldBox, &mtxScl);

		//����������]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_cube[i].g_rot1Box.y, g_cube[i].g_rot1Box.x, g_cube[i].g_rot1Box.z);
		D3DXMatrixMultiply(&g_cube[i].g_mtxWorldBox, &g_cube[i].g_mtxWorldBox, &mtxRot);

		//�ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_cube[i].g_posBox.x, g_cube[i].g_posBox.y, g_cube[i].g_posBox.z);
		D3DXMatrixMultiply(&g_cube[i].g_mtxWorldBox, &g_cube[i].g_mtxWorldBox, &mtxTranslate);

		//��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_cube[i].g_rot2Box.y, g_cube[i].g_rot2Box.x, g_cube[i].g_rot2Box.z);
		D3DXMatrixMultiply(&g_cube[i].g_mtxWorldBox, &g_cube[i].g_mtxWorldBox, &mtxRot);
	}

	g_cube[0].g_sclBox.x = 0.5f;
	g_cube[0].g_sclBox.y = 0.5f;
	g_cube[0].g_sclBox.z = 0.5f;

	g_cube[1].g_sclBox.x = 1.0f;
	g_cube[1].g_sclBox.y = 1.0f;
	g_cube[1].g_sclBox.z = 1.0f;

	g_cube[0].g_rot2Box.y -= 0.05f;
	//g_cube[1].g_rot1Box.y += 0.05f;
	//g_cube[1].g_rot2Box.y -= 0.05f;
	g_cube[1].g_posBox.x = 150.0f;

	D3DXMatrixMultiply(&g_cube[1].g_mtxWorldBox,&g_cube[1].g_mtxWorldBox, &g_cube[0].g_mtxWorldBox);

	//���̕��@�ŉ񂷂Ƃ���1�����񂷂���(�W���o�����b�N������邽��)
	//2�ȏ㓯���ɉ񂷂Ƃ��̓N�H�[�^�j�I�����g������


	//g_posBox.x = 250.0f;
	////g_posBox.y = 100.0f;
	////g_posBox.z = 100.0f;

	////g_rotBox.x = D3DXToRadian(45);
	////g_rotBox.z = D3DXToRadian(45);

	//g_rot1Box.y -= 0.05f;
	//g_rot2Box.x += 0.05f;
	//g_rot2Box.y += 0.05f;
	//g_rot2Box.z += 0.05f;

}

//=============================================================================
// �`�揈��
//=============================================================================
void Box_Draw(void)
{
	//�����Ń}�g���b�N�X�֐����΂���g���ł�������H

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	//D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	////���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&g_mtxWorldBox);

	////�X�P�[�����ĉ�]���Ĉړ��̏��Ԃ͑厖

	////�X�P�[���𔽉f
	//D3DXMatrixScaling(&mtxScl, g_sclBox.x, g_sclBox.y, g_sclBox.z);
	//D3DXMatrixMultiply(&g_mtxWorldBox,&g_mtxWorldBox,&mtxScl);

	////��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot,g_rotBox.y,g_rotBox.x,g_rotBox.z);
	//D3DXMatrixMultiply(&g_mtxWorldBox,&g_mtxWorldBox,&mtxRot);
	//
	////�ړ��𔽉f
	//D3DXMatrixTranslation(&mtxTranslate,g_posBox.x,g_posBox.y, g_posBox.z);
	//D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxTranslate);

	for (int i = 0; i < NUM_CUBE; i++)
	{
		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_cube[i].g_mtxWorldBox);

		//
		pDevice->SetStreamSource(0, g_pVtxBuffBox, 0, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pDevice->SetIndices(g_pIdxBuffBox);

		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

		//�|���S���̕`��
		//pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,NUM_POLYGON);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_NumIndexBox, 0, 12);
	}

	/*
	//������������
	D3DXVECTOR3 light;
	light.x =  0.0f;
	light.y = -1.0f;
	light.z =  0.0f;

	//���_�o�b�t�@�̒��𖄂߂�
	VERTEX_3D *pVtx;

	//���_�f�[�^�͈̔͂�Lock���A���_�o�b�t�@���Ȃ񂽂�
	g_pVtxBuffBox->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 calc_nor;
	
	//�x�N�g���ɍs��������ĕό`���Ă����֐�
	D3DXVec3TransformNormal(&calc_nor,&pVtx[0].nor,&g_mtxWorldBox);

	float brightness = D3DXVec3Dot(&light,&calc_nor);
	//���ς̒l���[���𒴂���ꍇ�̓[���ɂ���
	if (brightness > 0)
		brightness = 0;

	//-1�`0�̒l��0�`1�ɏ���������
	brightness = fabs(brightness);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(brightness, brightness, brightness, 1.0f);

	//���_�f�[�^��Unlock����
	g_pVtxBuffBox->Unlock();
	*/



	//DrawPrimitive������V�����g���������
	//��������ɒ��_���������Ă���
	//DrawPrimitiveUP���O�܂Ŏg���Ă����y�Ȃ��
	
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice)
{
	//�I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBox, NULL)))
	{
		return E_FAIL;
	}

	{
		VERTEX_3D *pVtx;

		//���_�f�[�^�͈̔͂�Lock����
		g_pVtxBuffBox->Lock(0, 0, (void**)&pVtx, 0);

		//�㉺�̖ʂ̒��_���W
		{
			pVtx [0].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			pVtx [1].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			pVtx [2].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);

			pVtx [3].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);
			pVtx [4].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);
			pVtx [5].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);


			pVtx [6].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
			pVtx [7].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
			//pVtx [8].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);

			//pVtx [9].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[10].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
			//pVtx[11].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
		}

		//��O���̖ʂ̒��_���W
		{
			//pVtx[12].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[13].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[14].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);

			//pVtx[15].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[16].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[17].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);


			//pVtx[18].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);
			//pVtx[19].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);
			//pVtx[20].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
 
			//pVtx[21].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);
			//pVtx[22].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
			//pVtx[23].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
		}

		//���E�̖ʂ̒��_���W
		{
			//pVtx[24].pos = D3DXVECTOR3( BOX_WIDTH,  BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[25].pos = D3DXVECTOR3( BOX_WIDTH,  BOX_HEIGHT, BOX_DEPTH);
			//pVtx[26].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT,-BOX_DEPTH);

			//pVtx[27].pos = D3DXVECTOR3( BOX_WIDTH,  BOX_HEIGHT, BOX_DEPTH);
			//pVtx[28].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
			//pVtx[29].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT,-BOX_DEPTH);


			//pVtx[30].pos = D3DXVECTOR3(-BOX_WIDTH,  BOX_HEIGHT, BOX_DEPTH);
			//pVtx[31].pos = D3DXVECTOR3(-BOX_WIDTH,  BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[32].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);

			//pVtx[33].pos = D3DXVECTOR3(-BOX_WIDTH,  BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[34].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[35].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);

		}

		//�@���x�N�g��
		{
			pVtx[0].nor = D3DXVECTOR3(-1.0f, 1.0f,-1.0f);
			pVtx[1].nor = D3DXVECTOR3( 1.0f, 1.0f,-1.0f);
			pVtx[2].nor = D3DXVECTOR3(-1.0f,-1.0f,-1.0f);
			pVtx[3].nor = D3DXVECTOR3( 1.0f,-1.0f,-1.0f);
			pVtx[4].nor = D3DXVECTOR3( 1.0f,-1.0f, 1.0f);
			pVtx[5].nor = D3DXVECTOR3(-1.0f,-1.0f, 1.0f);
			pVtx[6].nor = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
			pVtx[7].nor = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		}

		for (int i = 0; i < 8; i++)
		{
			//pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			switch (i % 4)
			{
			case 0:
				pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
				break;

			case 1:
				pVtx[i].tex = D3DXVECTOR2(1.0f, 0.0f);
				break;

			case 2:
				pVtx[i].tex = D3DXVECTOR2(0.0f, 1.0f);
				break;

			case 3:
				pVtx[i].tex = D3DXVECTOR2(1.0f, 1.0f);
				break;
			}
		}


		//���_�f�[�^��Unlock����
		g_pVtxBuffBox->Unlock();

		if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) *g_NumIndexBox, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffBox, NULL)))
		{
			return E_FAIL;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
	{
		WORD *pIdx;

		g_pIdxBuffBox->Lock(0,0,(void**)&pIdx,0);
	
		{
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
		}


		g_pIdxBuffBox->Unlock();
	}


	return S_OK;
}


