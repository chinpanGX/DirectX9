//=============================================================================
//
// 弾処理 [bullet.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "texture.h"
#include "shadow.h"

class Bullet
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBullet;	// 頂点バッファへのポインタ

	LPDIRECT3DDEVICE9		m_pDevice;
	D3DXMATRIX				m_mtxWorldBullet;	// ワールドマトリックス
	D3DXVECTOR3				m_pos;				// 位置
	D3DXVECTOR3				m_scl;				// スケール
	D3DXVECTOR3				m_move;				// 移動量
	int						m_nIdxShadowBullet;	// 影ID
	int						m_frame;
	bool					m_bUse;

	Texture		m_texture;
	Shadow		m_shadow;

public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Create(float x, float y, D3DXVECTOR2 dir);

private:
	HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexBullet(float fSizeX, float fSizeY);
};




