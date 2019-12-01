/*======================================================

	[debugproc.cpp]
	Author : �o���đ�

=======================================================*/

#include <stdio.h>
#include "debugproc.h"

LPD3DXFONT			DebugProc::m_pD3DXFont = NULL;			// �t�H���g�ւ̃|�C���^
char				DebugProc::m_aStrDebug[1024] = { "\0" };	// �f�o�b�O���
LPDIRECT3DDEVICE9	DebugProc::m_pDevice;

//	����������
HRESULT DebugProc::Init()
{
	m_pDevice = GetD3DDevice();
	HRESULT hr;

	// ���\���p�t�H���g��ݒ�
	hr = D3DXCreateFont(m_pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pD3DXFont);

	// ���N���A
	memset(m_aStrDebug, 0, sizeof m_aStrDebug);

	return hr;
}

//	�I������
void DebugProc::Uninit()
{
	SAFE_RELEASE(m_pD3DXFont);
}

//	�`�揈��
void DebugProc::Draw()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// ���\��
	m_pD3DXFont->DrawText(NULL, m_aStrDebug, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// ���N���A
	memset(m_aStrDebug, 0, sizeof m_aStrDebug);
}

//	�\���o�^����
void DebugProc::Print(char * fmt, ...)
{
	va_list list;			// �ψ�������������ׂɎg�p����ϐ�
	char *pCur;
	char aBuf[256] = { "\0" };
	char aWk[32];

	// �ψ����ɃA�N�Z�X����O�̏�������
	va_start(list, fmt);

	pCur = fmt;
	for (; *pCur; ++pCur)
	{
		if (*pCur != '%')
		{
			sprintf_s(aWk, "%c", *pCur);
		}
		else
		{
			pCur++;

			switch (*pCur)
			{
			case 'd':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf_s(aWk, "%d", va_arg(list, int));
				break;

			case 'f':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf_s(aWk, "%.2f", va_arg(list, double));		// double�^�Ŏw��
				break;

			case 's':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf_s(aWk, "%s", va_arg(list, char*));
				break;

			case 'c':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf_s(aWk, "%c", va_arg(list, char));
				break;

			default:
				sprintf_s(aWk, "%c", *pCur);
				break;
			}
		}
		strcat_s(aBuf, aWk);
	}

	// �ψ����ɃA�N�Z�X������̏I������
	va_end(list);

	// �A��
	if ((strlen(m_aStrDebug) + strlen(aBuf)) < ((sizeof m_aStrDebug) - 1))
	{
		strcat_s(m_aStrDebug, aBuf);
	}
}
