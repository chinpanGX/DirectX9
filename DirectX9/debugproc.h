/*======================================================

	[debugproc.h]
	Author : �o���đ�

=======================================================*/
#pragma once
#include "main.h"

class DebugProc
{
private:
	static LPD3DXFONT			m_pD3DXFont;			// �t�H���g�ւ̃|�C���^
	static char					m_aStrDebug[1024];	// �f�o�b�O���
	static LPDIRECT3DDEVICE9	m_pDevice;
public:
	static	HRESULT Init();
	static void Uninit();
	static void Draw();
	static void Print(char *fmt, ...);
};