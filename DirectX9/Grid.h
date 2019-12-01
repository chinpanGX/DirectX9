#pragma once
#include "main.h"
class Grid
{
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;
	D3DXMATRIX m_mat;
	VERTEX_3D pVtx[2];
	HRESULT MakeVertexField();
	void Set(int i, int j);
public:
	void Init();
	void Draw();
};

