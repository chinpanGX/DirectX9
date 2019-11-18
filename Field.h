/*===========================================================

	[Field.h]
	Author : 出合翔太

=============================================================*/

#pragma once

//# Fieldクラス
/* ゲームの背景の描画をするクラス */
class Field
{
private:
	static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffField;	//	頂点バッファへのポインタ
	static LPDIRECT3DINDEXBUFFER9	g_pIdxBuffField;	//	インデックスバッファ
	static D3DXMATRIX				g_mtxWorldField;	//	ワールドマトリックス
	static LPDIRECT3DDEVICE9		pDevice;			//	デバイス取得用変数
	//	3次元ベクトル
	static D3DXVECTOR3				g_posField;			//	地面の位置
	static D3DXVECTOR3				g_rotField;			//	地面の向き(回転)
	static D3DXVECTOR3				g_sclField;			//	地面の大きさ(スケール)
public:
	HRESULT Init();										//	初期化処理
	void Uninit();										//	終了処理
	void Draw();										//	描画
	HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);	//	頂点作成
};

