
#include "Box.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// 回転量

#define	BOX_WIDTH		(100.0f)						// 地面の幅(X方向)
#define	BOX_DEPTH		(100.0f)						// 地面の奥行(Z方向)
#define BOX_HEIGHT      (100.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBox = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffBox = NULL;
static int             g_NumIndexBox = 36;

#define NUM_BOX 2

typedef struct {
	D3DXMATRIX				g_mtxWorldBox;		// ワールドマトリックス
	D3DXVECTOR3				g_posBox;				// 地面の位置
	D3DXVECTOR3				g_rot1Box;				// 地面の向き(回転)
	D3DXVECTOR3				g_rot2Box;				// 地面の向き(回転)
	D3DXVECTOR3				g_sclBox;				// 地面の大きさ(スケール)
}BOX_T;

static BOX_T g_Box[NUM_BOX];

//blockBox g_blockBox[2];


//float g_move_x = 5.0f;
//float g_scl_x = 0.01f;
//float g_rot1_x = 0.01f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Box_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexBox(pDevice);

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < NUM_BOX; i++)
	{
		g_Box[i].g_posBox = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Box[i].g_rot1Box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Box[i].g_sclBox = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Box_Finalize(void)
{
	if (g_pIdxBuffBox != NULL)
	{
		g_pIdxBuffBox->Release();
		g_pIdxBuffBox = NULL;
	}

	if(g_pVtxBuffBox != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBox->Release();
		g_pVtxBuffBox = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Box_Update(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	for (int i = 0; i < NUM_BOX; i++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Box[i].g_mtxWorldBox);
	}

	g_Box[0].g_rot1Box.y += 0.01f;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[0].g_rot1Box.y, g_Box[0].g_rot1Box.x, g_Box[0].g_rot1Box.z);
	D3DXMatrixMultiply(&g_Box[0].g_mtxWorldBox, &g_Box[0].g_mtxWorldBox, &mtxRot);


	g_Box[0].g_posBox.x = 100.0f;
	g_Box[0].g_posBox.z = 100.0f;

	D3DXMatrixTranslation(&mtxTranslate, g_Box[0].g_posBox.x, g_Box[0].g_posBox.y, g_Box[0].g_posBox.z);
	D3DXMatrixMultiply(&g_Box[0].g_mtxWorldBox, &g_Box[0].g_mtxWorldBox, &mtxTranslate);


	g_Box[1].g_rot1Box.y -= 0.05f;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot1Box.y, g_Box[1].g_rot1Box.x, g_Box[1].g_rot1Box.z);
	D3DXMatrixMultiply(&g_Box[1].g_mtxWorldBox, &g_Box[1].g_mtxWorldBox, &mtxRot);

	g_Box[1].g_posBox.x = 120.0f;
	g_Box[1].g_posBox.z = 100.0f;

	D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posBox.x, g_Box[1].g_posBox.y, g_Box[1].g_posBox.z);
	D3DXMatrixMultiply(&g_Box[1].g_mtxWorldBox, &g_Box[1].g_mtxWorldBox, &mtxTranslate);

	g_Box[1].g_rot2Box.y += 0.05f;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot2Box.y, g_Box[1].g_rot2Box.x, g_Box[1].g_rot2Box.z);
	D3DXMatrixMultiply(&g_Box[1].g_mtxWorldBox, &g_Box[1].g_mtxWorldBox, &mtxRot);

	D3DXVECTOR3 pos;
	pos.x = 100.0f;
	pos.y = 100.0f;
	pos.z = 100.0f;

	D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posBox.x, g_Box[1].g_posBox.y, g_Box[1].g_posBox.z);
	D3DXMatrixMultiply(&g_Box[1].g_mtxWorldBox, &g_Box[1].g_mtxWorldBox, &mtxTranslate);

	////四角の拡大縮小いじり
	//{
	//	/*g_sclBox.x += g_scl_x;
	//	if (g_sclBox.x > 3.0f || g_sclBox.x < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}

	//	g_sclBox.z += g_scl_x;
	//	if (g_sclBox.z > 3.0f || g_sclBox.z < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}

	//	g_sclBox.y += g_scl_x;
	//	if (g_sclBox.y > 3.0f || g_sclBox.y < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}
	//	*/
	//}
	////スケールを反映
	//{
	//	D3DXMatrixScaling(&mtxScl, g_sclBox.x, g_sclBox.y, g_sclBox.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxScl);
	//}	
	////四角の角度いじり
	//{
	//	//g_rotBox.z = D3DXToRadian(45);
	//}
	////四角の回転
	//{
	//	g_rot1Box.y -= 0.03f;
	//	g_rot2Box.y += 0.03f;
	//}
	////中心を変更して回転
	//{
	//	/*
	//	回転反映→移動判定を移動判定→回転判定に変える
	//	*/
	//}
	////四角の移動
	///*{
	//	g_posBox.x += g_move_x;
	//	if (g_posBox.x >220.0f|| g_posBox.x < -220.0f)
	//	{
	//	g_move_x*=-1 ;
	//	}

	//}
	//*/
	////固定位置
	//{

	//	g_posBox.x = 300;
	//	g_posBox.y = 100;
	//	g_posBox.z = 300;

	//}
	////回転を反映
	///*
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_blockBox[0].g_rot1Box.y, g_blockBox[0].g_rot1Box.x, g_rot1Box.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxRot);
	//}
	//
	////移動を反映
	//
	//{
	//	D3DXMatrixTranslation(&mtxTranslate, g_posBox.x, g_posBox.y, g_posBox.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxTranslate);
	//}
	//
	////回転を反映(普通の回転)
	//
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot2Box.y, g_rot2Box.x, g_rot2Box.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxRot);
	//}
	//
	////回転を反映
	//
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_blockBox[0].g_rot1Box.y, g_rot1Box.x, g_rot1Box.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxRot);
	//}*/
	///*
	////移動を反映
	//{
	//	D3DXMatrixTranslation(&mtxTranslate, g_posBox.x, g_posBox.y, g_posBox.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxTranslate);
	//}
	//*/
	///*
	////回転を反映
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot2Box.y, g_rot2Box.x, g_rot2Box.z);
	//	D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxRot);
	//}
//	if (Keyboard_IsPress(DIK_A))
//	{
//		if (Keyboard_IsPress(DIK_W))
//		{// 左奥移動
//			g_posBox.x += sinf(-D3DX_PI * 0.75f) * VALUE_MOVE;
//			g_posBox.z -= cosf(-D3DX_PI * 0.75f) * VALUE_MOVE;
//		}
//		else if (Keyboard_IsPress(DIK_S))
//		{// 左手前移動
//			g_posBox.x += sinf(-D3DX_PI * 0.25f) * VALUE_MOVE;
//			g_posBox.z -= cosf(-D3DX_PI * 0.25f) * VALUE_MOVE;
//		}
//		else
//		{// 左移動
//			g_posBox.x += sinf(-D3DX_PI * 0.50f) * VALUE_MOVE;
//			g_posBox.z -= cosf(-D3DX_PI * 0.50f) * VALUE_MOVE;
//		}
//	}
//	else if (Keyboard_IsPress(DIK_D))
//	{
//		if (Keyboard_IsPress(DIK_W))
//		{// 右奥移動
//			g_posBox.x += sinf(D3DX_PI * 0.75f) * VALUE_MOVE;
//			g_posBox.z -= cosf(D3DX_PI * 0.75f) * VALUE_MOVE;
//		}
//		else if (Keyboard_IsPress(DIK_S))
//		{// 右手前移動
//			g_posBox.x += sinf(D3DX_PI * 0.25f) * VALUE_MOVE;
//			g_posBox.z -= cosf(D3DX_PI * 0.25f) * VALUE_MOVE;
//		}
//		else
//		{// 右移動
//			g_posBox.x += sinf(D3DX_PI * 0.50f) * VALUE_MOVE;
//			g_posBox.z -= cosf(D3DX_PI * 0.50f) * VALUE_MOVE;
//		}
//	}
//	else if (Keyboard_IsPress(DIK_W))
//	{// 奥移動
//		g_posBox.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
//		g_posBox.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
//	}
//	else if (Keyboard_IsPress(DIK_S))
//	{// 手前移動
//		g_posBox.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
//		g_posBox.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
//	}
//
//#if 1
//	if (Keyboard_IsPress(DIK_Q))
//	{// Y軸左回転
//		g_rotBox.y -= VALUE_ROTATE;
//		if(g_rotBox.y > D3DX_PI)
//		{
//			g_rotBox.y -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.y < -D3DX_PI)
//		{
//			g_rotBox.y += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_E))
//	{// Y軸右回転
//		g_rotBox.y += VALUE_ROTATE;
//		if(g_rotBox.y > D3DX_PI)
//		{
//			g_rotBox.y -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.y < -D3DX_PI)
//		{
//			g_rotBox.y += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//#if 1
//	if (Keyboard_IsPress(DIK_UP))
//	{// X軸右回転
//		g_rotBox.x += VALUE_ROTATE;
//		if(g_rotBox.x > D3DX_PI)
//		{
//			g_rotBox.x -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.x < -D3DX_PI)
//		{
//			g_rotBox.x += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_DOWN))
//	{// X軸左回転
//		g_rotBox.x -= VALUE_ROTATE;
//		if(g_rotBox.x > D3DX_PI)
//		{
//			g_rotBox.x -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.x < -D3DX_PI)
//		{
//			g_rotBox.x += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//#if 1
//	if (Keyboard_IsPress(DIK_LEFT))
//	{// Z軸右回転
//		g_rotBox.z += VALUE_ROTATE;
//		if(g_rotBox.z > D3DX_PI)
//		{
//			g_rotBox.z -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.z < -D3DX_PI)
//		{
//			g_rotBox.z += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_RIGHT))
//	{// Z軸左回転
//		g_rotBox.z -= VALUE_ROTATE;
//		if(g_rotBox.z > D3DX_PI)
//		{
//			g_rotBox.z -= D3DX_PI * 2.0f;
//		}
//		if(g_rotBox.z < -D3DX_PI)
//		{
//			g_rotBox.z += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//	if (Keyboard_IsPress(DIK_RETURN))
//	{// リセット
//		g_posBox.x = 0.0f;
//		g_posBox.y = 0.0f;
//		g_posBox.z = 0.0f;
//
//		g_rotBox.x = 0.0f;
//		g_rotBox.y = 0.0f;
//		g_rotBox.z = 0.0f;
//	}

	//DebugProc_Print("*** ３Ｄポリゴン操作 ***\n");
	//DebugProc_Print("位置 [%f : %f : %f]\n", g_posBox.x, g_posBox.y, g_posBox.z);
	//DebugProc_Print("前移動 : Ｗ\n");
	//DebugProc_Print("後移動 : Ｓ\n");
	//DebugProc_Print("左移動 : Ａ\n");
	//DebugProc_Print("右移動 : Ｄ\n");
	//DebugProc_Print("\n");

	//DebugProc_Print("向き [%f : %f : %f]\n", g_rotBox.x, g_rotBox.y, g_rotBox.z);
	//DebugProc_Print("X軸回転 : ↑ / ↓\n");
	//DebugProc_Print("Y軸回転 : Ｑ / Ｅ\n");
	//DebugProc_Print("Z軸回転 : ← / →\n");
	//DebugProc_Print("\n");

	//DebugProc_Print("位置・向きリセット : ENTER\n");
}

//=============================================================================
// 描画処理
//=============================================================================
void Box_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	for (int i = 0; i < NUM_BOX; i++)
	{//ワールドマトリクスの設定

		//ワールドマトリックスの初期化
		D3DXMATRIX mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);

		pDevice->SetTransform(D3DTS_WORLD, &g_Box[i].g_mtxWorldBox);
		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffBox, 0, sizeof(VERTEX_3D));
		//インデックスバッファのセット
		pDevice->SetIndices(g_pIdxBuffBox);
		//頂点フォーマットに設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		//テクスチャの設定
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));
		//メモリからVRAMにぶち込む、すぐ消えない
		/*pDevice->DrawPrimitive (D3DPT_TRIANGLELIST,0,NUM_POLYGON);
		*/
		//ポリゴンの描画（インデックスバッファ版）
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumIndexBox, 0, 12);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice)
{
	//オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBox,
		NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		g_pVtxBuffBox->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, -BOX_DEPTH);
		pVtx[1].pos = D3DXVECTOR3(BOX_WIDTH, BOX_HEIGHT, -BOX_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, -BOX_DEPTH);
		pVtx[3].pos = D3DXVECTOR3(BOX_WIDTH, -BOX_HEIGHT, -BOX_DEPTH);
		pVtx[4].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
		pVtx[5].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
		pVtx[6].pos = D3DXVECTOR3(BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
		pVtx[7].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);

		pVtx[0].nor = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
		pVtx[4].nor = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		pVtx[5].nor = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
		pVtx[6].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[7].nor = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_pVtxBuffBox->Unlock();
	}

	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIndexBox,
												D3DUSAGE_WRITEONLY,
												D3DFMT_INDEX16,
												D3DPOOL_MANAGED,
												&g_pIdxBuffBox,
												NULL)))
	{
		return E_FAIL;
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		g_pIdxBuffBox->Lock(0, 0, (void**)&pIdx, 0);

		pIdx[0] = 0;
		pIdx[1] = 1;
		pIdx[2] = 2;
		pIdx[3] = 1;
		pIdx[4] = 3;
		pIdx[5] = 2;

		g_pIdxBuffBox->Unlock();
	}
	return S_OK;
}
