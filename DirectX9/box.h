
#pragma once
#include "main.h"
#include "texture.h"

class Box
{
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBox;		//	頂点バッファへのポインタ
	static LPDIRECT3DINDEXBUFFER9	m_pIdxBuffBox;		//	インデックスバッファへのポインタ
	static int						m_NumVertexBox;		//	頂点の数
	static D3DXVECTOR3				m_posBox;			//	地面の位置
	static D3DXVECTOR3				m_rot1Box;			//	地面の向き(回転)
	static D3DXVECTOR3				m_rot2Box;			//	地面の向き(回転)
	static D3DXVECTOR3				m_sclBox;			//	地面の大きさ(スケール)
	static LPDIRECT3DDEVICE9		m_pDevice;
	D3DXMATRIX						m_mtxWorldBox;		//	ワールドマトリックス
	Texture							m_texture;
public:
	HRESULT Init();
	void	Uninit();
	void	Update();
	void	Draw();
private:
	HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice);
	void DrawPolygon(int i);
};