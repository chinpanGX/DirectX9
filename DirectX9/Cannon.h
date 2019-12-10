//=============================================================================
//
// �r���{�[�h���� [Cannon.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "texture.h"
#include "shadow.h"

class Cannon
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffCannon;		// ���_�o�b�t�@�ւ̃|�C���^
	static D3DXMATRIX				m_mtxWorldCannon;		// ���[���h�}�g���b�N�X
	static D3DXVECTOR3				m_posCannon;				// �ʒu
	static D3DXVECTOR3				m_sclCannon;				// �X�P�[��
	static D3DXVECTOR3				m_moveCannon;			// �ړ���
	static int						m_nIdxShadowCannon;		// �eID
	static bool						m_bEnableGravity;			// �d�͂�L���ɂ��邩�ǂ���
	static LPDIRECT3DDEVICE9		m_pDevice;
	Texture							m_texture;
	Shadow							m_shadow;
	int								m_IdxShadow;				//	�e�̔z��ԍ����i�[
	bool							m_buse;
	int								m_frame;
public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Create(float x, float y, D3DXVECTOR2 dir);
private:
	HRESULT MakeVertexCannon(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexCannon(float fSizeX, float fSizeY);
};
