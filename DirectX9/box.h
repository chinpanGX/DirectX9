
#pragma once
#include "main.h"
#include "texture.h"

class Box
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBox;		//	���_�o�b�t�@�ւ̃|�C���^
	static LPDIRECT3DINDEXBUFFER9	m_pIdxBuffBox;		//	�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	static int						m_NumVertexBox;		//	���_�̐�
	static D3DXVECTOR3				m_posBox;			//	�n�ʂ̈ʒu
	static D3DXVECTOR3				m_rot1Box;			//	�n�ʂ̌���(��])
	static D3DXVECTOR3				m_rot2Box;			//	�n�ʂ̌���(��])
	static D3DXVECTOR3				m_sclBox;			//	�n�ʂ̑傫��(�X�P�[��)
	static LPDIRECT3DDEVICE9		m_pDevice;
	D3DXMATRIX						m_mtxWorldBox;		//	���[���h�}�g���b�N�X
	Texture							m_texture;
public:
	HRESULT Init();
	void	Uninit();
	void	Update();
	void	Draw();
private:
	HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice);
	void DrawPolygon(int i);
};