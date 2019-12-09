/*=============================================================================

	[Shadow.h]
	Author : 出合翔太

=============================================================================*/
#pragma once

#include "main.h"
#include "texture.h"

class  Shadow
{
private:
	static LPDIRECT3DVERTEXBUFFER9  m_pVtxBuffShadow;	// 頂点バッファへのポインタ
	LPDIRECT3DDEVICE9				m_pDevice;
	D3DXMATRIX						m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3						m_pos;				// 位置
	D3DXVECTOR3						m_rot;				// 回転
	bool							m_bUse;				// 使用しているかどうか
	Texture							m_texture;
public:
	HRESULT Init();
	void	Uninit();
	void	Update();
	void	Draw();
	int		Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
	void	Release(int nIdxShadow);
	void	SetPosition(int nIdxShadow, D3DXVECTOR3 pos);
	void	SetColor(int nIdxShadow, D3DXCOLOR col);
private:
	void	SetVertex(int nIdxShadow, float fSizeX, float fSizeZ);
	HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
};

 