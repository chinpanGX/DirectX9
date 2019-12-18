//=============================================================================
//
// �G�l�~�[���� [enemy.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "shadow.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY			(5)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
class Enemy
{
private:
	static LPDIRECT3DTEXTURE9	m_pTextureModel;	// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH			m_pMeshModel;		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMatModel;	// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMatModel;		// �}�e���A�����̑���

	LPDIRECT3DDEVICE9	m_pDevice;
	D3DXMATRIX			m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3			m_pos;			// ���݂̈ʒu
	D3DXVECTOR3			m_rot;			// ���݂̌���
	D3DXVECTOR3			m_rotDest;		// �ړI�̌���
	D3DXVECTOR3			m_move;		// �ړ���
	int					m_idxShadow;
	bool				m_bUse;

	CIRCLE				m_circle;		// �����蔻��p�T�[�N��
	AABB				m_aabb;

public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	bool IsEnable(int index);
	CIRCLE* GetCircleCollision(int index);
	AABB * GetAABBCollision(int index);
	Enemy* GetEnemy();
};
