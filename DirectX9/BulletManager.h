/*=======================================================

	[BulletManager.h]
	Author : 出合翔太

========================================================*/

#pragma once
#include "main.h"

#include "texture.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"

//	マクロ定義
#define	SIZE_X		(20.0f)	// ビルボードの幅
#define	SIZE_Y		(20.0f)	// ビルボードの高さ
#define MAX_BULLET	(512)	//	最大数


class Bullet
{
public:
	D3DXVECTOR3	m_position;			//	位置
	D3DXVECTOR3	m_scale;			//	スケール
	D3DXVECTOR3	m_velocity;			//	移動量
	bool		m_bEnableGravity;	//	重力を有効にするかどうか
	float		m_fSizeX;			//	幅
	float		m_fSizeY;			//	高さ
	int			m_life;				//	寿命
	bool		m_bUse;				//	使用しているかどうか
	Shadow		m_shadow;
};

class BulletManager
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBullet;		//	頂点バッファへのポインタ
	LPDIRECT3DDEVICE9				m_pDevice;
	D3DXMATRIX						m_mtxWorld;				//	ワールドマトリックス
	D3DXVECTOR3						m_posBase;				//	ビルボード発生位置
	float							m_fWidthBase = 5.0f;	//	基準の幅
	float							m_fHeightBase = 10.0f;	//	基準の高さ
	float							m_roty = 0.0f;			//	移動方向
	float							m_spd = 0.0f;			//	移動スピード
	int								m_IdxShadow;			//	影ID
	Bullet							m_aBullet[MAX_BULLET];	//	BulletWork
	Texture							m_texture;
	Camera							*m_camera;
	Shadow							m_shadow;
	Player							m_player;
public:
	HRESULT Init();
	void	Uninit();
	void	Update();
	void	Draw();
private:
	HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexBullet(int nIdx, float fSizeX, float fSizeY);
	int SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nLife);
};

