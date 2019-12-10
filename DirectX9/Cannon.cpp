/*=============================================================================

	[Cannon.cpp]
	Author : �o���đ�

==============================================================================*/

#include "Cannon.h"
#include "Controller.h"
#include "camera.h"
#include "debugproc.h"

// �}�N����`
#define	BILLBOARD_SIZE_X		(20.0f)		// �r���{�[�h�̕�
#define	BILLBOARD_SIZE_Y		(20.0f)		// �r���{�[�h�̍���
#define	VALUE_MOVE				(0.30f)		// �ړ����x
#define	VALUE_GRAVITY			(0.45f)		// �d��
#define	RATE_REGIST				(0.075f)	// ��R�W��
#define	RATE_REFRECT			(-0.90f)	// ���ˌW��
#define	BILL_NUM_POY			(2)			//	�|���S��
#define	BILL_NUM_VER			(4)			//	���_
#define	MAX_CANNON				(100)

//	�O���[�o���ϐ�
Cannon	g_Cannon[MAX_CANNON];

// �X�^�e�B�b�N�ϐ�
LPDIRECT3DVERTEXBUFFER9	Cannon::m_pVtxBuffCannon = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				Cannon::m_mtxWorldCannon;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				Cannon::m_posCannon;				// �ʒu
D3DXVECTOR3				Cannon::m_sclCannon;				// �X�P�[��
D3DXVECTOR3				Cannon::m_moveCannon;				// �ړ���
int						Cannon::m_nIdxShadowCannon;			// �eID
bool					Cannon::m_bEnableGravity;			// �d�͂�L���ɂ��邩�ǂ���
LPDIRECT3DDEVICE9		Cannon::m_pDevice;


// ����������
HRESULT Cannon::Init()
{
	m_pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexCannon(m_pDevice);

	// �e�N�X�`���̓ǂݍ���	����(�f�o�C�X�̃|�C���^�A�t�@�C�����A�ǂݍ��ރ������|�C���^)
	//D3DXCreateTextureFromFile(m_pDevice,"asset/texture/bullet000.png",&m_pTextureCannon);	
	m_texture.Load("asset/texture/bullet000.png",1);

	for (int nCntCannon = 0; nCntCannon < MAX_CANNON; nCntCannon++)
	{
		//	�ʒu�A�X�P�[���A�ړ��ʂ̏����u
		g_Cannon[nCntCannon].m_posCannon  = D3DXVECTOR3(0.0f, 18.0f, 0.0f);
		g_Cannon[nCntCannon].m_sclCannon  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Cannon[nCntCannon].m_moveCannon = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		g_Cannon[nCntCannon].m_buse = false;
	}
	//m_IdxShadow = m_shadow.Create(m_posCannon, m_sclCannon);

	//	�d�͂̃t���O
	//m_bEnableGravity = false;

	return S_OK;
}

// �I������
void Cannon::Uninit()
{
	m_texture.Unload(1);
	//SAFE_RELEASE(m_pTextureCannon);	//	�e�N�X�`���̉��
	SAFE_RELEASE(m_pVtxBuffCannon);	//	���_�o�b�t�@�̉��
}


// �X�V����
void Cannon::Update()
{
	for (int nCntCannon = 0; nCntCannon < MAX_CANNON; nCntCannon++)
	{
		//	�g�p��
		if (g_Cannon[nCntCannon].m_buse == true)
		{
			g_Cannon[nCntCannon].m_frame++;	//	�t���[���X�V

			//	100�t���[���o��
			if (g_Cannon[nCntCannon].m_frame > 100)
			{
				m_shadow.Release(g_Cannon[nCntCannon].m_IdxShadow);
				g_Cannon[nCntCannon].m_buse = false;
			}

			//	���W�X�V
			g_Cannon[nCntCannon], m_posCannon += g_Cannon[nCntCannon].m_moveCannon;

			//	�e������
			D3DXVECTOR3 pos = g_Cannon[nCntCannon].m_posCannon;
			pos.y = 0.0f;	//	�e�͍��W���Œ肵�Ă���->�e�̓W�����v���Ȃ�
			m_shadow.SetPosition(m_IdxShadow, pos);
		}
	}
}

// �`�揈��
void Cannon::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//	���e�X�g��L���ɂ���
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//	���C�e�B���O�𖳌��ɂ���
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntCannon = 0; nCntCannon < MAX_CANNON; nCntCannon++)
	{
		//	�g�p��
		if (g_Cannon[nCntCannon].m_buse == false)
		{
			continue;
		}
		//	���[���h�}�g���N�X�̏�����
		D3DXMatrixIdentity(&g_Cannon[nCntCannon].m_mtxWorldCannon);

		//	�r���[�}�g���N�X���擾
		m_pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//	�|���S���𐳖ʂɌ�����
#if 0
	//	�s������߂�i��R�����̋t�s������߂�j
		D3DXMatrixInverse(&m_mtxWorldCannon, NULL, &mtxView);

		//	���s�s��͖�������
		m_mtxWorldCannon._41 = 0.0f;
		m_mtxWorldCannon._42 = 0.0f;
		m_mtxWorldCannon._43 = 0.0f;

#else
	//	�]�u�s��
		m_mtxWorldCannon._11 = mtxView._11;
		m_mtxWorldCannon._12 = mtxView._21;
		m_mtxWorldCannon._13 = mtxView._31;
		m_mtxWorldCannon._21 = mtxView._12;
		m_mtxWorldCannon._22 = mtxView._22;
		m_mtxWorldCannon._23 = mtxView._32;
		m_mtxWorldCannon._31 = mtxView._13;
		m_mtxWorldCannon._32 = mtxView._23;
		m_mtxWorldCannon._33 = mtxView._33;
#endif

		//	�X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScale, g_Cannon[nCntCannon].m_sclCannon.x, g_Cannon[nCntCannon].m_sclCannon.y, g_Cannon[nCntCannon].m_sclCannon.z);
		D3DXMatrixMultiply(&g_Cannon[nCntCannon].m_mtxWorldCannon, &g_Cannon[nCntCannon].m_mtxWorldCannon, &mtxScale);

		//	�ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, g_Cannon[nCntCannon].m_posCannon.x, g_Cannon[nCntCannon].m_posCannon.y, g_Cannon[nCntCannon].m_posCannon.z);
		D3DXMatrixMultiply(&g_Cannon[nCntCannon].m_mtxWorldCannon, &g_Cannon[nCntCannon].m_mtxWorldCannon, &mtxTranslate);

		//	���[���h�}�g���b�N�X�̐ݒ�
		m_pDevice->SetTransform(D3DTS_WORLD, &g_Cannon[nCntCannon].m_mtxWorldCannon);

		//	���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		//	�����i���_�o�b�t�@��ݒ肷��X�g���[��ID�A��2����:���_�o�b�t�@�ւ̃|�C���^�A�X�g���[���̐擪���璸�_�f�[�^�̐擪�܂ł̃I�t�Z�b�g�A���_�f�[�^�̈���̃f�[�^�T�C�Y���o�C�g�P�ʂŎw��j
		m_pDevice->SetStreamSource(0, m_pVtxBuffCannon, 0, sizeof(VERTEX_3D));

		//	���_�t�H�[�}�b�g��ݒ�
		m_pDevice->SetFVF(FVF_VERTEX3D);

		//	�e�N�X�`���̐ݒ�
		m_pDevice->SetTexture(0, m_texture.Set(1));

		//	�|���S���̕`��
		//	�����i�v���~�e�B�u�^�C�v�A�z��̓ǂݎ��J�n�ʒu�A�O�p�|���S���̐��j
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, BILL_NUM_POY);
		
	}
	//	���C���e�B���O��L���ɂ���
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	//	���e�X�g�𖳌��ɂ���
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//�e�̐���
void Cannon::Create(float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < MAX_CANNON; i++)
	{

		// �����e�����łɗL����������H
		if (g_Cannon[i].m_buse)
		{
			continue;
		}

		// �e�̍��W�Ɉ����̒l��������
		g_Cannon[i].m_posCannon.x = x;
		g_Cannon[i].m_posCannon.y = 0.0f;
		g_Cannon[i].m_posCannon.z = y;

#if 0
		// �e�̈ړ��������w�肷��
		D3DXVec2Normalize(&dir, &dir);
		g_Cannon[i].m_moveCannon.x = dir.x;
		g_Cannon[i].m_moveCannon.y = dir.y;
		g_Cannon[i].m_posCannon.z = dir.y;

		// �e�̈ړ���������e�̉�]�p�x�����߂�
		g_Bullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		g_Bullets[i].collision.cx = x;
		g_Bullets[i].collision.cy = y;
		g_Bullets[i].collision.r = BULLET_WIDTH * 0.8f;
#endif
		// �L���t���[����������
		g_Cannon[i].m_frame = 0;

		g_Cannon[i].m_IdxShadow =m_shadow.Create(g_Cannon[i].m_posCannon,D3DXVECTOR3(0.0f,0.0f,1.0f));

		// �e��L���ɂ���
		g_Cannon[i].m_buse = true;

		break;
	}
}


// ���_���̍쐬
HRESULT Cannon::MakeVertexCannon(LPDIRECT3DDEVICE9 m_pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BILL_NUM_VER,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,				// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX3D,					// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,				// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&m_pVtxBuffCannon,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))							// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffCannon->Lock(0, 0, (void**)&pVtx, 0);

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
		m_pVtxBuffCannon->Unlock();
	}

	return S_OK;
}

// ���_���W�̐ݒ�
void Cannon::SetVertexCannon(float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		m_pVtxBuffCannon->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		m_pVtxBuffCannon->Unlock();
	}
}
            