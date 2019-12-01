/*======================================================

	[debugproc.h]
	Author : 出合翔太

=======================================================*/
#pragma once
#include "main.h"

class DebugProc
{
private:
	static LPD3DXFONT			m_pD3DXFont;			// フォントへのポインタ
	static char					m_aStrDebug[1024];	// デバッグ情報
	static LPDIRECT3DDEVICE9	m_pDevice;
public:
	static	HRESULT Init();
	static void Uninit();
	static void Draw();
	static void Print(char *fmt, ...);
};