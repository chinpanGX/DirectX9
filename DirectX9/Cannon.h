//=============================================================================
//
// ビルボード処理 [Cannon.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "texture.h"
#include "shadow.h"

class Cannon
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffCannon;		// 頂点バッファへのポインタ
	static D3DXMATRIX				m_mtxWorldCannon;		// ワールドマトリックス
	static D3DXVECTOR3				m_posCannon;				// 位置
	static D3DXVECTOR3				m_sclCannon;				// スケール
	static D3DXVECTOR3				m_moveCannon;			// 移動量
	static int						m_nIdxShadowCannon;		// 影ID
	static bool						m_bEnableGravity;			// 重力を有効にするかどうか
	static LPDIRECT3DDEVICE9		m_pDevice;
	Texture							m_texture;
	Shadow							m_shadow;
	int								m_IdxShadow;				//	影の配列番号を格納
	bool							m_buse;
	int								m_frame;
public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Create(float x, float y, D3DXVECTOR2 dir);
private:
	HRESULT MakeVertexCannon(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexCannon(float fSizeX, float fSizeY);
};
