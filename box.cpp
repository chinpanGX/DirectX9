
#include "Box.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLYGON_NUM 2
#define VERTEX_NUM 6

#define	BOX_WIDTH		(100.0f)						// 地面の幅(X方向)
#define	BOX_DEPTH		(100.0f)						// 地面の奥行(Z方向)
#define	BOX_HEIGHT	(100.0f)							// 地面の高さ(Y方向)

#define NUM_CUBE		(2)								//立方体の数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBox = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffBox = NULL;

//ここを構造体にして描画したい立方体の分だけ増やす

struct CUBE_T
{
	D3DXMATRIX				g_mtxWorldBox;		// ワールドマトリックス
	D3DXVECTOR3				g_posBox;			// 地面の位置
	D3DXVECTOR3				g_rot1Box;			// 地面の向き(回転)
	D3DXVECTOR3				g_rot2Box;			// 地面の向き(回転)
	D3DXVECTOR3				g_sclBox;			// 地面の大きさ(スケール)
};

static CUBE_T g_cube[NUM_CUBE];

int g_NumIndexBox = 36;

static float move = 2.0f;
static float scl = 0.005f;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Box_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexBox(pDevice);

	// 位置・回転・スケールの初期設定
	for (int i = 0; i < NUM_CUBE; i++)
	{
		g_cube[i].g_posBox = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cube[i].g_rot1Box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cube[i].g_rot2Box = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cube[i].g_sclBox = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Box_Finalize(void)
{
	if (g_pIdxBuffBox != NULL)
	{// インデックスバッファの開放
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
//
//	DebugProc_Print((char*)"*** ３Ｄポリゴン操作 ***\n");
//	DebugProc_Print((char*)"位置 [%f : %f : %f]\n", g_posBox.x, g_posBox.y, g_posBox.z);
//	DebugProc_Print((char*)"前移動 : Ｗ\n");
//	DebugProc_Print((char*)"後移動 : Ｓ\n");
//	DebugProc_Print((char*)"左移動 : Ａ\n");
//	DebugProc_Print((char*)"右移動 : Ｄ\n");
//	DebugProc_Print((char*)"\n");
//
//	DebugProc_Print((char*)"向き [%f : %f : %f]\n", g_rotBox.x, g_rotBox.y, g_rotBox.z);
//	DebugProc_Print((char*)"X軸回転 : ↑ / ↓\n");
//	DebugProc_Print((char*)"Y軸回転 : Ｑ / Ｅ\n");
//	DebugProc_Print((char*)"Z軸回転 : ← / →\n");
//	DebugProc_Print((char*)"\n");
//
//	DebugProc_Print((char*)"位置・向きリセット : ENTER\n");

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;		//下から持ってきた

	for (int i = 0; i < NUM_CUBE; i++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_cube[i].g_mtxWorldBox);

		//スケールを反映
		D3DXMatrixScaling(&mtxScl, g_cube[i].g_sclBox.x, g_cube[i].g_sclBox.y, g_cube[i].g_sclBox.z);
		D3DXMatrixMultiply(&g_cube[i].g_mtxWorldBox, &g_cube[i].g_mtxWorldBox, &mtxScl);

		//うちけし回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_cube[i].g_rot1Box.y, g_cube[i].g_rot1Box.x, g_cube[i].g_rot1Box.z);
		D3DXMatrixMultiply(&g_cube[i].g_mtxWorldBox, &g_cube[i].g_mtxWorldBox, &mtxRot);

		//移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_cube[i].g_posBox.x, g_cube[i].g_posBox.y, g_cube[i].g_posBox.z);
		D3DXMatrixMultiply(&g_cube[i].g_mtxWorldBox, &g_cube[i].g_mtxWorldBox, &mtxTranslate);

		//回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_cube[i].g_rot2Box.y, g_cube[i].g_rot2Box.x, g_cube[i].g_rot2Box.z);
		D3DXMatrixMultiply(&g_cube[i].g_mtxWorldBox, &g_cube[i].g_mtxWorldBox, &mtxRot);
	}

	g_cube[0].g_sclBox.x = 0.5f;
	g_cube[0].g_sclBox.y = 0.5f;
	g_cube[0].g_sclBox.z = 0.5f;

	g_cube[1].g_sclBox.x = 1.0f;
	g_cube[1].g_sclBox.y = 1.0f;
	g_cube[1].g_sclBox.z = 1.0f;

	g_cube[0].g_rot2Box.y -= 0.05f;
	//g_cube[1].g_rot1Box.y += 0.05f;
	//g_cube[1].g_rot2Box.y -= 0.05f;
	g_cube[1].g_posBox.x = 150.0f;

	D3DXMatrixMultiply(&g_cube[1].g_mtxWorldBox,&g_cube[1].g_mtxWorldBox, &g_cube[0].g_mtxWorldBox);

	//この方法で回すときは1つだけ回すこと(ジンバルロックを避けるため)
	//2個以上同時に回すときはクォータニオンを使うこと


	//g_posBox.x = 250.0f;
	////g_posBox.y = 100.0f;
	////g_posBox.z = 100.0f;

	////g_rotBox.x = D3DXToRadian(45);
	////g_rotBox.z = D3DXToRadian(45);

	//g_rot1Box.y -= 0.05f;
	//g_rot2Box.x += 0.05f;
	//g_rot2Box.y += 0.05f;
	//g_rot2Box.z += 0.05f;

}

//=============================================================================
// 描画処理
//=============================================================================
void Box_Draw(void)
{
	//自分でマトリックス関数作ればこれ使わんでもいける？

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	//D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	////ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&g_mtxWorldBox);

	////スケールして回転して移動の順番は大事

	////スケールを反映
	//D3DXMatrixScaling(&mtxScl, g_sclBox.x, g_sclBox.y, g_sclBox.z);
	//D3DXMatrixMultiply(&g_mtxWorldBox,&g_mtxWorldBox,&mtxScl);

	////回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot,g_rotBox.y,g_rotBox.x,g_rotBox.z);
	//D3DXMatrixMultiply(&g_mtxWorldBox,&g_mtxWorldBox,&mtxRot);
	//
	////移動を反映
	//D3DXMatrixTranslation(&mtxTranslate,g_posBox.x,g_posBox.y, g_posBox.z);
	//D3DXMatrixMultiply(&g_mtxWorldBox, &g_mtxWorldBox, &mtxTranslate);

	for (int i = 0; i < NUM_CUBE; i++)
	{
		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_cube[i].g_mtxWorldBox);

		//
		pDevice->SetStreamSource(0, g_pVtxBuffBox, 0, sizeof(VERTEX_3D));

		//インデックスバッファのセット
		pDevice->SetIndices(g_pIdxBuffBox);

		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

		//ポリゴンの描画
		//pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,NUM_POLYGON);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_NumIndexBox, 0, 12);
	}

	/*
	//光が差す方向
	D3DXVECTOR3 light;
	light.x =  0.0f;
	light.y = -1.0f;
	light.z =  0.0f;

	//頂点バッファの中を埋める
	VERTEX_3D *pVtx;

	//頂点データの範囲をLockし、頂点バッファがなんたら
	g_pVtxBuffBox->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 calc_nor;
	
	//ベクトルに行列をかけて変形してくれる関数
	D3DXVec3TransformNormal(&calc_nor,&pVtx[0].nor,&g_mtxWorldBox);

	float brightness = D3DXVec3Dot(&light,&calc_nor);
	//内積の値がゼロを超える場合はゼロにする
	if (brightness > 0)
		brightness = 0;

	//-1～0の値を0～1に書き換える
	brightness = fabs(brightness);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(brightness, brightness, brightness, 1.0f);

	//頂点データをUnlockする
	g_pVtxBuffBox->Unlock();
	*/



	//DrawPrimitiveが今回新しく使う早いやつ
	//メモリ上に頂点情報を書いていた
	//DrawPrimitiveUPが前まで使っていた楽なやつ
	
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBox(LPDIRECT3DDEVICE9 pDevice)
{
	//オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBox, NULL)))
	{
		return E_FAIL;
	}

	{
		VERTEX_3D *pVtx;

		//頂点データの範囲をLockする
		g_pVtxBuffBox->Lock(0, 0, (void**)&pVtx, 0);

		//上下の面の頂点座標
		{
			pVtx [0].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			pVtx [1].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			pVtx [2].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);

			pVtx [3].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);
			pVtx [4].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);
			pVtx [5].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);


			pVtx [6].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
			pVtx [7].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
			//pVtx [8].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);

			//pVtx [9].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[10].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
			//pVtx[11].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
		}

		//手前奥の面の頂点座標
		{
			//pVtx[12].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[13].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[14].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);

			//pVtx[15].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[16].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[17].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT,-BOX_DEPTH);


			//pVtx[18].pos = D3DXVECTOR3(-BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);
			//pVtx[19].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);
			//pVtx[20].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
 
			//pVtx[21].pos = D3DXVECTOR3( BOX_WIDTH,-BOX_HEIGHT, BOX_DEPTH);
			//pVtx[22].pos = D3DXVECTOR3( BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
			//pVtx[23].pos = D3DXVECTOR3(-BOX_WIDTH, BOX_HEIGHT, BOX_DEPTH);
		}

		//左右の面の頂点座標
		{
			//pVtx[24].pos = D3DXVECTOR3( BOX_WIDTH,  BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[25].pos = D3DXVECTOR3( BOX_WIDTH,  BOX_HEIGHT, BOX_DEPTH);
			//pVtx[26].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT,-BOX_DEPTH);

			//pVtx[27].pos = D3DXVECTOR3( BOX_WIDTH,  BOX_HEIGHT, BOX_DEPTH);
			//pVtx[28].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);
			//pVtx[29].pos = D3DXVECTOR3( BOX_WIDTH, -BOX_HEIGHT,-BOX_DEPTH);


			//pVtx[30].pos = D3DXVECTOR3(-BOX_WIDTH,  BOX_HEIGHT, BOX_DEPTH);
			//pVtx[31].pos = D3DXVECTOR3(-BOX_WIDTH,  BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[32].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);

			//pVtx[33].pos = D3DXVECTOR3(-BOX_WIDTH,  BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[34].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT,-BOX_DEPTH);
			//pVtx[35].pos = D3DXVECTOR3(-BOX_WIDTH, -BOX_HEIGHT, BOX_DEPTH);

		}

		//法線ベクトル
		{
			pVtx[0].nor = D3DXVECTOR3(-1.0f, 1.0f,-1.0f);
			pVtx[1].nor = D3DXVECTOR3( 1.0f, 1.0f,-1.0f);
			pVtx[2].nor = D3DXVECTOR3(-1.0f,-1.0f,-1.0f);
			pVtx[3].nor = D3DXVECTOR3( 1.0f,-1.0f,-1.0f);
			pVtx[4].nor = D3DXVECTOR3( 1.0f,-1.0f, 1.0f);
			pVtx[5].nor = D3DXVECTOR3(-1.0f,-1.0f, 1.0f);
			pVtx[6].nor = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
			pVtx[7].nor = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
		}

		for (int i = 0; i < 8; i++)
		{
			//pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			switch (i % 4)
			{
			case 0:
				pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
				break;

			case 1:
				pVtx[i].tex = D3DXVECTOR2(1.0f, 0.0f);
				break;

			case 2:
				pVtx[i].tex = D3DXVECTOR2(0.0f, 1.0f);
				break;

			case 3:
				pVtx[i].tex = D3DXVECTOR2(1.0f, 1.0f);
				break;
			}
		}


		//頂点データをUnlockする
		g_pVtxBuffBox->Unlock();

		if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) *g_NumIndexBox, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffBox, NULL)))
		{
			return E_FAIL;
		}
	}

	//インデックスバッファの中身を埋める
	{
		WORD *pIdx;

		g_pIdxBuffBox->Lock(0,0,(void**)&pIdx,0);
	
		{
			//前面
			pIdx[0] = 0;
			pIdx[1] = 1;
			pIdx[2] = 2;
			pIdx[3] = 1;
			pIdx[4] = 3;
			pIdx[5] = 2;
			//上面
			pIdx[6] = 7;
			pIdx[7] = 6;
			pIdx[8] = 0;
			pIdx[9] = 6;
			pIdx[10] = 1;
			pIdx[11] = 0;
			//裏面
			pIdx[12] = 5;
			pIdx[13] = 4;
			pIdx[14] = 7;
			pIdx[15] = 4;
			pIdx[16] = 6;
			pIdx[17] = 7;
			//底面
			pIdx[18] = 2;
			pIdx[19] = 3;
			pIdx[20] = 5;
			pIdx[21] = 3;
			pIdx[22] = 4;
			pIdx[23] = 5;
			//右面
			pIdx[24] = 1;
			pIdx[25] = 6;
			pIdx[26] = 3;
			pIdx[27] = 6;
			pIdx[28] = 4;
			pIdx[29] = 3;
			//左面
			pIdx[30] = 7;
			pIdx[31] = 0;
			pIdx[32] = 5;
			pIdx[33] = 0;
			pIdx[34] = 2;
			pIdx[35] = 5;
		}


		g_pIdxBuffBox->Unlock();
	}


	return S_OK;
}


