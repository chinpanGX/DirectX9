//=============================================================================
//
// エネミー処理 [enemy.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "collision.h"
#include "shadow.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY			(5)


//*****************************************************************************
// 構造体定義
//*****************************************************************************
class Enemy
{
private:
	static LPDIRECT3DTEXTURE9	m_pTextureModel;	// テクスチャへのポインタ
	static LPD3DXMESH			m_pMeshModel;		// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMatModel;	// マテリアル情報へのポインタ
	static DWORD				m_nNumMatModel;		// マテリアル情報の総数

	LPDIRECT3DDEVICE9	m_pDevice;
	D3DXMATRIX			m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3			m_pos;			// 現在の位置
	D3DXVECTOR3			m_rot;			// 現在の向き
	D3DXVECTOR3			m_rotDest;		// 目的の向き
	D3DXVECTOR3			m_move;		// 移動量
	int					m_idxShadow;
	bool				m_bUse;

	CIRCLE				m_circle;		// 当たり判定用サークル
	AABB				m_aabb;

public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	bool IsEnable(int index);
	CIRCLE* GetCircleCollision(int index);
	AABB * GetAABBCollision(int index);
	Enemy* GetEnemy();
};
