/*==================================

	[main.h]
	Author : �o���đ�

==================================*/

#pragma once

//#	�C���N���[�h�t�@�C��
#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>

//#	�}�N����`
#define SCREEN_WIDTH  (1920)	// �X�N���[��(�N���C�A���g�̈�)�̕�
#define SCREEN_HEIGHT (1080)    // �X�N���[��(�N���C�A���g�̈�)�̍���

//!	�����[�X�ł̓t���X�N���[���iFALSE�j�ɂ���	
#define WINMODE	TRUE			//�E�B���h�E���[�h�̎w��iTRUE�F�E�B���h�E���[�h/FALSE:�t���X�N���[���j

//!	DirectX�f�o�C�X����}�N��
#define DEVICE_RELEASE(p) do {if(p){(p)->Release(); (p) = NULL;} } while(0)

//$ #define	NUM_VERTEX		(4)				// ���_��
//$ #define	NUM_POLYGON		(2)				// �|���S����


//# ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float rhw;			// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_2D;

//#	�Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )		2D�|���S�����X�N���[�����W�ϊ�
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���_�o�b�t�@��FVF�`���̎w��

//# ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_3D;

// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//�@�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetD3DDevice();	//Direct3DDevice�̃Q�b�^�[
HWND GetHWND();						//�E�B���h�E�n���h����Getter
double frand();
