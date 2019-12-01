/*==============================================

	[field.h]
	Author : 出合翔太

===============================================*/

#pragma once
#include "main.h"

//	Fieldクラス
class Field
{
private:
	static	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffer;	//	頂点バッファへのポインタ
	static	LPDIRECT3DINDEXBUFFER9	m_pIdxBuffer;	//	頂点バッファへのポインタ
	static	LPDIRECT3DDEVICE9		m_pDevice;		//	デバイス
	D3DXMATRIX						m_mtxWorld;		//	ワールドマトリックス
	static	D3DXVECTOR3				m_posField;		//	地面の位置
	static	D3DXVECTOR3				m_rotField;		//	地面の向き(回転)
	static	D3DXVECTOR3				m_sclField;		//	地面の大きさ(スケール)
public:
	HRESULT	Init();											//	初期化処理
	void	Uninit();										//	終了処理
	void	Draw();											//	描画処理
private:
	HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);		//	頂点作成
};
