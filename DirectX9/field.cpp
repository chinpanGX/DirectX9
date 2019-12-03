/*===============================================

	[Field.cpp]
	Author : �o���đ�

===============================================*/

#include "main.h"
#include "field.h"
#include "texture.h"
#include "debugproc.h"

//	�}�N����`
#define SQUARE_LENGTH (100)					//	�`�悷��n�ʂ̐�
#define SQUARE_NUM  (SQUARE_LENGTH + 1)		//	�k�ރ|���S�����܂߂��n�ʂ̐�
#define VERTEX (pow(SQUARE_NUM,2)*2)		//	���_�̐�

//	�X�^�e�B�b�N�ϐ�
PDIRECT3DVERTEXBUFFER9		Field::m_pVtxBuffer;	//	���_�o�b�t�@�ւ̃|�C���^
PDIRECT3DINDEXBUFFER9		Field::m_pIdxBuffer;	//	���_�o�b�t�@�ւ̃|�C���^
PDIRECT3DDEVICE9			Field::m_pDevice;		//	�f�o�C�X
D3DXVECTOR3					Field::m_posField;		//	�n�ʂ̈ʒu
D3DXVECTOR3					Field::m_rotField;		//	�n�ʂ̌���(��])
D3DXVECTOR3					Field::m_sclField;		//	�n�ʂ̑傫��(�X�P�[��)

//	����������
HRESULT Field::Init()
{
	m_pDevice = GetD3DDevice();
	
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	m_posField = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	m_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	// ���_���̍쐬
	MakeVertexField(m_pDevice);

	return S_OK;
}

//	�I������
void Field::Uninit()
{
	SAFE_RELEASE(m_pVtxBuffer);
	SAFE_RELEASE(m_pIdxBuffer);
}

//	�`�揈��
void Field::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	//	�����_�����O�X�e�[�g�̐ݒ�@
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//�t�B�����[�h,���C���[�t���[���̐ݒ�
	m_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	//	���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//	�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_sclField.x, m_sclField.y, m_sclField.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//	��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotField.x, m_rotField.y, m_rotField.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//	�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_posField.x, m_posField.y, m_posField.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	//	���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//	���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	//	�����i���_�o�b�t�@��ݒ肷��X�g���[��ID�A��2����:���_�o�b�t�@�ւ̃|�C���^�A�X�g���[���̐擪���璸�_�f�[�^�̐擪�܂ł̃I�t�Z�b�g�A���_�f�[�^�̈���̃f�[�^�T�C�Y���o�C�g�P�ʂŎw��j
	m_pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//	�C���f�b�N�X�o�b�t�@�̃Z�b�g
	m_pDevice->SetIndices(m_pIdxBuffer);
	
	m_pDevice->SetFVF(FVF_VERTEX3D);

	//	�e�N�X�`���̐ݒ�
	m_pDevice->SetTexture(0, Texture::GetTexture(TEXTURE_INDEX_FIELD01));

	//	�|���S���̕`��
	//	�����i�v���~�e�B�u�^�C�v�A�C���f�b�N�X�o�b�t�@�̊J�n�n�_����ŏ��̃C���f�b�N�X�܂ł̃I�t�Z�b�g�A�ŏ����_�ԍ��A�C���f�b�N�X�̐��A�z��̓ǂݎ��J�n�ʒu�A�O�p�|���S���̐��j
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VERTEX, 0, VERTEX - 4);

	//	������̕`��
	DebugProc::Print((char*)"*** �R�c�|���S������ ***\n");
	DebugProc::Print((char*)"�ʒu [%f : %f : %f]\n", m_posField.x, m_posField.y, m_posField.z);
	DebugProc::Print((char*)"�O�ړ� : �v\n");
	DebugProc::Print((char*)"��ړ� : �r\n");
	DebugProc::Print((char*)"���ړ� : �`\n");
	DebugProc::Print((char*)"�E�ړ� : �c\n");
	DebugProc::Print((char*)"\n");
	DebugProc::Print((char*)"���� [%f : %f : %f]\n", m_rotField.x, m_rotField.y, m_rotField.z);
	DebugProc::Print((char*)"X����] : �� / ��\n");
	DebugProc::Print((char*)"Y����] : �p / �d\n");
	DebugProc::Print((char*)"Z����] : �� / ��\n");
	DebugProc::Print((char*)"\n");
	DebugProc::Print((char*)"�ʒu�E�������Z�b�g : ENTER\n");
}

//	���_�쐬
HRESULT Field::MakeVertexField(LPDIRECT3DDEVICE9 m_pDevice)
{
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX / 2,	//	���_�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,														//	�����[�X�����\���t���O
		FVF_VERTEX3D,															//	���_�t�H�[�}�b�g�̐ݒ�
		D3DPOOL_MANAGED,														//	�g�p���郁�����iD3DPOLL�񋓌^�j
		&m_pVtxBuffer,															//	�����������_�o�b�t�@���󂯎��|�C���^�̃A�h���X
		NULL)))
	{
		return E_FAIL;
		//	���]�b�g���s�̃G���[�l	�萔�Ɖ��
		//	D3DERR_INVALIDCALL		->	���\�b�h�̌Ăяo���������ł���
		//	D3DERR_OUTOFVIDEOMEMORY	->	�\���ȃf�B�X�v���C�����������݂��Ȃ�
		//	E_OUTOFMEMORY			->	�\���ȃ����������蓖�Ă邱�Ƃ��ł��Ȃ�
	}

	{
		//	���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;	//	���_�f�[�^�ւ̃|�C���^�A�h���X

		//	Lock�֐�
		//	�����i���b�N���钸�_�f�[�^�̃I�t�Z�b�g���w��A���b�N����͈͂��o�C�g�P�ʂŎw��A���_�f�[�^�ւ̃|�C���^�̃A�h���X���L���X�g(void**)���Ă��w��A���b�N�̕��@��\���t���O�j
		m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);		//	���_�f�[�^�̎w��͈͂����b�N���A�o�b�t�@�ւ̃|�C���^���擾���܂�

		//	���_�̐ݒ�
		for (int z = 0; z < SQUARE_NUM; z++)
		{
			for (int x = 0; x < SQUARE_NUM; x++) 
			{
				pVtx[z * SQUARE_NUM + x].pos = D3DXVECTOR3(x * 10.0f - 500.0f,0.0f, z * -10.0f + 100.0f);	//	�ʒu
				pVtx[z * SQUARE_NUM + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								//	�@���x�N�g��
				pVtx[z * SQUARE_NUM + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);							//	���_�J���[
				pVtx[z * SQUARE_NUM + x].tex.x = (float)x;													//	�e�N�X�`����x���W
				pVtx[z * SQUARE_NUM + x].tex.y = (float)z;													//	�e�N�X�`����y���W
			}
		}

		//	Unlock�֐�	
		m_pVtxBuffer->Unlock();	//	�f�[�^��VRAM�֕Ԃ�
	}

	if (FAILED(m_pDevice->CreateIndexBuffer(sizeof(WORD) * VERTEX,	//	�z���WORD�^�Ȃ̂�,�w��t�H�[�}�b�g��2��ނ����Ȃ�����B�iWORD�^��16�r�b�g�j
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,												//	�w��ł���̂�D3DFMT_INDEX16(16�r�b�g)��D3DFMT_INDEX32(32�r�b�g)�̂�
		D3DPOOL_MANAGED,
		&m_pIdxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	{
		//	�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

		//	for�����킩��₷�����邽�߂̕ϐ�
		int cnt1 = SQUARE_NUM;
		int cnt2 = 0;
		int DegenerateCnt = 1;	//	�k�ރ|���S���̃J�E���g

		for (int i = 0; i < VERTEX; i++)
		{
			//	�k�ރ|���S����������
			if (i == ((SQUARE_LENGTH + 1) * 2 + 2) * DegenerateCnt - 2 && i != 0)
			{
				pIdx[i] = pIdx[i - 1];  //	�O�̒��_�ɔ킹��
				pIdx[i + 1] = cnt1;		//	��̒��_
				pIdx[i + 2] = cnt1;		//	�O�̒��_�ɔ킹��
				i += 2;					//	���_�]���ɑ�������̂�+2
				DegenerateCnt++;		//	�k�ރ|���S���̉񐔂𑝂₷
			}
			//	�k�ރ|���S���������Ȃ��Ƃ�(i������������)
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

		m_pVtxBuffer->Unlock();
	}
	return S_OK;
}

