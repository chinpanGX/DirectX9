/*=============================================================================

	[player.h]
	Author : �o���đ�

=============================================================================*/
#pragma once

#include "main.h"

//	Player�N���X
class Player
{
private:
	static LPDIRECT3DTEXTURE9	m_pTextureModel;	// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH			m_pMeshModel;		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMatModel;	// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMatModel;		// �}�e���A�����̑���
	static LPDIRECT3DDEVICE9	m_pDevice;
	D3DXMATRIX			mtxWorldModel;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3			posModel;			// ���݂̈ʒu
	D3DXVECTOR3			rotModel;			// ���݂̌���
	D3DXVECTOR3			rotDestModel;		// �ړI�̌���
	D3DXVECTOR3			moveModel;			// �ړ���
public:
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();
};