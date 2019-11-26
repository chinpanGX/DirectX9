/*==========================================================

	[Field.cpp]
	Author : 出合翔太

===========================================================*/

#include "main.h"
#include "Field.h"
#include "texture.h"
#include <time.h>

#define BLOCK_X	4	//
#define BLOCK_Z	2
//	スタティック変数

//!	頂点バッファ…頂点情報を格納したメモリ領域 -> VRAM（グラフィックボードのメモリ）につくられる
LPDIRECT3DVERTEXBUFFER9 Field::g_pVtxBuffField = NULL;	//	頂点バッファへのポインタ

//!	インデックスバッファ…ポリゴンを作成するための頂点番号リスト　->　頂点情報とポリゴンを作成する頂点を分離するために使う
//!	モデルファイルなどのポリゴンメッシュを作成するときに必要
LPDIRECT3DINDEXBUFFER9	Field::g_pIdxBuffField = NULL;	//	インデックスバッファ

D3DXMATRIX				Field::g_mtxWorldField;			//	ワールドマトリックス
D3DXVECTOR3				Field::g_posField;				//	地面の位置
D3DXVECTOR3				Field::g_rotField;				//	地面の向き(回転)
D3DXVECTOR3				Field::g_sclField;				//	地面の大きさ	
LPDIRECT3DDEVICE9		Field::pDevice;					//  デバイス取得用変数

int g_NumVertex;	//	頂点の数			
int g_NumIndex;		//	インデックスの数
int g_NumPolygon;	//	ポリゴンの数

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

	g_NumVertex	 = (BLOCK_X + 1) * (BLOCK_Z + 1);	//	X方向の頂点数　＊　Z方向の頂点数
	g_NumIndex	 = ((BLOCK_X + 1) * 2) * BLOCK_Z + (BLOCK_Z - 1) * 2;		//	(BLOCK_X + 1)を2倍し、Z方向分必要 + 縮退ポリゴン分
	g_NumPolygon = (BLOCK_X * 2) * BLOCK_Z + (BLOCK_Z - 1) * 4;

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
	//	SetStreamSource	(第１引数:頂点バッファを設定するストリーム番号を指定、第２引数:頂点バッファへのポインタを指定、第３引数:頂点データの一つ分のデータサイズをバイト単位で指定)
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	//インデックスバッファのセット
	pDevice->SetIndices(g_pIdxBuffField);

	//	頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD03));

	//----------------------------------------------------------------------------------------------
	//	DrawPrimitiveUpとDrawRimitiveの違い
	//"	DrawPrimitiveUpの説明	->	板ポリなど頂点が少ないオブジェクトで使う
	//!	１．DrawPrimitiveUpが頂点情報を通常のメモリに置く -> 処理が遅くなる
	//!	２．LockとUnlockを使わなくていい	

	//"	DrawPrimitiveの説明		->	頂点が多いモデルファイルなどを描画するときに使う
	//!	１．頂点情報がVRAM上に置かれる　-> 処理速度が速い
	//!	２．LockとUnlockが必要になる	-> パフォーマンスが低下 ->なるべく少なくしたい
	//----------------------------------------------------------------------------------------------

	//	自分で作った頂点バッファを描画（メモリの確保、解放をしなければならない　-> 多用すると遅くなる）
	//DrawPrimitive (第1引数：プリミティブを指定、第2引数：描画を開始する頂点を指定、第3引数：使用するプリミティブの数を指定)
	//pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, POLYGON_NUM);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVertex, 0, 12);

}


//	頂点作成関数
HRESULT Field::MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	//オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer
		(sizeof(VERTEX_3D) * BLOCK_X * BLOCK_Z,		//Length		 -	頂点バッファのサイズをバイト単位で指定
		D3DUSAGE_WRITEONLY, 						//Usage			 -	必要の応じてリソースの制御方法を表すフラグを指定
		FVF_VERTEX_3D, 								//FVF			 -	用いられる頂点フォーマットのフラグの組み合わせを指定
		D3DPOOL_MANAGED, 							//Pool			 -	D3DPOOL 列挙型のメンバで、どのメモリを使用するかを指定
		&g_pVtxBuffField, 							//ppVertexBuffer -	作成した頂点バッファを受けるポインタのポインタを指定
		NULL)))
	{
		return E_FAIL;
		//	メゾット失敗のエラー値	定数と解説
		//	D3DERR_INVALIDCALL		->	メソッドの呼び出しが無効である
		//	D3DERR_OUTOFVIDEOMEMORY	->	十分なディスプレイメモリが存在しない
		//	E_OUTOFMEMORY			->	十分なメモリを割り当てることができない
	}

	{
		VERTEX_3D *pVtx;

		//頂点データの指定範囲をロックし、バッファへのポインタを取得します
		//	OffsetToLock - ロックする頂点データのオフセットをバイト単位で指定します
		//	SizeToLock - ロックする範囲をバイト単位で指定します
		//	ppbData - 取得する頂点データへのポインタのポインタを指定します
		//	Flags - ロックの方法を表すフラグを指定します
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		for (int z = 0; z < BLOCK_Z; z++)
		{
			for (int x = 0; x < BLOCK_X; x++)
			{
				pVtx[z * 4 + x].pos = D3DXVECTOR3(x * 100.0f, 0.0f, z * -100.0f);
				pVtx[z * 4 + x].tex = D3DXVECTOR2(x, z);
			}
		}

		for (int i = 0; i < BLOCK_X + 1 * BLOCK_Z + 1; i++)
		{
			pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		
		//頂点データをUnlockする
		g_pVtxBuffField->Unlock();

		if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * BLOCK_X * BLOCK_Z, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffField, NULL)))
		{
			return E_FAIL;
		}
	}
	//-----------------------------------------------------------------------------
	//	インデックスバッファのメリット
	//!	１．頂点バッファの削減	->	頂点バッファを使うと頂点の重複が発生する
	//"			問題点	１．メモリの無駄遣い。
	//"					２．頂点の変更がしにくい
	//!	２．頂点計算の数が減り、処理速度の向上	
	//------------------------------------------------------------------------------
	//インデックスバッファの中身を埋める
	{
		WORD *pIdx;

		g_pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		for (int z = 0; BLOCK_Z * 2; z++)
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

