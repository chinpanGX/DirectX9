/*===============================================

	[Field.cpp]
	Author : 出合翔太

===============================================*/

#include "main.h"
#include "field.h"
#include "texture.h"
#include "debugproc.h"

//	マクロ定義
#define SQUARE_LENGTH (100)					//	描画する地面の数
#define SQUARE_NUM  (SQUARE_LENGTH + 1)		//	縮退ポリゴンを含めた地面の数
#define VERTEX (pow(SQUARE_NUM,2)*2)		//	頂点の数

//	スタティック変数
PDIRECT3DVERTEXBUFFER9		Field::m_pVtxBuffer;	//	頂点バッファへのポインタ
PDIRECT3DINDEXBUFFER9		Field::m_pIdxBuffer;	//	頂点バッファへのポインタ
PDIRECT3DDEVICE9			Field::m_pDevice;		//	デバイス
D3DXVECTOR3					Field::m_posField;		//	地面の位置
D3DXVECTOR3					Field::m_rotField;		//	地面の向き(回転)
D3DXVECTOR3					Field::m_sclField;		//	地面の大きさ(スケール)

//	初期化処理
HRESULT Field::Init()
{
	m_pDevice = GetD3DDevice();
	
	// 位置・回転・スケールの初期設定
	m_posField = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	m_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	// 頂点情報の作成
	MakeVertexField(m_pDevice);

	return S_OK;
}

//	終了処理
void Field::Uninit()
{
	SAFE_RELEASE(m_pVtxBuffer);
	SAFE_RELEASE(m_pIdxBuffer);
}

//	描画処理
void Field::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	//	レンダリングステートの設定　
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//フィルモード,ワイヤーフレームの設定
	m_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	//	ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//	スケールを反映
	D3DXMatrixScaling(&mtxScl, m_sclField.x, m_sclField.y, m_sclField.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//	回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotField.x, m_rotField.y, m_rotField.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//	移動を反映
	D3DXMatrixTranslation(&mtxTranslate, m_posField.x, m_posField.y, m_posField.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	//	ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//	頂点バッファをデバイスのデータストリームにバインド
	//	引数（頂点バッファを設定するストリームID、第2引数:頂点バッファへのポインタ、ストリームの先頭から頂点データの先頭までのオフセット、頂点データの一つ分のデータサイズをバイト単位で指定）
	m_pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//	インデックスバッファのセット
	m_pDevice->SetIndices(m_pIdxBuffer);
	
	m_pDevice->SetFVF(FVF_VERTEX3D);

	//	テクスチャの設定
	m_pDevice->SetTexture(0, Texture::GetTexture(TEXTURE_INDEX_FIELD01));

	//	ポリゴンの描画
	//	引数（プリミティブタイプ、インデックスバッファの開始地点から最初のインデックスまでのオフセット、最小頂点番号、インデックスの数、配列の読み取り開始位置、三角ポリゴンの数）
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, VERTEX, 0, VERTEX - 4);

	//	文字列の描画
	DebugProc::Print((char*)"*** ３Ｄポリゴン操作 ***\n");
	DebugProc::Print((char*)"位置 [%f : %f : %f]\n", m_posField.x, m_posField.y, m_posField.z);
	DebugProc::Print((char*)"前移動 : Ｗ\n");
	DebugProc::Print((char*)"後移動 : Ｓ\n");
	DebugProc::Print((char*)"左移動 : Ａ\n");
	DebugProc::Print((char*)"右移動 : Ｄ\n");
	DebugProc::Print((char*)"\n");
	DebugProc::Print((char*)"向き [%f : %f : %f]\n", m_rotField.x, m_rotField.y, m_rotField.z);
	DebugProc::Print((char*)"X軸回転 : ↑ / ↓\n");
	DebugProc::Print((char*)"Y軸回転 : Ｑ / Ｅ\n");
	DebugProc::Print((char*)"Z軸回転 : ← / →\n");
	DebugProc::Print((char*)"\n");
	DebugProc::Print((char*)"位置・向きリセット : ENTER\n");
}

//	頂点作成
HRESULT Field::MakeVertexField(LPDIRECT3DDEVICE9 m_pDevice)
{
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX / 2,	//	頂点バッファサイズ
		D3DUSAGE_WRITEONLY,														//	リリース制御を表すフラグ
		FVF_VERTEX3D,															//	頂点フォーマットの設定
		D3DPOOL_MANAGED,														//	使用するメモリ（D3DPOLL列挙型）
		&m_pVtxBuffer,															//	生成した頂点バッファを受け取るポインタのアドレス
		NULL)))
	{
		return E_FAIL;
		//	メゾット失敗のエラー値	定数と解説
		//	D3DERR_INVALIDCALL		->	メソッドの呼び出しが無効である
		//	D3DERR_OUTOFVIDEOMEMORY	->	十分なディスプレイメモリが存在しない
		//	E_OUTOFMEMORY			->	十分なメモリを割り当てることができない
	}

	{
		//	頂点バッファの中身を埋める
		VERTEX_3D *pVtx;	//	頂点データへのポインタアドレス

		//	Lock関数
		//	引数（ロックする頂点データのオフセットを指定、ロックする範囲をバイト単位で指定、頂点データへのポインタのアドレスをキャスト(void**)してを指定、ロックの方法を表すフラグ）
		m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);		//	頂点データの指定範囲をロックし、バッファへのポインタを取得します

		//	頂点の設定
		for (int z = 0; z < SQUARE_NUM; z++)
		{
			for (int x = 0; x < SQUARE_NUM; x++) 
			{
				pVtx[z * SQUARE_NUM + x].pos = D3DXVECTOR3(x * 10.0f - 500.0f,0.0f, z * -10.0f + 100.0f);	//	位置
				pVtx[z * SQUARE_NUM + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								//	法線ベクトル
				pVtx[z * SQUARE_NUM + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);							//	頂点カラー
				pVtx[z * SQUARE_NUM + x].tex.x = (float)x;													//	テクスチャのx座標
				pVtx[z * SQUARE_NUM + x].tex.y = (float)z;													//	テクスチャのy座標
			}
		}

		//	Unlock関数	
		m_pVtxBuffer->Unlock();	//	データをVRAMへ返す
	}

	if (FAILED(m_pDevice->CreateIndexBuffer(sizeof(WORD) * VERTEX,	//	配列にWORD型なのは,指定フォーマットが2種類しかないから。（WORD型は16ビット）
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,												//	指定できるのはD3DFMT_INDEX16(16ビット)とD3DFMT_INDEX32(32ビット)のみ
		D3DPOOL_MANAGED,
		&m_pIdxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	{
		//	インデックスバッファの中身を埋める
		WORD *pIdx;

		m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

		//	for文をわかりやすくするための変数
		int cnt1 = SQUARE_NUM;
		int cnt2 = 0;
		int DegenerateCnt = 1;	//	縮退ポリゴンのカウント

		for (int i = 0; i < VERTEX; i++)
		{
			//	縮退ポリゴンを書く時
			if (i == ((SQUARE_LENGTH + 1) * 2 + 2) * DegenerateCnt - 2 && i != 0)
			{
				pIdx[i] = pIdx[i - 1];  //	前の頂点に被せる
				pIdx[i + 1] = cnt1;		//	先の頂点
				pIdx[i + 2] = cnt1;		//	前の頂点に被せる
				i += 2;					//	二回点余分に代入したので+2
				DegenerateCnt++;		//	縮退ポリゴンの回数を増やす
			}
			//	縮退ポリゴンを書かないとき(iが偶数か見る)
			if (i % 2 == 0)
			{
				pIdx[i] = cnt1;
				cnt1++;
			}
			else
			{
				pIdx[i] = cnt2;
				cnt2++;
			}
		}

		m_pVtxBuffer->Unlock();
	}
	return S_OK;
}

