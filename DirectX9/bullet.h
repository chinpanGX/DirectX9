//=============================================================================
//
// 弾処理 [bullet.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "texture.h"
#include "shadow.h"
#include "collision.h"

#define MAX_BULLET			100

class Bullet
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBullet;	// 頂点バッファへのポインタ

	LPDIRECT3DDEVICE9		m_pDevice;
	D3DXMATRIX				m_mtxWorldBullet;	// ワールドマトリックス
	D3DXVECTOR3				m_pos;				// 位置
	D3DXVECTOR3				m_scl;				// スケール
	D3DXVECTOR3				m_move;				// 移動量
	int						m_frame;

	Texture		m_texture;
	Shadow		m_shadow;
	CIRCLE		col_circle;		// 当たり判定用サークル
	AABB		col_aabb;

public:
	
	int						m_nIdxShadowBullet;	// 影ID
	bool					m_bUse;
	
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Create(float x, float y, float z, D3DXVECTOR3 dir);
	bool IsEnable(int index);

	CIRCLE* GetCollision(int index);
	AABB* GetCollisionBox(int index);
	Bullet* GetBullet(void);

private:
	HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexBullet(float fSizeX, float fSizeY);
};




