//=============================================================================
//
// �r���{�[�h���� [billboard.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "texture.h"
#include "shadow.h"

class BillBoard
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBillboard;		// ���_�o�b�t�@�ւ̃|�C���^
	static D3DXMATRIX				m_mtxWorldBillboard;		// ���[���h�}�g���b�N�X
	static D3DXVECTOR3				m_posBillboard;				// �ʒu
	static D3DXVECTOR3				m_sclBillboard;				// �X�P�[��
	static D3DXVECTOR3				m_moveBillboard;			// �ړ���
	static int						m_nIdxShadowBillboard;		// �eID
	static bool						m_bEnableGravity;			// �d�͂�L���ɂ��邩�ǂ���
	static LPDIRECT3DDEVICE9		m_pDevice;
	Texture							m_texture;
	int								m_IdxShadow;				//	�e�̔z��ԍ����i�[
public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
private:
	HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexBillboard(float fSizeX, float fSizeY);
};
