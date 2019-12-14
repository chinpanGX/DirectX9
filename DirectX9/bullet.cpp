//=============================================================================
//
// 弾処理 [bullet.cpp]
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLET		"asset/TEXTURE/bullet000.png"	// 読み込むテクスチャファイル名
#define	BULLET_SIZE_X		(20.0f)							// ビルボードの幅
#define	BULLET_SIZE_Y		(20.0f)							// ビルボードの高さ
#define	VALUE_MOVE_BULLET	(0.30f)							// 移動速度

#define BULLET_NUM_VERTEX	4
#define BULLET_NUM_POLYGON	2

#define MAX_BULLET			100

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBullet(float fSizeX, float fSizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ

typedef struct{
	D3DXMATRIX				g_mtxWorldBullet;		// ワールドマトリックス
	D3DXVECTOR3				g_posBullet;			// 位置
	D3DXVECTOR3				g_sclBullet;			// スケール
	D3DXVECTOR3				g_moveBullet;			// 移動量
	int						g_nIdxShadowBullet;		// 影ID
	int						frame;
	bool					bUse;
}BULLET;

BULLET g_Bullet[MAX_BULLET];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Bullet_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexBullet(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,TEXTURE_BULLET,&g_pTextureBullet);	// 読み込むメモリー

	for (int i = 0; i < MAX_BULLET; i++)
	{
		g_Bullet[i].g_posBullet = D3DXVECTOR3(0.0f, 18.0f, 0.0f);
		g_Bullet[i].g_sclBullet = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[i].g_moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Bullet_Finalize(void)
{
	if(g_pTextureBullet != NULL)
	{// テクスチャの開放
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	if(g_pVtxBuffBullet != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Bullet_Update(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullet[i].bUse == true)
		{
			//時間を経過させる
			g_Bullet[i].frame++;

			//100フレーム経過で消滅
			if (g_Bullet[i].frame > 100)
			{
				//影の解放処理
				Shadow_Release(g_Bullet[i].g_nIdxShadowBullet);
				g_Bullet[i].bUse = false;
			}

			//座標の更新処理
			g_Bullet[i].g_posBullet += g_Bullet[i].g_moveBullet;

			//影の位置を弾に合わせる
			D3DXVECTOR3 pos = g_Bullet[i].g_posBullet;
			pos.y = 0.0f;
			Shadow_SetPosition(g_Bullet[i].g_nIdxShadowBullet, pos);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Bullet_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

#if 1	// 壁より先にビルボードを描画しているので、ここで対処
	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
#endif

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	for (int i = 0; i < MAX_BULLET; i++)
	{
		//有効フラグチェック
		if (g_Bullet[i].bUse == false)
			continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Bullet[i].g_mtxWorldBullet);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンを正面に向ける
#if 0
		// 逆行列をもとめる
		D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);

		// 平行移動の部分は無視する
		g_mtxWorldBillboard._41 = 0.0f;
		g_mtxWorldBillboard._42 = 0.0f;
		g_mtxWorldBillboard._43 = 0.0f;
#else
		g_Bullet[i].g_mtxWorldBullet._11 = mtxView._11;
		g_Bullet[i].g_mtxWorldBullet._12 = mtxView._21;
		g_Bullet[i].g_mtxWorldBullet._13 = mtxView._31;
		g_Bullet[i].g_mtxWorldBullet._21 = mtxView._12;	// ←
		g_Bullet[i].g_mtxWorldBullet._22 = mtxView._22;	// ←ここをコメントアウトすると垂直方向は無視
		g_Bullet[i].g_mtxWorldBullet._23 = mtxView._32;	// ←
		g_Bullet[i].g_mtxWorldBullet._31 = mtxView._13;
		g_Bullet[i].g_mtxWorldBullet._32 = mtxView._23;
		g_Bullet[i].g_mtxWorldBullet._33 = mtxView._33;
#endif

		// スケールを反映
		D3DXMatrixScaling(&mtxScale, g_Bullet[i].g_sclBullet.x,g_Bullet[i].g_sclBullet.y,g_Bullet[i].g_sclBullet.z);
		D3DXMatrixMultiply(&g_Bullet[i].g_mtxWorldBullet,&g_Bullet[i].g_mtxWorldBullet, &mtxScale);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Bullet[i].g_posBullet.x,g_Bullet[i].g_posBullet.y,g_Bullet[i].g_posBullet.z);
		D3DXMatrixMultiply(&g_Bullet[i].g_mtxWorldBullet,&g_Bullet[i].g_mtxWorldBullet, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[i].g_mtxWorldBullet);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, BULLET_NUM_POLYGON);

	}

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

#if 1
	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
#endif
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BULLET_NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffBullet,			// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pVtxBuffBullet->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBullet(float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffBullet->Unlock();
	}
}

void Bullet_Create(float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < MAX_BULLET; i++) {

		// もし弾がすでに有効だったら？
		if (g_Bullet[i].bUse) {
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_Bullet[i].g_posBullet.x = x;
		g_Bullet[i].g_posBullet.y = 0.0f;
		g_Bullet[i].g_posBullet.z = y;

		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_Bullet[i].g_moveBullet.x = dir.x;
		g_Bullet[i].g_moveBullet.y = 0.0f;
		g_Bullet[i].g_moveBullet.z = dir.y;

		// 有効フレームを初期化
		g_Bullet[i].frame = 0;

		//影を作成する
       	g_Bullet[i].g_nIdxShadowBullet = Shadow_Create( g_Bullet[i].g_posBullet,D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		// 弾を有効にする
		g_Bullet[i].bUse = true;

		break;
	}
}