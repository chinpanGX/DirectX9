/*=============================================================================

	ビルボード処理 [billboard.cpp]
	Author : 出合翔太

==============================================================================*/

#include "billboard.h"
#include "Controller.h"
#include "camera.h"
#include "debugproc.h"

// マクロ定義
#define	BILLBOARD_SIZE_X		(20.0f)		// ビルボードの幅
#define	BILLBOARD_SIZE_Y		(20.0f)		// ビルボードの高さ
#define	VALUE_MOVE_BILLBOARD	(0.30f)		// 移動速度
#define	VALUE_JUMP				(10.0f)		// ジャンプ力
#define	VALUE_GRAVITY			(0.45f)		// 重力
#define	RATE_REGIST				(0.075f)	// 抵抗係数
#define	RATE_REFRECT			(-0.90f)	// 反射係数
#define	BILL_NUM_POY			(2)			//	ポリゴン
#define	BILL_NUM_VER			(4)			//	頂点


// スタティック変数
LPDIRECT3DVERTEXBUFFER9	BillBoard::m_pVtxBuffBillboard = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX				BillBoard::m_mtxWorldBillboard;			// ワールドマトリックス
D3DXVECTOR3				BillBoard::m_posBillboard;				// 位置
D3DXVECTOR3				BillBoard::m_sclBillboard;				// スケール
D3DXVECTOR3				BillBoard::m_moveBillboard;				// 移動量
int						BillBoard::m_nIdxShadowBillboard;		// 影ID
bool					BillBoard::m_bEnableGravity;			// 重力を有効にするかどうか
LPDIRECT3DDEVICE9		BillBoard::m_pDevice;


// 初期化処理
HRESULT BillBoard::Init()
{
	m_pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexBillboard(m_pDevice);

	// テクスチャの読み込み	引数(デバイスのポインタ、ファイル名、読み込むメモリポインタ)
	//D3DXCreateTextureFromFile(m_pDevice,"asset/texture/bullet000.png",&m_pTextureBillboard);	
	m_texture.Load("asset/texture/bullet000.png",1);

	//	位置、スケール、移動量の初期置
	m_posBillboard = D3DXVECTOR3(0.0f, 18.0f, 0.0f);
	m_sclBillboard = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_moveBillboard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	//m_IdxShadow = m_shadow.Create(m_posBillboard, m_sclBillboard);

	//	重力のフラグ
	m_bEnableGravity = false;

	return S_OK;
}

// 終了処理
void BillBoard::Uninit()
{
	m_texture.Unload(1);
	//SAFE_RELEASE(m_pTextureBillboard);	//	テクスチャの解放
	SAFE_RELEASE(m_pVtxBuffBillboard);	//	頂点バッファの解放
}


// 更新処理
void BillBoard::Update()
{
	Camera *pCamera;

	// カメラの取得
	pCamera = GetCamera();

	// ジャンプ
	if (KeyBoard::IsTrigger(DIK_J))
	{
		if(m_bEnableGravity == true)	//	重力フラグが有効な場合
		{
			m_moveBillboard.y = VALUE_JUMP;	//VALUE_JUMPの値を代入
		}
	}

	if(m_bEnableGravity == true)	//	重力フラグが有効な場合
	{
		m_posBillboard.y += m_moveBillboard.y;	//	m_moveBillboard.yの値を加算代入
		if(m_posBillboard.y < 9.0f)				//	9.0fよりうえに上がったら
		{
			m_posBillboard.y = 9.0f;
			m_moveBillboard.y *= RATE_REFRECT;	//	反射係数を乗算代入
		}
		m_moveBillboard.y -= VALUE_GRAVITY;		//	重力の値を除算
	}


	// 重力ON/OFF
	if (KeyBoard::IsTrigger(DIK_G))
	{
		m_bEnableGravity ^= 1;
		if(m_bEnableGravity == false)
		{
			m_moveBillboard.y = 0.0f;
		}
	}

	D3DXVECTOR3 pos = m_posBillboard;
	pos.y = 0.0f;	//	影は座標を固定しておく->影はジャンプしない
	m_shadow.SetPosition(m_IdxShadow, pos);

	DebugProc::Print((char*)"*** ボール操作 ***\n");
	DebugProc::Print((char*)"[ボールの位置  ：(%f : %f : %f)]\n", m_posBillboard.x, m_posBillboard.y, m_posBillboard.z);
	DebugProc::Print((char*)"[ボールの移動量：(%f : %f : %f)]\n", m_moveBillboard.x, m_moveBillboard.y, m_moveBillboard.z);
	DebugProc::Print((char*)"前移動   : ↑\n");
	DebugProc::Print((char*)"後移動   : ↓\n");
	DebugProc::Print((char*)"左移動   : ←\n");
	DebugProc::Print((char*)"右移動   : →\n");
	DebugProc::Print((char*)"ジャンプ : J\n");
	DebugProc::Print((char*)"\n");
	DebugProc::Print((char*)"*** 重力 ***\n");
	DebugProc::Print((char*)"ON / OFF : [G] ( 重力 = ");

	if(m_bEnableGravity)
	{
		DebugProc::Print((char*)"ON )\n");
	}
	else
	{
		DebugProc::Print((char*)"OFF )\n");
	}
}

// 描画処理
void BillBoard::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//	αテストを有効にする
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//	ラインティングを無効にする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//	ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorldBillboard);

	//	ビューマトリクスを取得
	m_pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//	ポリゴンを正面に向ける
#if 0
	//	逆行列を求める（第３引数の逆行列を求める）
	D3DXMatrixInverse(&m_mtxWorldBillboard, NULL, &mtxView);

	//	平行行列は無視する
	m_mtxWorldBillboard._41 = 0.0f;
	m_mtxWorldBillboard._42 = 0.0f;
	m_mtxWorldBillboard._43 = 0.0f;

#else
	//	転置行列
	m_mtxWorldBillboard._11 = mtxView._11;
	m_mtxWorldBillboard._12 = mtxView._21;
	m_mtxWorldBillboard._13 = mtxView._31;
	m_mtxWorldBillboard._21 = mtxView._12;
	m_mtxWorldBillboard._22 = mtxView._22;
	m_mtxWorldBillboard._23 = mtxView._32;
	m_mtxWorldBillboard._31 = mtxView._13;
	m_mtxWorldBillboard._32 = mtxView._23;
	m_mtxWorldBillboard._33 = mtxView._33;
#endif

	//	スケールを反映
	D3DXMatrixScaling(&mtxScale, m_sclBillboard.x, m_sclBillboard.y, m_sclBillboard.z);
	D3DXMatrixMultiply(&m_mtxWorldBillboard, &m_mtxWorldBillboard, &mtxScale);

	//	移動を反映
	D3DXMatrixTranslation(&mtxTranslate, m_posBillboard.x, m_posBillboard.y, m_posBillboard.z);
	D3DXMatrixMultiply(&m_mtxWorldBillboard, &m_mtxWorldBillboard, &mtxTranslate);

	//	ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldBillboard);

	//	頂点バッファをデバイスのデータストリームにバインド
	//	引数（頂点バッファを設定するストリームID、第2引数:頂点バッファへのポインタ、ストリームの先頭から頂点データの先頭までのオフセット、頂点データの一つ分のデータサイズをバイト単位で指定）
	m_pDevice->SetStreamSource(0, m_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	//	頂点フォーマットを設定
	m_pDevice->SetFVF(FVF_VERTEX3D);

	//	テクスチャの設定
	m_pDevice->SetTexture(0,m_texture.Set(1));

	//	ポリゴンの描画
	//	引数（プリミティブタイプ、配列の読み取り開始位置、三角ポリゴンの数）
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,BILL_NUM_POY);

	//	ラインティングを有効にする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	//	αテストを無効にする
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

// 頂点情報の作成
HRESULT BillBoard::MakeVertexBillboard(LPDIRECT3DDEVICE9 m_pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BILL_NUM_VER,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,				// 頂点バッファの使用法　
												FVF_VERTEX3D,					// 使用する頂点フォーマット
												D3DPOOL_MANAGED,				// リソースのバッファを保持するメモリクラスを指定
												&m_pVtxBuffBillboard,			// 頂点バッファインターフェースへのポインタ
												NULL)))							// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

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
		m_pVtxBuffBillboard->Unlock();
	}

	return S_OK;
}

// 頂点座標の設定
void BillBoard::SetVertexBillboard(float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		m_pVtxBuffBillboard->Unlock();
	}
}
