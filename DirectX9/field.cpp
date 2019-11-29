
#include "field.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// 回転量

#define NUM_BOX 2

#define	FIELD_WIDTH		(100.0f)						// 地面の幅(X方向)
#define	FIELD_DEPTH		(100.0f)						// 地面の奥行(Z方向)
#define FIELD_HEIGHT     (100.0f)
#define BLOCK_X 4
#define BLOCK_Y 2

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffField = NULL;
int                     g_NumIndexField = 36;

int g_NumVertex = (BLOCK_X + 1)*(BLOCK_Y + 1);
int g_NumIndex = ((BLOCK_X + 1) * 2) * BLOCK_Y + (BLOCK_Y - 1) * 2;//(BLOCK_Y - 1) * 2は宿題ポリゴンを追加する
int g_NumPolygon = (BLOCK_X * 2)*BLOCK_Y + (BLOCK_Y - 1) * 4;

class blockField
{

public:

	D3DXMATRIX				g_mtxWorldField;		// ワールドマトリックス
	D3DXVECTOR3				g_posField;				// 地面の位置
	D3DXVECTOR3				g_rot1Field;				// 地面の向き(回転)
	D3DXVECTOR3				g_rot2Field;				// 地面の向き(回転)
	D3DXVECTOR3				g_sclField;				// 地面の大きさ(スケール)


};

typedef struct {
	D3DXMATRIX				g_mtxWorldField;		// ワールドマトリックス
	D3DXVECTOR3				g_posField;				// 地面の位置
	D3DXVECTOR3				g_rot1Field;				// 地面の向き(回転)
	D3DXVECTOR3				g_rot2Field;				// 地面の向き(回転)
	D3DXVECTOR3				g_sclField;				// 地面の大きさ(スケール)
}BOX_T;

BOX_T g_Box[NUM_BOX];

blockField g_blockField[2];


float g_move_x = 5.0f;
float g_scl_x = 0.01f;
float g_rot1_x = 0.01f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Field_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexField(pDevice);

	 //位置・回転・スケールの初期設定
	for (int i = 0; i < NUM_BOX; i++)
	{
		g_Box[i].g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		/*g_Box[i].g_rot1Field = D3DXVECTOR3(0.0f, 0.0f, 0.0f);*/
		g_Box[i].g_sclField = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Field_Finalize(void)
{
	if (g_pIdxBuffField != NULL)
	{//インデックスバッファ開放
		g_pIdxBuffField->Release();
		g_pIdxBuffField = NULL;
	}

	if(g_pVtxBuffField != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Field_Update(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	for (int i = 0; i < NUM_BOX; i++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Box[i].g_mtxWorldField);
	}

	//g_Box[0].g_rot1Field.y += 0.01f;

	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[0].g_rot1Field.y, g_Box[0].g_rot1Field.x, g_Box[0].g_rot1Field.z);
	//D3DXMatrixMultiply(&g_Box[0].g_mtxWorldField, &g_Box[0].g_mtxWorldField, &mtxRot);


	//g_Box[0].g_posField.x = 100.0f;
	//g_Box[0].g_posField.z = 100.0f;

	//D3DXMatrixTranslation(&mtxTranslate, g_Box[0].g_posField.x, g_Box[0].g_posField.y, g_Box[0].g_posField.z);
	//D3DXMatrixMultiply(&g_Box[0].g_mtxWorldField, &g_Box[0].g_mtxWorldField, &mtxTranslate);


	//g_Box[1].g_rot1Field.y -= 0.05f;

	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot1Field.y, g_Box[1].g_rot1Field.x, g_Box[1].g_rot1Field.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxRot);

	//g_Box[1].g_posField.x = 120.0f;
	//g_Box[1].g_posField.z = 100.0f;

	//D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posField.x, g_Box[1].g_posField.y, g_Box[1].g_posField.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxTranslate);

	//g_Box[1].g_rot2Field.y += 0.05f;

	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Box[1].g_rot2Field.y, g_Box[1].g_rot2Field.x, g_Box[1].g_rot2Field.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxRot);

	//D3DXVECTOR3 pos;
	//pos.x = 100.0f;
	//pos.y = 100.0f;
	//pos.z = 100.0f;

	//D3DXMatrixTranslation(&mtxTranslate, g_Box[1].g_posField.x, g_Box[1].g_posField.y, g_Box[1].g_posField.z);
	//D3DXMatrixMultiply(&g_Box[1].g_mtxWorldField, &g_Box[1].g_mtxWorldField, &mtxTranslate);

	//////四角の拡大縮小いじり
	//{
	//	/*g_sclField.x += g_scl_x;
	//	if (g_sclField.x > 3.0f || g_sclField.x < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}

	//	g_sclField.z += g_scl_x;
	//	if (g_sclField.z > 3.0f || g_sclField.z < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}

	//	g_sclField.y += g_scl_x;
	//	if (g_sclField.y > 3.0f || g_sclField.y < 1.0f)
	//	{
	//	g_scl_x *= -1;
	//	}
	//	*/
	//}
	////スケールを反映
	//{
	//	D3DXMatrixScaling(&mtxScl, g_sclField.x, g_sclField.y, g_sclField.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);
	//}	
	////四角の角度いじり
	//{
	//	//g_rotField.z = D3DXToRadian(45);
	//}
	////四角の回転
	//{
	//	g_rot1Field.y -= 0.03f;
	//	g_rot2Field.y += 0.03f;
	//}
	////中心を変更して回転
	//{
	//	/*
	//	回転反映→移動判定を移動判定→回転判定に変える
	//	*/
	//}
	////四角の移動
	///*{
	//	g_posField.x += g_move_x;
	//	if (g_posField.x >220.0f|| g_posField.x < -220.0f)
	//	{
	//	g_move_x*=-1 ;
	//	}

	//}
	//*/
	////固定位置
	//{

	//	g_posField.x = 300;
	//	g_posField.y = 100;
	//	g_posField.z = 300;

	//}
	////回転を反映
	///*
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_blockField[0].g_rot1Field.y, g_blockField[0].g_rot1Field.x, g_rot1Field.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	//}
	//
	////移動を反映
	//
	//{
	//	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	//}
	//
	////回転を反映(普通の回転)
	//
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot2Field.y, g_rot2Field.x, g_rot2Field.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	//}
	//
	////回転を反映
	//
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_blockField[0].g_rot1Field.y, g_rot1Field.x, g_rot1Field.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	//}*/
	///*
	////移動を反映
	//{
	//	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	//}
	//*/
	///*
	////回転を反映
	//{
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot2Field.y, g_rot2Field.x, g_rot2Field.z);
	//	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	//}
//	if (Keyboard_IsPress(DIK_A))
//	{
//		if (Keyboard_IsPress(DIK_W))
//		{// 左奥移動
//			g_posField.x += sinf(-D3DX_PI * 0.75f) * VALUE_MOVE;
//			g_posField.z -= cosf(-D3DX_PI * 0.75f) * VALUE_MOVE;
//		}
//		else if (Keyboard_IsPress(DIK_S))
//		{// 左手前移動
//			g_posField.x += sinf(-D3DX_PI * 0.25f) * VALUE_MOVE;
//			g_posField.z -= cosf(-D3DX_PI * 0.25f) * VALUE_MOVE;
//		}
//		else
//		{// 左移動
//			g_posField.x += sinf(-D3DX_PI * 0.50f) * VALUE_MOVE;
//			g_posField.z -= cosf(-D3DX_PI * 0.50f) * VALUE_MOVE;
//		}
//	}
//	else if (Keyboard_IsPress(DIK_D))
//	{
//		if (Keyboard_IsPress(DIK_W))
//		{// 右奥移動
//			g_posField.x += sinf(D3DX_PI * 0.75f) * VALUE_MOVE;
//			g_posField.z -= cosf(D3DX_PI * 0.75f) * VALUE_MOVE;
//		}
//		else if (Keyboard_IsPress(DIK_S))
//		{// 右手前移動
//			g_posField.x += sinf(D3DX_PI * 0.25f) * VALUE_MOVE;
//			g_posField.z -= cosf(D3DX_PI * 0.25f) * VALUE_MOVE;
//		}
//		else
//		{// 右移動
//			g_posField.x += sinf(D3DX_PI * 0.50f) * VALUE_MOVE;
//			g_posField.z -= cosf(D3DX_PI * 0.50f) * VALUE_MOVE;
//		}
//	}
//	else if (Keyboard_IsPress(DIK_W))
//	{// 奥移動
//		g_posField.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
//		g_posField.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
//	}
//	else if (Keyboard_IsPress(DIK_S))
//	{// 手前移動
//		g_posField.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
//		g_posField.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
//	}
//
//#if 1
//	if (Keyboard_IsPress(DIK_Q))
//	{// Y軸左回転
//		g_rotField.y -= VALUE_ROTATE;
//		if(g_rotField.y > D3DX_PI)
//		{
//			g_rotField.y -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.y < -D3DX_PI)
//		{
//			g_rotField.y += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_E))
//	{// Y軸右回転
//		g_rotField.y += VALUE_ROTATE;
//		if(g_rotField.y > D3DX_PI)
//		{
//			g_rotField.y -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.y < -D3DX_PI)
//		{
//			g_rotField.y += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//#if 1
//	if (Keyboard_IsPress(DIK_UP))
//	{// X軸右回転
//		g_rotField.x += VALUE_ROTATE;
//		if(g_rotField.x > D3DX_PI)
//		{
//			g_rotField.x -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.x < -D3DX_PI)
//		{
//			g_rotField.x += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_DOWN))
//	{// X軸左回転
//		g_rotField.x -= VALUE_ROTATE;
//		if(g_rotField.x > D3DX_PI)
//		{
//			g_rotField.x -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.x < -D3DX_PI)
//		{
//			g_rotField.x += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//#if 1
//	if (Keyboard_IsPress(DIK_LEFT))
//	{// Z軸右回転
//		g_rotField.z += VALUE_ROTATE;
//		if(g_rotField.z > D3DX_PI)
//		{
//			g_rotField.z -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.z < -D3DX_PI)
//		{
//			g_rotField.z += D3DX_PI * 2.0f;
//		}
//	}
//	if (Keyboard_IsPress(DIK_RIGHT))
//	{// Z軸左回転
//		g_rotField.z -= VALUE_ROTATE;
//		if(g_rotField.z > D3DX_PI)
//		{
//			g_rotField.z -= D3DX_PI * 2.0f;
//		}
//		if(g_rotField.z < -D3DX_PI)
//		{
//			g_rotField.z += D3DX_PI * 2.0f;
//		}
//	}
//#endif
//
//	if (Keyboard_IsPress(DIK_RETURN))
//	{// リセット
//		g_posField.x = 0.0f;
//		g_posField.y = 0.0f;
//		g_posField.z = 0.0f;
//
//		g_rotField.x = 0.0f;
//		g_rotField.y = 0.0f;
//		g_rotField.z = 0.0f;
//	}

	//DebugProc_Print("*** ３Ｄポリゴン操作 ***\n");
	//DebugProc_Print("位置 [%f : %f : %f]\n", g_posField.x, g_posField.y, g_posField.z);
	//DebugProc_Print("前移動 : Ｗ\n");
	//DebugProc_Print("後移動 : Ｓ\n");
	//DebugProc_Print("左移動 : Ａ\n");
	//DebugProc_Print("右移動 : Ｄ\n");
	//DebugProc_Print("\n");

	//DebugProc_Print("向き [%f : %f : %f]\n", g_rotField.x, g_rotField.y, g_rotField.z);
	//DebugProc_Print("X軸回転 : ↑ / ↓\n");
	//DebugProc_Print("Y軸回転 : Ｑ / Ｅ\n");
	//DebugProc_Print("Z軸回転 : ← / →\n");
	//DebugProc_Print("\n");

	//DebugProc_Print("位置・向きリセット : ENTER\n");
}

//=============================================================================
// 描画処理
//=============================================================================
void Field_Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	for (int i = 0; i < NUM_BOX; i++)
	{//ワールドマトリクスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Box[i].g_mtxWorldField);
		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));
		//インデックスバッファのセット
		pDevice->SetIndices(g_pIdxBuffField);
		//頂点フォーマットに設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		//テクスチャの設定
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));
		//メモリからVRAMにぶち込む、すぐ消えない
		/*pDevice->DrawPrimitive (D3DPT_TRIANGLELIST,0,NUM_POLYGON);
		*/
		//ポリゴンの描画（インデックスバッファ版）
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 8, 0, 12);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	//オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		//頂点データの範囲をロック
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for (int z = 0; z < 2; z++) {
			for (int x = 0; x < 4; x++) {
				pVtx[z * 4 + x].pos = D3DXVECTOR3(x*100.0f, 0.0f, z*-100.0f);
			}
		}

		/*pVtx[0].pos = D3DXVECTOR3(-FIELD_WIDTH, FIELD_HEIGHT, -FIELD_DEPTH);
		pVtx[1].pos = D3DXVECTOR3(FIELD_WIDTH, FIELD_HEIGHT, -FIELD_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(-FIELD_WIDTH, -FIELD_HEIGHT, -FIELD_DEPTH);
		pVtx[3].pos = D3DXVECTOR3(FIELD_WIDTH, -FIELD_HEIGHT, -FIELD_DEPTH);
		pVtx[4].pos = D3DXVECTOR3(-FIELD_WIDTH, -FIELD_HEIGHT, FIELD_DEPTH);
		pVtx[5].pos = D3DXVECTOR3(-FIELD_WIDTH, -FIELD_HEIGHT, FIELD_DEPTH);
		pVtx[6].pos = D3DXVECTOR3(FIELD_WIDTH, FIELD_HEIGHT, FIELD_DEPTH);
		pVtx[7].pos = D3DXVECTOR3(-FIELD_WIDTH, FIELD_HEIGHT, FIELD_DEPTH);*/

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

		pVtx[0].tex.x = 0.0f;
		pVtx[0].tex.y = 0.0f;
		pVtx[1].tex.x = 1.0f;
		pVtx[1].tex.y = 0.0f;
		pVtx[2].tex.x = 2.0f;
		pVtx[2].tex.y = 0.0f;
		pVtx[3].tex.x = 3.0f;
		pVtx[3].tex.y = 0.0f;
		pVtx[4].tex.x = 0.0f;
		pVtx[4].tex.y = 1.0f;
		pVtx[5].tex.x = 1.0f;
		pVtx[5].tex.y = 1.0f;
		pVtx[6].tex.x = 3.0f;
		pVtx[6].tex.y = 1.0f;
		pVtx[7].tex.x = 2.0f;
		pVtx[7].tex.y = 1.0f;


		g_pVtxBuffField->Unlock();
	}

	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * 8,
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

		pIdx[0] = 4;
		pIdx[1] = 0;
		pIdx[2] = 5;
		pIdx[3] = 1;
		pIdx[4] = 6;
		pIdx[5] = 2;
		pIdx[6] = 7;
		pIdx[7] = 3;

		g_pIdxBuffField->Unlock();
	}
	return S_OK;
}
