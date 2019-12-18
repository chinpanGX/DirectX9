//=============================================================================
//
// 弾処理 [bullet.cpp]
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

// マクロ定義
#define	BULLET_SIZE_X		(20.0f)							// ビルボードの幅
#define	BULLET_SIZE_Y		(20.0f)							// ビルボードの高さ
#define	VALUE_MOVE_BULLET	(5.0f)							// 移動速度
#define BULLET_NUM_VERTEX	4
#define BULLET_NUM_POLYGON	2 

//	グローバル変数
Bullet g_Bullet[MAX_BULLET];

//	スタティック変数
LPDIRECT3DVERTEXBUFFER9	Bullet::m_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ

//	弾の初期化処理
HRESULT Bullet::Init()
{
	m_pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexBullet(m_pDevice);

	// テクスチャの読み込み
	m_texture.Load("asset/TEXTURE/bullet000.png" ,1);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_Bullet[nCntBullet].m_pos = D3DXVECTOR3(0.0f, 18.0f, 0.0f);
		g_Bullet[nCntBullet].m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[nCntBullet].m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//スフィア  
		/*
		g_Bullet[nCntBullet].col_circle.cx = 0.0f;
		g_Bullet[nCntBullet].col_circle.cy = 0.0f;
		g_Bullet[nCntBullet].col_circle.cz = 0.0f;
		g_Bullet[nCntBullet].col_circle.r = 10.0f; 
		*/
		//	AABB    
		g_Bullet[nCntBullet].col_aabb.cx = 0.0f;
	    g_Bullet[nCntBullet].col_aabb.cy = 0.0f;
		g_Bullet[nCntBullet].col_aabb.cz = 0.0f;
		g_Bullet[nCntBullet].col_aabb.sx = 10.0f;
		g_Bullet[nCntBullet].col_aabb.sy = 10.0f;
		g_Bullet[nCntBullet].col_aabb.sz = 10.0f;

		g_Bullet[nCntBullet].m_bUse = false;
	}

	return S_OK;
}

//	弾の終了処理
void Bullet::Uninit()
{
	m_texture.Unload(1);
	SAFE_RELEASE(m_pVtxBuffBullet);
}

//	弾の更新処理
void Bullet::Update()
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_Bullet[nCntBullet].m_bUse == true)
		{
			//時間を経過させる
			g_Bullet[nCntBullet].m_frame++;

			//100フレーム経過で消滅
			if (g_Bullet[nCntBullet].m_frame > 100)
			{
				//影の解放処理
				m_shadow.Release(g_Bullet[nCntBullet].m_nIdxShadowBullet);
				g_Bullet[nCntBullet].m_bUse = false;
			}

			//座標の更新処理
			g_Bullet[nCntBullet].m_pos += g_Bullet[nCntBullet].m_move * VALUE_MOVE_BULLET;

			/*
			//当たり判定用座標更新
			g_Bullet[nCntBullet].col_circle.cx = g_Bullet[nCntBullet].m_pos.x;
			g_Bullet[nCntBullet].col_circle.cy = g_Bullet[nCntBullet].m_pos.y;
			g_Bullet[nCntBullet].col_circle.cz = g_Bullet[nCntBullet].m_pos.z; 
			*/
			//	AABB
			g_Bullet[nCntBullet].col_aabb.cx = g_Bullet[nCntBullet].m_pos.x;
			g_Bullet[nCntBullet].col_aabb.cy = g_Bullet[nCntBullet].m_pos.y;
			g_Bullet[nCntBullet].col_aabb.cz = g_Bullet[nCntBullet].m_pos.z;

			//影の位置を弾に合わせる
			D3DXVECTOR3 pos = g_Bullet[nCntBullet].m_pos;
			pos.y = 0.0f;
			m_shadow.SetPosition(g_Bullet[nCntBullet].m_nIdxShadowBullet, pos);
		}
	}
}

//	弾の描画処理
void Bullet::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

#if 1	// 壁より先にビルボードを描画しているので、ここで対処
	// αテストを有効に
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
#endif

	// ライティングを無効にする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//有効フラグチェック
		if (g_Bullet[nCntBullet].m_bUse == false)
			continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Bullet[nCntBullet].m_mtxWorldBullet);

		// ビューマトリックスを取得
		m_pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//	転置行列
		g_Bullet[nCntBullet].m_mtxWorldBullet._11 = mtxView._11;
		g_Bullet[nCntBullet].m_mtxWorldBullet._12 = mtxView._21;
		g_Bullet[nCntBullet].m_mtxWorldBullet._13 = mtxView._31;
		g_Bullet[nCntBullet].m_mtxWorldBullet._21 = mtxView._12;	// ←
		g_Bullet[nCntBullet].m_mtxWorldBullet._22 = mtxView._22;	// ←ここをコメントアウトすると垂直方向は無視
		g_Bullet[nCntBullet].m_mtxWorldBullet._23 = mtxView._32;	// ←
		g_Bullet[nCntBullet].m_mtxWorldBullet._31 = mtxView._13;
		g_Bullet[nCntBullet].m_mtxWorldBullet._32 = mtxView._23;
		g_Bullet[nCntBullet].m_mtxWorldBullet._33 = mtxView._33;

		// スケールを反映
		D3DXMatrixScaling(&mtxScale, g_Bullet[nCntBullet].m_scl.x, g_Bullet[nCntBullet].m_scl.y, g_Bullet[nCntBullet].m_scl.z);
		D3DXMatrixMultiply(&g_Bullet[nCntBullet].m_mtxWorldBullet, &g_Bullet[nCntBullet].m_mtxWorldBullet, &mtxScale);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Bullet[nCntBullet].m_pos.x, g_Bullet[nCntBullet].m_pos.y, g_Bullet[nCntBullet].m_pos.z);
		D3DXMatrixMultiply(&g_Bullet[nCntBullet].m_mtxWorldBullet, &g_Bullet[nCntBullet].m_mtxWorldBullet, &mtxTranslate);

		// ワールドマトリックスの設定
		m_pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[nCntBullet].m_mtxWorldBullet);

		// 頂点バッファをデバイスのデータストリームにバインド
		m_pDevice->SetStreamSource(0, m_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		m_pDevice->SetFVF(FVF_VERTEX3D);

		// テクスチャの設定
		m_pDevice->SetTexture(0, m_texture.Set(1));

		// ポリゴンの描画
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, BULLET_NUM_POLYGON);

	}

	// ラインティングを有効にする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//	弾の生成
void Bullet::Create(float x, float y, float z, D3DXVECTOR3 dir)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_Bullet[nCntBullet].m_bUse == true)
		{
			continue;
		}
		// 弾の座標に引数の値を代入する
		g_Bullet[nCntBullet].m_pos.x = x;
		g_Bullet[nCntBullet].m_pos.y = y;
		g_Bullet[nCntBullet].m_pos.z = z;

		// 弾の移動方向を指定する
		D3DXVec3Normalize(&dir, &dir);
		g_Bullet[nCntBullet].m_move.x = dir.x;
		g_Bullet[nCntBullet].m_move.y = dir.y;
		g_Bullet[nCntBullet].m_move.z = dir.z;

		// 有効フレームを初期化
		g_Bullet[nCntBullet].m_frame = 0;

		//影を作成する
		g_Bullet[nCntBullet].m_nIdxShadowBullet = m_shadow.Create(g_Bullet[nCntBullet].m_pos, D3DXVECTOR3(0.7f, 0.7f, 0.7f));

		// 弾を有効にする
		g_Bullet[nCntBullet].m_bUse = true;

		break;
	}
	
}

//	頂点情報の生成
HRESULT Bullet::MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BULLET_NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&m_pVtxBuffBullet,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 頂点データをアンロックする
		m_pVtxBuffBullet->Unlock();
	}

	return S_OK;
}

//	頂点座標の設定
void Bullet::SetVertexBullet(float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		m_pVtxBuffBullet->Unlock();
	}
}

bool Bullet::IsEnable(int index)
{
	return g_Bullet[index].m_bUse;
}

CIRCLE * Bullet::GetCollision(int index)
{
	return &g_Bullet[index].col_circle;
}

AABB * Bullet::GetCollisionBox(int index)
{
	return &g_Bullet[index].col_aabb;
}

Bullet * Bullet::GetBullet(void)
{
	return &(g_Bullet[0]);
}
