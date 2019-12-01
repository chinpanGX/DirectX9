/*======================================================

	[debugproc.cpp]
	Author : 出合翔太

=======================================================*/

#include <stdio.h>
#include "debugproc.h"

LPD3DXFONT			DebugProc::m_pD3DXFont = NULL;			// フォントへのポインタ
char				DebugProc::m_aStrDebug[1024] = { "\0" };	// デバッグ情報
LPDIRECT3DDEVICE9	DebugProc::m_pDevice;

//	初期化処理
HRESULT DebugProc::Init()
{
	m_pDevice = GetD3DDevice();
	HRESULT hr;

	// 情報表示用フォントを設定
	hr = D3DXCreateFont(m_pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pD3DXFont);

	// 情報クリア
	memset(m_aStrDebug, 0, sizeof m_aStrDebug);

	return hr;
}

//	終了処理
void DebugProc::Uninit()
{
	SAFE_RELEASE(m_pD3DXFont);
}

//	描画処理
void DebugProc::Draw()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// 情報表示
	m_pD3DXFont->DrawText(NULL, m_aStrDebug, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// 情報クリア
	memset(m_aStrDebug, 0, sizeof m_aStrDebug);
}

//	表示登録処理
void DebugProc::Print(char * fmt, ...)
{
	va_list list;			// 可変引数を処理する為に使用する変数
	char *pCur;
	char aBuf[256] = { "\0" };
	char aWk[32];

	// 可変引数にアクセスする前の初期処理
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
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf_s(aWk, "%d", va_arg(list, int));
				break;

			case 'f':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf_s(aWk, "%.2f", va_arg(list, double));		// double型で指定
				break;

			case 's':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf_s(aWk, "%s", va_arg(list, char*));
				break;

			case 'c':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf_s(aWk, "%c", va_arg(list, char));
				break;

			default:
				sprintf_s(aWk, "%c", *pCur);
				break;
			}
		}
		strcat_s(aBuf, aWk);
	}

	// 可変引数にアクセスした後の終了処理
	va_end(list);

	// 連結
	if ((strlen(m_aStrDebug) + strlen(aBuf)) < ((sizeof m_aStrDebug) - 1))
	{
		strcat_s(m_aStrDebug, aBuf);
	}
}
