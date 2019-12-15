/*=============================================================================

	[Shadow.h]
	Author : 出合翔太

=============================================================================*/
#pragma once

#include "main.h"
#include "texture.h"

class Shadow
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffShadow;	// 頂点バッファへのポインタ
	LPDIRECT3DDEVICE9				m_pDevice;
	D3DXMATRIX						m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3						m_pos;				// 位置
	D3DXVECTOR3						m_rot;				// 回転
	D3DXVECTOR3						m_scl;				// 拡大縮小
	bool							m_bUse;				// 使用しているかどうか

	Texture		m_texture;

public:
	HRESULT Init();
	void	Uninit();
	void	Update();
	void	Draw();
	int		Create(D3DXVECTOR3 pos ,D3DXVECTOR3 scl);
	void	Release(int nIdxShadow);
	void	SetPosition(int nIdxShadow,D3DXVECTOR3 pos);
	void	SetScale(int nIdxShadow, D3DXVECTOR3 scl);

private:
	HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
};