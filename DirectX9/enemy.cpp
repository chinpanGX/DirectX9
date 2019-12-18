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

//	グローバル変数
Enemy g_Enemy[MAX_ENEMY];

// スタティック変数
LPDIRECT3DTEXTURE9	Enemy::m_pTextureModel;	// テクスチャへのポインタ
LPD3DXMESH			Enemy::m_pMeshModel;		// メッシュ情報へのポインタ
LPD3DXBUFFER		Enemy::m_pBuffMatModel;	// マテリアル情報へのポインタ
DWORD				Enemy::m_nNumMatModel;		// マテリアル情報の総数


HRESULT Enemy::Init()
{
	m_pDevice = GetD3DDevice();

	m_pTextureModel = NULL;
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;

	float offset = (50.0f * MAX_ENEMY / 2) - 25.0f;

	// 位置・向き・移動量の初期設定
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].m_pos = D3DXVECTOR3(50.0f*i - offset, 0.0f, 50.0f);
		g_Enemy[i].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//当たり判定用スフィアの半径
		//g_Enemy[i].col_circle.r = 10.0f;

		//	AABB    
		g_Enemy[i].m_aabb.cx = 0.0f;
		g_Enemy[i].m_aabb.cy = 0.0f;
		g_Enemy[i].m_aabb.cz = 0.0f;
		g_Enemy[i].m_aabb.sx = 10.0f;
		g_Enemy[i].m_aabb.sy = 10.0f;
		g_Enemy[i].m_aabb.sz = 10.0f;

		//影の作成
		//g_Enemy[i].idxShadow = m_Shadow.Create(g_Enemy[i].posModel, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		g_Enemy[i].m_bUse = true;
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(m_pDevice,				// デバイスへのポインタ
		TEXTURE_FILENAME,	// ファイルの名前
		&g_pTextureModel);	// 読み込むメモリー
#endif

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,
		D3DXMESH_SYSTEMMEM,
		m_pDevice,
		NULL,
		&m_pBuffMatModel,
		NULL,
		&m_nNumMatModel,
		&m_pMeshModel)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void Enemy::Uninit()
{
	//影の解放処理
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//m_Shadow.Release(g_Enemy[i].idxShadow);
	}
	
	SAFE_RELEASE(m_pTextureModel);
	SAFE_RELEASE(m_pMeshModel);
	SAFE_RELEASE(m_pBuffMatModel);
}

void Enemy::Update()
{
	//当たり判定用座標更新
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//有効フラグをチェック
		if (g_Enemy[i].m_bUse == false)
			continue;
		/*
		g_Enemy[i].col_circle.cx = g_Enemy[i].posModel.x;
		g_Enemy[i].col_circle.cy = g_Enemy[i].posModel.y;
		g_Enemy[i].col_circle.cz = g_Enemy[i].posModel.z;
		]*/
		//	AABB
		g_Enemy[i].m_aabb.cx = g_Enemy[i].m_pos.x;
		g_Enemy[i].m_aabb.cy = g_Enemy[i].m_pos.y;
		g_Enemy[i].m_aabb.cz = g_Enemy[i].m_pos.z;
	}
}

void Enemy::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//有効フラグをチェック
		if (g_Enemy[i].m_bUse == false)
			continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Enemy[i].m_mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[i].m_rot.y, g_Enemy[i].m_rot.x, g_Enemy[i].m_rot.z);
		D3DXMatrixMultiply(&g_Enemy[i].m_mtxWorld, &g_Enemy[i].m_mtxWorld, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Enemy[i].m_pos.x, g_Enemy[i].m_pos.y, g_Enemy[i].m_pos.z);
		D3DXMatrixMultiply(&g_Enemy[i].m_mtxWorld, &g_Enemy[i].m_mtxWorld, &mtxTranslate);

		// ワールドマトリックスの設定
		m_pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[i].m_mtxWorld);

		// 現在のマテリアルを取得
		m_pDevice->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pD3DXMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();


		for (int nCntMat = 0; nCntMat < (int)m_nNumMatModel; nCntMat++)
		{
			// マテリアルの設定
			m_pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			m_pDevice->SetTexture(0, m_pTextureModel);

			// 描画
			m_pMeshModel->DrawSubset(nCntMat);
		}

		// マテリアルをデフォルトに戻す
		m_pDevice->SetMaterial(&matDef);
	}
}

bool Enemy::IsEnable(int index)
{
	return false;
}

CIRCLE * Enemy::GetCircleCollision(int index)
{
	return &g_Enemy[index].m_circle;
}

AABB * Enemy::GetAABBCollision(int index)
{
	return &g_Enemy[index].m_aabb;
}

Enemy * Enemy::GetEnemy()
{
	return &(g_Enemy[0]);
}
