//=============================================================================
//
// エネミー処理 [enemy.cpp]
//
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_CAR			"asset/MODEL/car001.x"	// 読み込むモデル名
#define	VALUE_MOVE_MODEL	(0.50f)					// 移動速度
#define	RATE_MOVE_MODEL		(0.20f)					// 移動慣性係数
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.20f)					// 回転慣性係数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9	g_pTextureModel;	// テクスチャへのポインタ
static LPD3DXMESH			g_pMeshModel;		// メッシュ情報へのポインタ
static LPD3DXBUFFER			g_pBuffMatModel;	// マテリアル情報へのポインタ
static DWORD				g_nNumMatModel;		// マテリアル情報の総数

ENEMY g_Enemy[MAX_ENEMY];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Enemy_Initialize()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	g_pTextureModel = NULL;
	g_pMeshModel = NULL;
	g_pBuffMatModel = NULL;

	float offset = (50.0f * MAX_ENEMY / 2) - 25.0f;

	// 位置・向き・移動量の初期設定
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].posModel = D3DXVECTOR3(50.0f*i - offset, 0.0f, 50.0f);
		g_Enemy[i].rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//当たり判定用スフィアの半径
		g_Enemy[i].col_circle.r = 10.0f;

		//	AABB    
		g_Enemy[i].col_aabb.cx = 0.0f;
		g_Enemy[i].col_aabb.cy = 0.0f;
		g_Enemy[i].col_aabb.cz = 0.0f;

		g_Enemy[i].col_aabb.sx = 10.0f;
		g_Enemy[i].col_aabb.sy = 10.0f;
		g_Enemy[i].col_aabb.sz = 10.0f;

		//影の作成
    	//g_Enemy[i].idxShadow = m_Shadow.Create(g_Enemy[i].posModel, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		g_Enemy[i].bUse = true;
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_FILENAME,	// ファイルの名前
		&g_pTextureModel);	// 読み込むメモリー
#endif

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Enemy_Finalize(void)
{
	//影の解放処理
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//m_Shadow.Release(g_Enemy[i].idxShadow);
	}

	if(g_pTextureModel != NULL)
	{// テクスチャの開放
		g_pTextureModel->Release();
		g_pTextureModel = NULL;
	}

	if(g_pMeshModel != NULL)
	{// メッシュの開放
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	if(g_pBuffMatModel != NULL)
	{// マテリアルの開放
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Enemy_Update(void)
{
	//当たり判定用座標更新
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//有効フラグをチェック
		if (g_Enemy[i].bUse == false)
			continue;

		g_Enemy[i].col_circle.cx = g_Enemy[i].posModel.x;
		g_Enemy[i].col_circle.cy = g_Enemy[i].posModel.y;
		g_Enemy[i].col_circle.cz = g_Enemy[i].posModel.z;
		
		//	AABB
		g_Enemy[i].col_aabb.cx = g_Enemy[i].posModel.x;
		g_Enemy[i].col_aabb.cy = g_Enemy[i].posModel.y;
		g_Enemy[i].col_aabb.cz = g_Enemy[i].posModel.z;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Enemy_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//有効フラグをチェック
		if (g_Enemy[i].bUse == false)
			continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy[i].mtxWorldModel);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[i].rotModel.y, g_Enemy[i].rotModel.x, g_Enemy[i].rotModel.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorldModel, &g_Enemy[i].mtxWorldModel, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy[i].posModel.x, g_Enemy[i].posModel.y, g_Enemy[i].posModel.z);
		D3DXMatrixMultiply(&g_Enemy[i].mtxWorldModel, &g_Enemy[i].mtxWorldModel, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[i].mtxWorldModel);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pD3DXMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();


		for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureModel);

			// 描画
			g_pMeshModel->DrawSubset(nCntMat);
		}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}

bool Enemy_IsEnable(int index)
{
	return g_Enemy[index].bUse;
}

CIRCLE* Enemy_GetCollision(int index)
{
	return &g_Enemy[index].col_circle;
}

AABB * GetCollision(int index)
{
	return &g_Enemy[index].col_aabb;
}

ENEMY* GetEnemy(void)
{
	return &(g_Enemy[0]);
}

