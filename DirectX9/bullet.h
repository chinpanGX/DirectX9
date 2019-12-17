//=============================================================================
//
// �e���� [bullet.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "texture.h"
#include "shadow.h"
#include "collision.h"

#define MAX_BULLET			100

class Bullet
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBullet;	// ���_�o�b�t�@�ւ̃|�C���^

	LPDIRECT3DDEVICE9		m_pDevice;
	D3DXMATRIX				m_mtxWorldBullet;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_pos;				// �ʒu
	D3DXVECTOR3				m_scl;				// �X�P�[��
	D3DXVECTOR3				m_move;				// �ړ���
	int						m_frame;

	Texture		m_texture;
	Shadow		m_shadow;
	CIRCLE		col_circle;		// �����蔻��p�T�[�N��
	AABB		col_aabb;

public:
	
	int						m_nIdxShadowBullet;	// �eID
	bool					m_bUse;
	
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Create(float x, float y, float z, D3DXVECTOR3 dir);
	bool IsEnable(int index);

	CIRCLE* GetCollision(int index);
	AABB* GetCollisionBox(int index);
	Bullet* GetBullet(void);

private:
	HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexBullet(float fSizeX, float fSizeY);
};




