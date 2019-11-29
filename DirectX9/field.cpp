#include "field.h"

#include "input.h"
#include "texture.h"
#include "debugproc.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define VALUE_MOVE  (5.0f)       // 移動量
#define VALUE_ROTATE (D3DX_PI * 0.02f)    // 回転量

#define FIELD_WIDTH  (100.0f)      // 地面の幅(X方向)
#define FIELD_HEIGHT (100.0f)      // 地面の幅(X方向)
#define FIELD_DEPTH  (100.0f)      // 地面の奥行(Z方向)



#define SQUARE_LENGTH (100)
#define SQUARE_NUM  (SQUARE_LENGTH + 1)
#define TYOTEN (pow(SQUARE_NUM,2)*2)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL; // 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffField = NULL; // 頂点バッファへのポインタ
int      g_NumIndexField = 18;

D3DXMATRIX    g_mtxWorldField;  // ワールドマトリックス
D3DXVECTOR3    g_posField;    // 地面の位置
D3DXVECTOR3    g_posField1;   // 地面の位置
D3DXVECTOR3    g_rotField;    // 地面の向き(回転)
D3DXVECTOR3    g_rotField1;   // 地面の向き(回転)
D3DXVECTOR3    g_sclField;    // 地面の大きさ(スケール)

static float g_NumVertex;
static float g_NumIndex;
static float g_NumPolygon;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Field_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexField(pDevice);
	//MakeVertexField1(pDevice1);
	// 位置・回転・スケールの初期設定
	g_posField = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//MoveBox = 1.0f;
	//Select = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Field_Finalize(void)
{
	if (g_pVtxBuffField != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}

	if (g_pIdxBuffField != NULL)
	{// 頂点バッファの開放
		g_pIdxBuffField->Release();
		g_pIdxBuffField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Field_Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void Field_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldField);

	//スケールを反映
	D3DXMatrixScaling(&mtxScl, g_sclField.x, g_sclField.y, g_sclField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);

	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.x, g_rotField.y, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	//
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(g_pIdxBuffField);
	//インデックスバッファのセット
	pDevice->SetFVF(FVF_VERTEX3D);

	//テクスチャの設定
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, TYOTEN, 0, TYOTEN - 4);


	DebugProc_Print((char*)"*** ３Ｄポリゴン操作 ***\n");
	DebugProc_Print((char*)"位置 [%f : %f : %f]\n", g_posField.x, g_posField.y, g_posField.z);
	DebugProc_Print((char*)"前移動 : Ｗ\n");
	DebugProc_Print((char*)"後移動 : Ｓ\n");
	DebugProc_Print((char*)"左移動 : Ａ\n");
	DebugProc_Print((char*)"右移動 : Ｄ\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"向き [%f : %f : %f]\n", g_rotField.x, g_rotField.y, g_rotField.z);
	DebugProc_Print((char*)"X軸回転 : ↑ / ↓\n");
	DebugProc_Print((char*)"Y軸回転 : Ｑ / Ｅ\n");
	DebugProc_Print((char*)"Z軸回転 : ← / →\n");
	DebugProc_Print((char*)"\n");

	DebugProc_Print((char*)"位置・向きリセット : ENTER\n");
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * TYOTEN / 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL)))
	{
		return E_FAIL;
	}


	{
		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		//VRAM上の情報を持ってくるから、頂点をいじれるようになる
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for (int z = 0; z < SQUARE_NUM; z++) {
			for (int x = 0; x < SQUARE_NUM; x++) {

				pVtx[z * SQUARE_NUM + x].pos = D3DXVECTOR3(x * 10.0f - 500.0f, 0.0f, z * -10.0f + 400.0f);

				pVtx[z * SQUARE_NUM + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				pVtx[z * SQUARE_NUM + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				pVtx[z * SQUARE_NUM + x].tex.x = (float)x;
				pVtx[z * SQUARE_NUM + x].tex.y = (float)z;
			}
		}


		//ここでVRAMへ帰す
		g_pVtxBuffField->Unlock();
	}

	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * TYOTEN,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffField,
		NULL)))
	{
		return E_FAIL;
	}
	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		g_pIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		//for文をわかりやすくするための変数
		static int cnt1 = SQUARE_NUM;
		static int cnt2 = 0;

		//縮退ポリゴンのカウント
		static int cnt3 = 1;

		for (int i = 0; i < TYOTEN; i++)
		{
			//縮退ポリゴンを書く時
			if (i == ((SQUARE_LENGTH + 1) * 2 + 2) * cnt3 - 2 && i != 0)
			{
				pIdx[i] = pIdx[i - 1];  //前の頂点に被せる
				pIdx[i + 1] = cnt1;   //先の頂点
				pIdx[i + 2] = cnt1;   //前の頂点に被せる
				i += 2;      //二回点余分に代入したので+2
				cnt3++;      //縮退ポリゴンの回数を増やす
			}
			//縮退ポリゴンを書かないとき(iが偶数か見る)
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

		/*pIdx[0] = 6;
		pIdx[1] = 0;
		pIdx[2] = 7;
		pIdx[3] = 1;
		pIdx[4] = 8;
		pIdx[5] = 2;
		pIdx[6] = 9;
		pIdx[7] = 3;
		pIdx[8] = 10;
		pIdx[9] = 4;
		pIdx[10] = 11;
		pIdx[11] = 5;
		pIdx[12] = 5;
		pIdx[13] = 12;
		pIdx[14] = 12;
		pIdx[15] = 6;
		pIdx[16] = 13;
		pIdx[17] = 7;
		pIdx[18] = 14;
		pIdx[19] = 8;
		pIdx[20] = 15;
		pIdx[21] = 9;
		pIdx[22] = 16;
		pIdx[23] = 10;
		pIdx[24] = 18;
		pIdx[25] = 11;*/

		//ここでVRAMへ帰す
		g_pVtxBuffField->Unlock();
	}
	return S_OK;
}