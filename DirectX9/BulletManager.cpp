/*=======================================================

	[BulletManager.cpp]
	Author : 出合翔太

=========================================================*/

#include "BulletManager.h"

#define	BULLET_NUM_VERTEX	4
#define	BULLET_NUM_POYGON	2

//	スタティック変数
LPDIRECT3DVERTEXBUFFER9	BulletManager::m_pVtxBuffBullet;		//	頂点バッファへのポインタ

HRESULT BulletManager::Init()
{
	m_pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexBullet(m_pDevice);

	/// <summary>
	///	テクスチャのロード
	/// </summary>
	///	<argument> ("ファイルパス名",格納配列番号) </argument>
	m_texture.Load("asset/texture/bullet000.png", 1);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		m_aBullet[nCntBullet].m_position = D3DXVECTOR3(0.0f, 18.0f, 0.0f);
		m_aBullet[nCntBullet].m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_aBullet[nCntBullet].m_velocity = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_aBullet[nCntBullet].m_bEnableGravity = true;
		m_aBullet[nCntBullet].m_fSizeX = SIZE_X;
		m_aBullet[nCntBullet].m_fSizeY = SIZE_Y;
		m_aBullet[nCntBullet].m_life = 0;
		m_aBullet[nCntBullet].m_bUse = false;
		m_shadow.Create(m_aBullet[nCntBullet].m_position, m_aBullet[nCntBullet].m_scale);
	}
	m_posBase = D3DXVECTOR3(0.0f,18.0f,0.0f);
	m_roty = 0.0f;
	m_spd = 0.0f;

	return S_OK;
}

void BulletManager::Uninit()
{
	SAFE_RELEASE(m_pVtxBuffBullet);
	//m_texture.Unload(1);
}

void BulletManager::Update()
{
	m_camera = GetCamera();
	m_posBase.x = 0;
	m_posBase.z = 0;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//	使用中
		if (m_aBullet[nCntBullet].m_bUse == true)
		{
			m_aBullet[nCntBullet].m_position.x += m_aBullet[nCntBullet].m_velocity.x;
			m_aBullet[nCntBullet].m_position.z += m_aBullet[nCntBullet].m_velocity.z;
			m_aBullet[nCntBullet].m_position.y += m_aBullet[nCntBullet].m_velocity.y;
			
			D3DXVECTOR3 pos = m_aBullet[nCntBullet].m_position;
			pos.y = 0.0f;	//	影は座標を固定しておく->影はジャンプしない
			m_shadow.SetPosition(m_IdxShadow, pos);

			//	着地
			if (m_aBullet[nCntBullet].m_position.y <= m_aBullet[nCntBullet].m_fSizeY / 2)
			{
				m_aBullet[nCntBullet].m_position.y = m_aBullet[nCntBullet].m_fSizeY / 2;
				m_aBullet[nCntBullet].m_velocity.y = -m_aBullet[nCntBullet].m_velocity.y * 0.75f;
			}

			m_aBullet[nCntBullet].m_velocity.x += (0.0f - m_aBullet[nCntBullet].m_velocity.x) * 0.015f;
			m_aBullet[nCntBullet].m_velocity.y -= 0.25f;
			m_aBullet[nCntBullet].m_velocity.z += (0.0f - m_aBullet[nCntBullet].m_velocity.z) * 0.015f;
			
			m_aBullet[nCntBullet].m_life--;	//	ライフのデクリメント
			if (m_aBullet[nCntBullet].m_life <= 0)
			{
				m_aBullet[nCntBullet].m_bUse = false;
			}
		}
	}

	//	弾の発生
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 move;
		float fAngle, fLength;
		int nLife;
		float fSize;

		pos = m_posBase;
		fAngle = (float)(rand() % 628 - 314) / 100.0f;
		fLength = rand() % (int)(5.0f * 200) / 100.0f - 5.0f;
		move.x = sinf(fAngle) * fLength;
		move.y = rand() % 300 / 100.0f + 10.0f;
		move.z = cosf(fAngle) * fLength;
		nLife = rand() % 100 + 150;
		fSize = (float)(rand() % 30 + 20);
		pos.y = fSize / 2;
		SetPosition(pos, move, fSize, fSize, nLife);	// ビルボードの設定
	}
}

void BulletManager::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//	αテストを有効にする
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//	ラインティングを無効にする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (m_aBullet[nCntBullet].m_bUse)
		{

			//	ワールドマトリクスの初期化
			D3DXMatrixIdentity(&m_mtxWorld);

			//	ビューマトリクスを取得
			m_pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//	転置行列
			m_mtxWorld._11 = mtxView._11;
			m_mtxWorld._12 = mtxView._21;
			m_mtxWorld._13 = mtxView._31;
			m_mtxWorld._21 = mtxView._12;
			m_mtxWorld._22 = mtxView._22;
			m_mtxWorld._23 = mtxView._32;
			m_mtxWorld._31 = mtxView._13;
			m_mtxWorld._32 = mtxView._23;
			m_mtxWorld._33 = mtxView._33;

			//	スケールを反映
			D3DXMatrixScaling(&mtxScale, m_aBullet[nCntBullet].m_scale.x, m_aBullet[nCntBullet].m_scale.y, m_aBullet[nCntBullet].m_scale.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

			//	移動を反映
			D3DXMatrixTranslation(&mtxTranslate, m_aBullet[nCntBullet].m_position.x, m_aBullet[nCntBullet].m_position.y, m_aBullet[nCntBullet].m_position.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

			//	ワールドマトリックスの設定
			m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

			//	頂点バッファをデバイスのデータストリームにバインド
			//	引数（頂点バッファを設定するストリームID、第2引数:頂点バッファへのポインタ、ストリームの先頭から頂点データの先頭までのオフセット、頂点データの一つ分のデータサイズをバイト単位で指定）
			m_pDevice->SetStreamSource(0, m_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//	頂点フォーマットを設定
			m_pDevice->SetFVF(FVF_VERTEX3D);

			//	テクスチャの設定
			m_pDevice->SetTexture(0, m_texture.Set(1));

			//	ポリゴンの描画
			//	引数（プリミティブタイプ、配列の読み取り開始位置、三角ポリゴンの数）
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBullet * 4), BULLET_NUM_POYGON);
		}
	}
	//	ラインティングを有効にする
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//	αテストを無効にする
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT BulletManager::MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(m_pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BULLET_NUM_VERTEX * MAX_BULLET,	// 頂点データ用に確保するバッファサイズ(バイト単位)
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
		pVtx[0].pos = D3DXVECTOR3(-20.0f / 2, 20.0f / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20.0f / 2, 20.0f / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-20.0f / 2, -20.0f / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20.0f / 2, -20.0f / 2, 0.0f); 
		
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

void BulletManager::SetVertexBullet(int nIdx, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdx * 4);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		m_pVtxBuffBullet->Unlock();
	}
}

int BulletManager::SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 velocity, float fSizeX, float fSizeY, int nLife)
{
	int nIdxBullet = -1;
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!m_aBullet[nCntBullet].m_bUse)	//	空き状態の配列番号を探す
		{
			m_aBullet[nCntBullet].m_position = pos;
			m_aBullet[nCntBullet].m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			m_aBullet[nCntBullet].m_velocity = velocity;
			m_aBullet[nCntBullet].m_fSizeX = fSizeX;
			m_aBullet[nCntBullet].m_fSizeY = fSizeY;
			m_aBullet[nCntBullet].m_life = nLife;
			m_aBullet[nCntBullet].m_bUse = true;
			SetVertexBullet(nCntBullet, fSizeX, fSizeY);	//	頂点座標の指定
			m_IdxShadow = m_shadow.Create(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			nIdxBullet = nCntBullet;	//	見つけた配列番号を代入
			break;
		}
	}
	return nIdxBullet;	//	配列番号をリターン
}
