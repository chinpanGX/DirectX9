/*=============================================================================

	[Shadow.h]
	Author : �o���đ�

=============================================================================*/
#pragma once

#include "main.h"
#include "texture.h"

class Shadow
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffShadow;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DDEVICE9				m_pDevice;
	D3DXMATRIX						m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3						m_pos;				// �ʒu
	D3DXVECTOR3						m_rot;				// ��]
	D3DXVECTOR3						m_scl;				// �g��k��
	bool							m_bUse;				// �g�p���Ă��邩�ǂ���

	Texture		m_texture;

public:
	HRESULT Init();
	void	Uninit();
	void	Update();
	void	Draw();
	int		Create(D3DXVECTOR3 pos ,D3DXVECTOR3 scl);
	void	Release(int nIdxShadow);
	void	SetPosition(int nIdxShadow,D3DXVECTOR3 pos);
	void	SetScale(int nIdxShadow, D3DXVECTOR3 scl);

private:
	HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
};