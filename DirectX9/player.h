/*=============================================================================

	[player.h]
	Author : 出合翔太

=============================================================================*/
#pragma once

#include "main.h"
#include "shadow.h"

//	Playerクラス
class Player
{
private:
	static LPDIRECT3DTEXTURE9	m_pTextureModel;	//	テクスチャへのポインタ
	static LPD3DXMESH			m_pMeshModel;		//	メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMatModel;	//	マテリアル情報へのポインタ
	static DWORD				m_nNumMatModel;		//	マテリアル情報の総数
	LPDIRECT3DDEVICE9			m_pDevice;			//	デバイス
	D3DXMATRIX					mtxWorldModel;		//	ワールドマトリックス
	D3DXVECTOR3					posModel;			//	現在の位置
	D3DXVECTOR3					rotModel;			//	現在の向き
	D3DXVECTOR3					rotDestModel;		//	目的の向き
	D3DXVECTOR3					moveModel;			//	移動量
	int							m_IdxShadow;		//	影をつくるインデックスの配列番号
	Shadow						m_shadow;
public:
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();
	D3DXVECTOR3	GetPos();
};