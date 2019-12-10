/*=======================================================

	[BulletManager.h]
	Author : �o���đ�

========================================================*/

#pragma once
#include "main.h"

#include "texture.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"

//	�}�N����`
#define	SIZE_X		(20.0f)	// �r���{�[�h�̕�
#define	SIZE_Y		(20.0f)	// �r���{�[�h�̍���
#define MAX_BULLET	(512)	//	�ő吔


class Bullet
{
public:
	D3DXVECTOR3	m_position;			//	�ʒu
	D3DXVECTOR3	m_scale;			//	�X�P�[��
	D3DXVECTOR3	m_velocity;			//	�ړ���
	bool		m_bEnableGravity;	//	�d�͂�L���ɂ��邩�ǂ���
	float		m_fSizeX;			//	��
	float		m_fSizeY;			//	����
	int			m_life;				//	����
	bool		m_bUse;				//	�g�p���Ă��邩�ǂ���
	Shadow		m_shadow;
};

class BulletManager
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBullet;		//	���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DDEVICE9				m_pDevice;
	D3DXMATRIX						m_mtxWorld;				//	���[���h�}�g���b�N�X
	D3DXVECTOR3						m_posBase;				//	�r���{�[�h�����ʒu
	float							m_fWidthBase = 5.0f;	//	��̕�
	float							m_fHeightBase = 10.0f;	//	��̍���
	float							m_roty = 0.0f;			//	�ړ�����
	float							m_spd = 0.0f;			//	�ړ��X�s�[�h
	int								m_IdxShadow;			//	�eID
	Bullet							m_aBullet[MAX_BULLET];	//	BulletWork
	Texture							m_texture;
	Camera							*m_camera;
	Shadow							m_shadow;
	Player							m_player;
public:
	HRESULT Init();
	void	Uninit();
	void	Update();
	void	Draw();
private:
	HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexBullet(int nIdx, float fSizeX, float fSizeY);
	int SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nLife);
};

