/*=============================================================================

	[Cannon.cpp]
	Author : 出合翔太

==============================================================================*/

#include "Cannon.h"
#include "Controller.h"
#include "camera.h"
#include "debugproc.h"

// マクロ定義
#define	BILLBOARD_SIZE_X		(20.0f)		// ビルボードの幅
#define	BILLBOARD_SIZE_Y		(20.0f)		// ビルボードの高さ
#define	VALUE_MOVE				(0.30f)		// 移動速度
#define	VALUE_GRAVITY			(0.45f)		// 重力
#define	RATE_REGIST				(0.075f)	// 抵抗係数
#define	RATE_REFRECT			(-0.90f)	// 反射係数
#define	BILL_NUM_POY			(2)			//	ポリゴン
#define	BILL_NUM_VER			(4)			//	頂点
#define	MAX_CANNON				(100)

//	グローバル変数
Cannon	g_Cannon[MAX_CANNON];

// スタティック変数
LPDIRECT3DVERTEXBUFFER9	Cannon::m_pVtxBuffCannon = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX				Cannon::m_mtxWorldCannon;			// ワールドマトリックス
D3DXVECTOR3				Cannon::m_posCannon;				// 位置
D3DXVECTOR3				Cannon::m_sclCannon;				// スケール
D3DXVECTOR3				Cannon::m_moveCannon;				// 移動量
int						Cannon::m_nIdxShadowCannon;			// 影ID
bool					Cannon::m_bEnableGravity;			// 重力を有効にするかどうか
LPDIRECT3DDEVICE9		Cannon::m_pDevice;


// 初期化処理
HRESULT Cannon::Init()
{
	m_pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexCannon(m_pDevice);

	// テクスチャの読み込み	引数(デバイスのポインタ、ファイル名、読み込むメモリポインタ)
	//D3DXCreateTextureFromFile(m_pDevice,"asset/texture/bullet000.png",&m_pTextureCannon);	
	m_texture.Load("asset/texture/bullet000.png",1);

	for (int nCntCannon = 0; nCntCannon < MAX_CANNON; nCntCannon++)
	{
		//	位置、スケール、移動量の初期置
		g_Cannon[nCntCannon].m_posCannon  = D3DXVECTOR3(0.0f, 18.0f, 0.0f);
		g_Cannon[nCntCannon].m_sclCannon  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Cannon[nCntCannon].m_moveCannon = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		g_Cannon[nCntCannon].m_buse = false;
	}
	//m_IdxShadow = m_shadow.Create(m_posCannon, m_sclCannon);

	//	重力のフラグ
	//m_bEnableGravity = false;

	return S_OK;
}

// 終了処理
void Cannon::Uninit()
{
	m_texture.Unload(1);
	//SAFE_RELEASE(m_pTextureCannon);	//	テクスチャの解放
	SAFE_RELEASE(m_pVtxBuffCannon);	//	頂点バッファの解放
}


// 更新処理
void Cannon::Update()
{
	for (int nCntCannon = 0; nCntCannon < MAX_CANNON; nCntCannon++)
	{
		//	使用中
		if (g_Cannon[nCntCannon].m_buse == true)
		{
			g_Cannon[nCntCannon].m_frame++;	//	フレーム更新

			//	100フレーム経過
			if (g_Cannon[nCntCannon].m_frame > 100)
			{
				m_shadow.Release(g_Cannon[nCntCannon].m_IdxShadow);
				g_Cannon[nCntCannon].m_buse = false;
			}

			//	座標更新
			g_Cannon[nCntCannon], m_posCannon += g_Cannon[nCntCannon].m_moveCannon;

			//	影をつける
			D3DXVECTOR3 pos = g_Cannon[nCntCannon].m_posCannon;
			pos.y = 0.0f;	//	影は座標を固定しておく->影はジャンプしない
			m_shadow.SetPosition(m_IdxShadow, pos);
		}
	}
}

// 描画処理
void Cannon::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//	αテストを有効にする
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//	ライティングを無効にする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntCannon = 0; nCntCannon < MAX_CANNON; nCntCannon++)
	{
		//	使用中
		if (g_Cannon[nCntCannon].m_buse == false)
		{
			continue;
		}
		//	ワールドマトリクスの初期化
		D3DXMatrixIdentity(&g_Cannon[nCntCannon].m_mtxWorldCannon);

		//	ビューマトリクスを取得
		m_pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//	ポリゴンを正面に向ける
#if 0
	//	行列を求める（第３引数の逆行列を求める）
		D3DXMatrixInverse(&m_mtxWorldCannon, NULL, &mtxView);

		//	平行行列は無視する
		m_mtxWorldCannon._41 = 0.0f;
		m_mtxWorldCannon._42 = 0.0f;
		m_mtxWorldCannon._43 = 0.0f;

#else
	//	転置行列
		m_mtxWorldCannon._11 = mtxView._11;
		m_mtxWorldCannon._12 = mtxView._21;
		m_mtxWorldCannon._13 = mtxView._31;
		m_mtxWorldCannon._21 = mtxView._12;
		m_mtxWorldCannon._22 = mtxView._22;
		m_mtxWorldCannon._23 = mtxView._32;
		m_mtxWorldCannon._31 = mtxView._13;
		m_mtxWorldCannon._32 = mtxView._23;
		m_mtxWorldCannon._33 = mtxView._33;
#endif

		//	スケールを反映
		D3DXMatrixScaling(&mtxScale, g_Cannon[nCntCannon].m_sclCannon.x, g_Cannon[nCntCannon].m_sclCannon.y, g_Cannon[nCntCannon].m_sclCannon.z);
		D3DXMatrixMultiply(&g_Cannon[nCntCannon].m_mtxWorldCannon, &g_Cannon[nCntCannon].m_mtxWorldCannon, &mtxScale);

		//	移動を反映
		D3DXMatrixTranslation(&mtxTranslate, g_Cannon[nCntCannon].m_posCannon.x, g_Cannon[nCntCannon].m_posCannon.y, g_Cannon[nCntCannon].m_posCannon.z);
		D3DXMatrixMultiply(&g_Cannon[nCntCannon].m_mtxWorldCannon, &g_Cannon[nCntCannon].m_mtxWorldCannon, &mtxTranslate);

		//	ワールドマトリックスの設定
		m_pDevice->SetTransform(D3DTS_WORLD, &g_Cannon[nCntCannon].m_mtxWorldCannon);

		//	頂点バッファをデバイスのデータストリームにバインド
		//	引数（頂点バッファを設定するストリームID、第2引数:頂点バッファへのポインタ、ストリームの先頭から頂点データの先頭までのオフセット、頂点データの一つ分のデータサイズをバイト単位で指定）
		m_pDevice->SetStreamSource(0, m_pVtxBuffCannon, 0, sizeof(VERTEX_3D));

		//	頂点フォーマットを設定
		m_pDevice->SetFVF(FVF_VERTEX3D);

		//	テクスチャの設定
		m_pDevice->SetTexture(0, m_texture.Set(1));

		//	ポリゴンの描画
		//	引数（プリミティブタイプ、配列の読み取り開始位置、三角ポリゴンの数）
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, BILL_NUM_POY);
		
	}
	//	ラインティングを有効にする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	//	αテストを無効にする
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//弾の生成
void Cannon::Create(float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < MAX_CANNON; i++)
	{

		// もし弾がすでに有効だったら？
		if (g_Cannon[i].m_buse)
		{
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_Cannon[i].m_posCannon.x = x;
		g_Cannon[i].m_posCannon.y = 0.0f;
		g_Cannon[i].m_posCannon.z = y;

#if 0
		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_Cannon[i].m_moveCannon.x = dir.x;
		g_Cannon[i].m_moveCannon.y = dir.y;
		g_Cannon[i].m_posCannon.z = dir.y;

		// 弾の移動方向から弾の回転角度を求める
		g_Bullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		g_Bullets[i].collision.cx = x;
		g_Bullets[i].collision.cy = y;
		g_Bullets[i].collision.r = BULLET_WIDTH * 0.8f;
#endif
		// 有効フレームを初期化
		g_Cannon[i].m_frame = 0;

		g_Cannon[i].m_IdxShadow =m_shadow.Create(g_Cannon[i].m_posCannon,D3DXVECTOR3(0.0f,0.0f,1.0f));

		// 弾を有効にする
		g_Cannon[i].m_buse = true;

		break;
	}
}


// 頂点情報の作成
HRESULT Cannon::MakeVertexCannon(LPDIRECT3DDEVICE9 m_pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BILL_NUM_VER,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,				// 頂点バッファの使用法　
												FVF_VERTEX3D,					// 使用する頂点フォーマット
												D3DPOOL_MANAGED,				// リソースのバッファを保持するメモリクラスを指定
												&m_pVtxBuffCannon,				// 頂点バッファインターフェースへのポインタ
												NULL)))							// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffCannon->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE_X / 2, -BILLBOARD_SIZE_Y / 2, 0.0f);-

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
		m_pVtxBuffCannon->Unlock();
	}

	return S_OK;
}

// 頂点座標の設定
void Cannon::SetVertexCannon(float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffCannon->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		m_pVtxBuffCannon->Unlock();
	}
}
            