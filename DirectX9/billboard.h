//=============================================================================
//
// ビルボード処理 [billboard.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "texture.h"
#include "shadow.h"

class BillBoard
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBillboard;		// 頂点バッファへのポインタ
	static D3DXMATRIX				m_mtxWorldBillboard;		// ワールドマトリックス
	static D3DXVECTOR3				m_posBillboard;				// 位置
	static D3DXVECTOR3				m_sclBillboard;				// スケール
	static D3DXVECTOR3				m_moveBillboard;			// 移動量
	static int						m_nIdxShadowBillboard;		// 影ID
	static bool						m_bEnableGravity;			// 重力を有効にするかどうか
	static LPDIRECT3DDEVICE9		m_pDevice;
	Texture							m_texture;
	int								m_IdxShadow;				//	影の配列番号を格納
public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
private:
	HRESULT MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);
	void SetVertexBillboard(float fSizeX, float fSizeY);
};
