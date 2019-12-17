/*=============================================================================

	[player.h]
	Author : �o���đ�

=============================================================================*/
#pragma once

#include "main.h"
#include "shadow.h"
#include "bullet.h"

//	Player�N���X
class Player
{
private:
	static LPDIRECT3DTEXTURE9	m_pTextureModel;	//	�e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH			m_pMeshModel;		//	���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMatModel;	//	�}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMatModel;		//	�}�e���A�����̑���
	
	LPDIRECT3DDEVICE9			m_pDevice;			//	�f�o�C�X
	D3DXMATRIX					m_mtxWorldModel;		//	���[���h�}�g���b�N�X
	D3DXVECTOR3					m_pos;				//	���݂̈ʒu
	D3DXVECTOR3					m_rot;				//	���݂̌���
	D3DXVECTOR3					m_rotDest;			//	�ړI�̌���
	int							m_dir;				//	�����Ă������
	D3DXVECTOR3					m_move;				//	�ړ���
	int							m_IdxShadow;		//	�e������C���f�b�N�X�̔z��ԍ�
	bool						m_bEnableGravity;

	Bullet	m_bullet;
	Shadow	m_shadow;

public:
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();
};