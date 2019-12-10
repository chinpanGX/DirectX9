/*=============================================================================

	�r���{�[�h���� [billboard.cpp]
	Author : �o���đ�

==============================================================================*/

#include "billboard.h"
#include "Controller.h"
#include "camera.h"
#include "debugproc.h"

// �}�N����`
#define	BILLBOARD_SIZE_X		(20.0f)		// �r���{�[�h�̕�
#define	BILLBOARD_SIZE_Y		(20.0f)		// �r���{�[�h�̍���
#define	VALUE_MOVE_BILLBOARD	(0.30f)		// �ړ����x
#define	VALUE_JUMP				(10.0f)		// �W�����v��
#define	VALUE_GRAVITY			(0.45f)		// �d��
#define	RATE_REGIST				(0.075f)	// ��R�W��
#define	RATE_REFRECT			(-0.90f)	// ���ˌW��
#define	BILL_NUM_POY			(2)			//	�|���S��
#define	BILL_NUM_VER			(4)			//	���_


// �X�^�e�B�b�N�ϐ�
LPDIRECT3DVERTEXBUFFER9	BillBoard::m_pVtxBuffBillboard = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				BillBoard::m_mtxWorldBillboard;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				BillBoard::m_posBillboard;				// �ʒu
D3DXVECTOR3				BillBoard::m_sclBillboard;				// �X�P�[��
D3DXVECTOR3				BillBoard::m_moveBillboard;				// �ړ���
int						BillBoard::m_nIdxShadowBillboard;		// �eID
bool					BillBoard::m_bEnableGravity;			// �d�͂�L���ɂ��邩�ǂ���
LPDIRECT3DDEVICE9		BillBoard::m_pDevice;


// ����������
HRESULT BillBoard::Init()
{
	m_pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexBillboard(m_pDevice);

	// �e�N�X�`���̓ǂݍ���	����(�f�o�C�X�̃|�C���^�A�t�@�C�����A�ǂݍ��ރ������|�C���^)
	//D3DXCreateTextureFromFile(m_pDevice,"asset/texture/bullet000.png",&m_pTextureBillboard);	
	m_texture.Load("asset/texture/bullet000.png",1);

	//	�ʒu�A�X�P�[���A�ړ��ʂ̏����u
	m_posBillboard = D3DXVECTOR3(0.0f, 18.0f, 0.0f);
	m_sclBillboard = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_moveBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	//m_IdxShadow = m_shadow.Create(m_posBillboard, m_sclBillboard);

	//	�d�͂̃t���O
	m_bEnableGravity = false;

	return S_OK;
}

// �I������
void BillBoard::Uninit()
{
	m_texture.Unload(1);
	//SAFE_RELEASE(m_pTextureBillboard);	//	�e�N�X�`���̉��
	SAFE_RELEASE(m_pVtxBuffBillboard);	//	���_�o�b�t�@�̉��
}


// �X�V����
void BillBoard::Update()
{
	Camera *pCamera;

	// �J�����̎擾
	pCamera = GetCamera();

	// �W�����v
	if (KeyBoard::IsTrigger(DIK_J))
	{
		if(m_bEnableGravity == true)	//	�d�̓t���O���L���ȏꍇ
		{
			m_moveBillboard.y = VALUE_JUMP;	//VALUE_JUMP�̒l����
		}
	}

	if(m_bEnableGravity == true)	//	�d�̓t���O���L���ȏꍇ
	{
		m_posBillboard.y += m_moveBillboard.y;	//	m_moveBillboard.y�̒l�����Z���
		if(m_posBillboard.y < 9.0f)				//	9.0f��肤���ɏオ������
		{
			m_posBillboard.y = 9.0f;
			m_moveBillboard.y *= RATE_REFRECT;	//	���ˌW������Z���
		}
		m_moveBillboard.y -= VALUE_GRAVITY;		//	�d�͂̒l�����Z
	}


	// �d��ON/OFF
	if (KeyBoard::IsTrigger(DIK_G))
	{
		m_bEnableGravity ^= 1;
		if(m_bEnableGravity == false)
		{
			m_moveBillboard.y = 0.0f;
		}
	}

	D3DXVECTOR3 pos = m_posBillboard;
	pos.y = 0.0f;	//	�e�͍��W���Œ肵�Ă���->�e�̓W�����v���Ȃ�
	m_shadow.SetPosition(m_IdxShadow, pos);

	DebugProc::Print((char*)"*** �{�[������ ***\n");
	DebugProc::Print((char*)"[�{�[���̈ʒu  �F(%f : %f : %f)]\n", m_posBillboard.x, m_posBillboard.y, m_posBillboard.z);
	DebugProc::Print((char*)"[�{�[���̈ړ��ʁF(%f : %f : %f)]\n", m_moveBillboard.x, m_moveBillboard.y, m_moveBillboard.z);
	DebugProc::Print((char*)"�O�ړ�   : ��\n");
	DebugProc::Print((char*)"��ړ�   : ��\n");
	DebugProc::Print((char*)"���ړ�   : ��\n");
	DebugProc::Print((char*)"�E�ړ�   : ��\n");
	DebugProc::Print((char*)"�W�����v : J\n");
	DebugProc::Print((char*)"\n");
	DebugProc::Print((char*)"*** �d�� ***\n");
	DebugProc::Print((char*)"ON / OFF : [G] ( �d�� = ");

	if(m_bEnableGravity)
	{
		DebugProc::Print((char*)"ON )\n");
	}
	else
	{
		DebugProc::Print((char*)"OFF )\n");
	}
}

// �`�揈��
void BillBoard::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//	���e�X�g��L���ɂ���
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//	���C���e�B���O�𖳌��ɂ���
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//	���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldBillboard);

	//	�r���[�}�g���N�X���擾
	m_pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//	�|���S���𐳖ʂɌ�����
#if 0
	//	�t�s������߂�i��R�����̋t�s������߂�j
	D3DXMatrixInverse(&m_mtxWorldBillboard, NULL, &mtxView);

	//	���s�s��͖�������
	m_mtxWorldBillboard._41 = 0.0f;
	m_mtxWorldBillboard._42 = 0.0f;
	m_mtxWorldBillboard._43 = 0.0f;

#else
	//	�]�u�s��
	m_mtxWorldBillboard._11 = mtxView._11;
	m_mtxWorldBillboard._12 = mtxView._21;
	m_mtxWorldBillboard._13 = mtxView._31;
	m_mtxWorldBillboard._21 = mtxView._12;
	m_mtxWorldBillboard._22 = mtxView._22;
	m_mtxWorldBillboard._23 = mtxView._32;
	m_mtxWorldBillboard._31 = mtxView._13;
	m_mtxWorldBillboard._32 = mtxView._23;
	m_mtxWorldBillboard._33 = mtxView._33;
#endif

	//	�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, m_sclBillboard.x, m_sclBillboard.y, m_sclBillboard.z);
	D3DXMatrixMultiply(&m_mtxWorldBillboard, &m_mtxWorldBillboard, &mtxScale);

	//	�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_posBillboard.x, m_posBillboard.y, m_posBillboard.z);
	D3DXMatrixMultiply(&m_mtxWorldBillboard, &m_mtxWorldBillboard, &mtxTranslate);

	//	���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldBillboard);

	//	���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	//	�����i���_�o�b�t�@��ݒ肷��X�g���[��ID�A��2����:���_�o�b�t�@�ւ̃|�C���^�A�X�g���[���̐擪���璸�_�f�[�^�̐擪�܂ł̃I�t�Z�b�g�A���_�f�[�^�̈���̃f�[�^�T�C�Y���o�C�g�P�ʂŎw��j
	m_pDevice->SetStreamSource(0, m_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	//	���_�t�H�[�}�b�g��ݒ�
	m_pDevice->SetFVF(FVF_VERTEX3D);

	//	�e�N�X�`���̐ݒ�
	m_pDevice->SetTexture(0,m_texture.Set(1));

	//	�|���S���̕`��
	//	�����i�v���~�e�B�u�^�C�v�A�z��̓ǂݎ��J�n�ʒu�A�O�p�|���S���̐��j
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,BILL_NUM_POY);

	//	���C���e�B���O��L���ɂ���
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	//	���e�X�g�𖳌��ɂ���
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

// ���_���̍쐬
HRESULT BillBoard::MakeVertexBillboard(LPDIRECT3DDEVICE9 m_pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BILL_NUM_VER,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX3D,					// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,				// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&m_pVtxBuffBillboard,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))							// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);-

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffBillboard->Unlock();
	}

	return S_OK;
}

// ���_���W�̐ݒ�
void BillBoard::SetVertexBillboard(float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffBillboard->Unlock();
	}
}
