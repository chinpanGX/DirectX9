/*=============================================================================

	[player.h]
	Author : 出合翔太

=============================================================================*/
#pragma once

#include "main.h"
#include "shadow.h"
#include "bullet.h"

//	Playerクラス
class Player
{
private:
	static LPDIRECT3DTEXTURE9	m_pTextureModel;	//	テクスチャへのポインタ
	static LPD3DXMESH			m_pMeshModel;		//	メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMatModel;	//	マテリアル情報へのポインタ
	static DWORD				m_nNumMatModel;		//	マテリアル情報の総数
	
	LPDIRECT3DDEVICE9			m_pDevice;			//	デバイス
	D3DXMATRIX					m_mtxWorldModel;		//	ワールドマトリックス
	D3DXVECTOR3					m_pos;				//	現在の位置
	D3DXVECTOR3					m_rot;				//	現在の向き
	D3DXVECTOR3					m_rotDest;			//	目的の向き
	int							m_dir;				//	向いている方向
	D3DXVECTOR3					m_move;				//	移動量
	int							m_IdxShadow;		//	影をつくるインデックスの配列番号
	bool						m_bEnableGravity;

	Bullet	m_bullet;
	Shadow	m_shadow;

public:
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();
};