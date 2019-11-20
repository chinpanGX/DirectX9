/*==========================================================

	[Field.cpp]
	Author : 出合翔太

===========================================================*/

#include "main.h"
#include "Field.h"
#include "texture.h"
#include <time.h>

#define VERTEX_X	100
#define VERTEX_Z	100
//	グローバル変数
LPDIRECT3DVERTEXBUFFER9 Field::g_pVtxBuffField = NULL;	//	頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	Field::g_pIdxBuffField = NULL;			//	インデックスバッファ
D3DXMATRIX				Field::g_mtxWorldField;			//	ワールドマトリックス
D3DXVECTOR3				Field::g_posField;				//	地面の位置
D3DXVECTOR3				Field::g_rotField;				//	地面の向き(回転)
D3DXVECTOR3				Field::g_sclField;				//	地面の大きさ	
LPDIRECT3DDEVICE9		Field::pDevice;					//  デバイス取得用変数

int g_NumVertexField;

//	フィールドの初期化
HRESULT Field::Init()
{
	pDevice = GetD3DDevice();
	// 頂点情報の作成
	MakeVertexField(pDevice);

	// 位置・回転・スケールの初期設定
	g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_NumVertexField = (VERTEX_X + 1) * (VERTEX_Z + 1);

	return S_OK;
}

//	フィールドの終了処理
void Field::Uninit()
{
	DEVICE_RELEASE(g_pVtxBuffField);
	DEVICE_RELEASE(g_pIdxBuffField);
}

//	フィールドの描画
void Field::Draw()
{
	pDevice = GetD3DDevice();

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	
	//	ワールド変換（ワールドマトリクスの初期化）
	D3DXMatrixIdentity(&g_mtxWorldField);
	D3DXMatrixScaling(&mtxScl, g_sclField.x, g_sclField.y, g_sclField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);					//　引数は2番目と3番目の計算結果を1番目に入れる
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.x, g_rotField.y, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	D3DXMatrixTranslation(&mtxTranslate, g_posField.y, g_posField.x, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	
	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	//	頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	//インデックスバッファのセット
	pDevice->SetIndices(g_pIdxBuffField);

	//	頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD03));

	//	自分で作った頂点バッファを描画（メモリの確保、解放をしなければならない→遅くなる）
	//pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, POLYGON_NUM);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVertexField, 0, 12);

}


//	頂点作成関数
HRESULT Field::MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	//オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_X * VERTEX_Z, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffField, NULL)))
	{
		return E_FAIL;
	}

	{
		VERTEX_3D *pVtx;

		//頂点データの範囲をLockする
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		for (int z = 0; z < VERTEX_Z; z++)
		{
			for (int x = 0; x < VERTEX_X; x++)
			{
				pVtx[z * 4 + x].pos = D3DXVECTOR3(x * 100.0f, 0.0f, z * -100.0f);
				pVtx[z * 4 + x].tex = D3DXVECTOR2(x, z);
			}
		}

		for (int i = 0; i < 10000; i++)
		{
			pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		
		//頂点データをUnlockする
		g_pVtxBuffField->Unlock();

		if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * VERTEX_X * VERTEX_Z, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffField, NULL)))
		{
			return E_FAIL;
		}
	}

	//インデックスバッファの中身を埋める
	{
		WORD *pIdx;

		g_pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		for (int z = 0; VERTEX_Z * 2; z++)
		{

		}

		//{
		//	pIdx[0] = 4;
		//	pIdx[1] = 0;
		//	pIdx[2] = 5;
		//	pIdx[3] = 1;
		//	pIdx[4] = 6;
		//	pIdx[5] = 2;
		//	pIdx[6] = 7;
		//	pIdx[7] = 3;
		//}


		g_pIdxBuffField->Unlock();
	}


	return S_OK;
}

