/*==============================================

	[field.h]
	Author : �o���đ�

===============================================*/

#pragma once
#include "main.h"

//	Field�N���X
class Field
{
private:
	static	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffer;	//	���_�o�b�t�@�ւ̃|�C���^
	static	LPDIRECT3DINDEXBUFFER9	m_pIdxBuffer;	//	���_�o�b�t�@�ւ̃|�C���^
	static	LPDIRECT3DDEVICE9		m_pDevice;		//	�f�o�C�X
	D3DXMATRIX						m_mtxWorld;		//	���[���h�}�g���b�N�X
	static	D3DXVECTOR3				m_posField;		//	�n�ʂ̈ʒu
	static	D3DXVECTOR3				m_rotField;		//	�n�ʂ̌���(��])
	static	D3DXVECTOR3				m_sclField;		//	�n�ʂ̑傫��(�X�P�[��)
public:
	HRESULT	Init();											//	����������
	void	Uninit();										//	�I������
	void	Draw();											//	�`�揈��
private:
	HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);		//	���_�쐬
};
