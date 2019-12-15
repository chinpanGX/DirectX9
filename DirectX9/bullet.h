//=============================================================================
//
// �e���� [bullet.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "texture.h"
#include "shadow.h"

class Bullet
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBullet;	// ���_�o�b�t�@�ւ̃|�C���^

	LPDIRECT3DDEVICE9		m_pDevice;
	D3DXMATRIX				m_mtxWorldBullet;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_pos;				// �ʒu
	D3DXVECTOR3				m_scl;				// �X�P�[��
	D3DXVECTOR3				m_move;				// �ړ���
	int						m_nIdxShadowBullet;	// �eID
	int						m_frame;
	bool					m_bUse;

	Texture		m_texture;
	Shadow		m_shadow;

public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Create(float x, float y, D3DXVECTOR2 dir);

private:
	HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexBullet(float fSizeX, float fSizeY);
};




