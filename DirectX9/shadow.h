/*=============================================================================

	[Shadow.h]
	Author : �o���đ�

=============================================================================*/
#pragma once

#include "main.h"
#include "texture.h"

class  Shadow
{
private:
	static LPDIRECT3DVERTEXBUFFER9  m_pVtxBuffShadow;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DDEVICE9				m_pDevice;
	D3DXMATRIX						m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3						m_pos;				// �ʒu
	D3DXVECTOR3						m_rot;				// ��]
	bool							m_bUse;				// �g�p���Ă��邩�ǂ���
	Texture							m_texture;
public:
	HRESULT Init();
	void	Uninit();
	void	Update();
	void	Draw();
	int		Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
	void	Release(int nIdxShadow);
	void	SetPosition(int nIdxShadow, D3DXVECTOR3 pos);
	void	SetColor(int nIdxShadow, D3DXCOLOR col);
private:
	void	SetVertex(int nIdxShadow, float fSizeX, float fSizeZ);
	HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
};

 