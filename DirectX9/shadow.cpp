/*=============================================================================

	[shadow.cpp]
	Author : 出合翔太

=============================================================================*/

#include "main.h"
#include "shadow.h"

// マクロ定義
#define	SHADOW_SIZE_X		(50.0f)							// 弾の幅
#define	SHADOW_SIZE_Z		(50.0f)							// 弾の高さ
#define	MAX_SHADOW			(128)							// 影最大数

// グローバル変数
Shadow					g_aShadow[MAX_SHADOW];		// 影ワーク
//	スタティック変数
LPDIRECT3DVERTEXBUFFER9  Shadow::m_pVtxBuffShadow;	// 頂点バッファへのポインタ

//	初期化処理
HRESULT Shadow::Init()
{
	m_pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexShadow(m_pDevice);

	// テクスチャの読み込み
	m_texture.Load("asset/texture/shadow000.jpg", 4);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].m_pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
		g_aShadow[nCntShadow].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].m_bUse = false;
	}

	return S_OK;
}

//	初期化処理
void Shadow::Uninit()
{
	SAFE_RELEASE(m_pVtxBuffShadow);
	m_texture.Unload(4);
}

void Shadow::Update()
{
}

void Shadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	Shadow *pShadow;

	///	<summary>
	///	減算合成
	///	今はサポートされていない
	///	αブレントでやるしかないが、真っ黒な画像を加算合成すれば、同じことができる。
	/// </summary>
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	//	結果　=　転送先(DEST)　－　転送元(SRC)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//	Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	pShadow = &g_aShadow[0];
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pShadow)
	{
		if (pShadow->m_bUse)
		{
			//	ワールドマトリクスを初期化
			D3DXMatrixIdentity(&pShadow->m_mtxWorld);

			//	回転
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->m_rot.y, pShadow->m_rot.x, pShadow->m_rot.z);
			D3DXMatrixMultiply(&pShadow->m_mtxWorld, &pShadow->m_mtxWorld, &mtxRot);

			//	移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pShadow->m_pos.x, pShadow->m_pos.y, pShadow->m_pos.z);
			D3DXMatrixMultiply(&pShadow->m_mtxWorld, &pShadow->m_mtxWorld, &mtxTranslate);

			//	ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->m_mtxWorld);

			//	頂点バッファをデバイスのデータストリームにバインド
			//	引数（頂点バッファを設定するストリームID、第2引数:頂点バッファへのポインタ、ストリームの先頭から頂点データの先頭までのオフセット、頂点データの一つ分のデータサイズをバイト単位で指定）
			pDevice->SetStreamSource(0, m_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//	頂点フォーマットを設定
			pDevice->SetFVF(FVF_VERTEX3D);

			//	テクスチャの設定
			pDevice->SetTexture(0, m_texture.Set(4));

			//	ポリゴンの描画
			//	引数（プリミティブタイプ、配列の読み取り開始位置、三角ポリゴンの数）
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntShadow * 4), 4);
		}
	}

	//	通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	//	結果　=　転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//	Z比較あり
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//	作成処理
int Shadow::Create(D3DXVECTOR3 m_pos, float fSizeX, float fSizeZ)
{
	int nIdxShadow = -1;

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (!g_aShadow[nCntShadow].m_bUse)	//	配列の空きを見つける
		{
			g_aShadow[nCntShadow].m_pos = m_pos;
			g_aShadow[nCntShadow].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aShadow[nCntShadow].m_bUse = true;

			SetVertex(nCntShadow, fSizeX, fSizeZ);

			nIdxShadow = nCntShadow;	//	空きを見つけた配列番号を保存
			break;
		}
	}

	return nIdxShadow;	//	空きを見つけた配列番号を返す
}

//	リリース処理
void Shadow::Release(int nIdxShadow)
{
	if (nIdxShadow >= 0 && nIdxShadow < MAX_SHADOW)
	{
		g_aShadow[nIdxShadow].m_bUse = false;
	}
}

//	位置の設定
void Shadow::SetPosition(int nIdxShadow, D3DXVECTOR3 m_pos)
{
	g_aShadow[nIdxShadow].m_pos = m_pos;
}

// 頂点カラーの設定
void Shadow::SetColor(int nIdxShadow, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxShadow * 4);

		// 頂点カラーの設定
		pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = col;

		// 頂点データをアンロックする
		m_pVtxBuffShadow->Unlock();
	}
}

//	頂点座標の設定
void Shadow::SetVertex(int nIdxShadow, float fSizeX, float fSizeZ)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxShadow * 4);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, 0.0f, fSizeZ / 2);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, 0.0f, fSizeZ / 2);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, 0.0f, -fSizeZ / 2);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, 0.0f, -fSizeZ / 2);

		// 頂点データをアンロックする
		m_pVtxBuffShadow->Unlock();
	}
}

// 頂点情報の作成
HRESULT Shadow::MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,				// 頂点バッファの使用法　
		FVF_VERTEX3D,					// 使用する頂点フォーマット
		D3DPOOL_MANAGED,				// リソースのバッファを保持するメモリクラスを指定
		&m_pVtxBuffShadow,				// 頂点バッファインターフェースへのポインタ
		NULL)))							// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
			pVtx[1].pos = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
			pVtx[2].pos = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);
			pVtx[3].pos = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		m_pVtxBuffShadow->Unlock();
	}

	return S_OK;
}

