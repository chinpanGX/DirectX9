/*=============================================================================

	[cpp]
	Author : 出合翔太

=============================================================================*/
#include "Player.h"
#include "camera.h"
#include "Controller.h"
#include "shadow.h"
#include "bullet.h"

// マクロ定義
#define	MODEL_CAR			"asset/MODEL/car000.x"	// 読み込むモデル名
#define	VALUE_MOVE_MODEL	(0.5f)					// 移動速度
#define	RATE_MOVE_MODEL		(0.2f)					// 移動慣性係数
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.2f)					// 回転慣性係数

//	スタティック変数
LPDIRECT3DTEXTURE9	Player::m_pTextureModel = NULL;		// テクスチャへのポインタ
LPD3DXMESH			Player::m_pMeshModel	= NULL;		// メッシュ情報へのポインタ
LPD3DXBUFFER		Player::m_pBuffMatModel	= NULL;		// マテリアル情報へのポインタ
DWORD				Player::m_nNumMatModel;				// マテリアル情報の総数

//	初期化処理
HRESULT Player::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot )
{
	m_pDevice = GetD3DDevice();

	//位置・向き・移動の初期設定
	m_pos = pos;
	m_rot = rot;
	m_rotDest = rot;
	m_dir = 3;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,D3DXMESH_SYSTEMMEM,m_pDevice,NULL,&m_pBuffMatModel,NULL,&m_nNumMatModel,&m_pMeshModel)))
	{
		return E_FAIL;
	}

	//	影の作成
	m_IdxShadow = m_shadow.Create(m_pos, D3DXVECTOR3(0.7f,0.7f,0.7f));

	return S_OK;
}

void Player::Uninit()
{
	m_shadow.Release(m_IdxShadow);
	SAFE_RELEASE(m_pTextureModel);
	SAFE_RELEASE(m_pBuffMatModel);
	SAFE_RELEASE(m_pMeshModel);
}

void Player::Update()
{
	//	方向ベクトルの初期化
	D3DXVECTOR3 dir = D3DXVECTOR3(0.0f,0.0f,0.0f); 

	// カメラの取得
	Camera *m_pCamera = GetCamera();
	float fDiffRotY;

	//	前移動
	if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0, LEFTSTICK_UP))
	{
		dir.z -= -1;
		m_dir = 0;
		m_move.x += sinf(D3DX_PI * 1.0f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_move.z -= cosf(D3DX_PI * 1.0f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_rotDest.y = m_pCamera->rot.y + D3DX_PI * 1.0f;
	}
	//	後移動
	else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
	{
		dir.z += -1;
		m_dir = 3;
		m_move.x += sinf(D3DX_PI * 0.0f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_move.z -= cosf(D3DX_PI * 0.0f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_rotDest.y = m_pCamera->rot.y + D3DX_PI * 0.0f;
	}
	//	左移動
	else if (KeyBoard::IsPress(DIK_A) || GamePad::IsPress(0,LEFTSTICK_LEFT))
	{
		dir.x += -1;
		m_dir = 1;
		m_move.x += sinf(-D3DX_PI * 0.50f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_move.z -= cosf(-D3DX_PI * 0.50f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_rotDest.y = m_pCamera->rot.y + D3DX_PI * 0.50f;
	}
	//	右移動
	else if (KeyBoard::IsPress(DIK_D) || GamePad::IsPress(0, LEFTSTICK_RIGHT))
	{
		dir.x -= -1;
		m_dir = 2;
		m_move.x += sinf(D3DX_PI * 0.50f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_move.z -= cosf(D3DX_PI * 0.50f - m_pCamera->rot.y) * VALUE_MOVE_MODEL;
		m_rotDest.y = m_pCamera->rot.y - D3DX_PI * 0.50f;
	}

	// 目的の角度までの差分
	fDiffRotY = m_rotDest.y - m_rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	m_rot.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	//	弾を発射
    if (Mouse::LeftTrigger())
	{
		///	<summary>
		///	入力がされていない場合、向いている方向から発射方向を作成
		/// </summary>
		if (D3DXVec3Length(&dir) < 0.01f)
		{
			switch (m_dir)
			{
			case 0:	//	前向き
				dir.z = 1.0f;
				break;
			case 1:	//	左向き
				dir.x = -1.0f;
				break;
			case 2:	//	右向き
				dir.x = 1.0f;
				break;
			case 3:	//	後向き
				dir.z = -1.0f;
				break;
			}
		}
		m_bullet.Create(m_pos.x, 6.5f, m_pos.z, D3DXVECTOR3(dir));
		dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	/// 位置移動
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;


	// 移動量に慣性をかける
	m_move.x += (0.0f - m_move.x) * RATE_MOVE_MODEL;
	m_move.z += (0.0f - m_move.z) * RATE_MOVE_MODEL;

	// 範囲チェック
#if 0
	if (g_m_pos.x < -310.0f)
	{
		g_m_pos.x = -310.0f;
	}
	if (g_m_pos.x > 310.0f)
	{
		g_m_pos.x = 310.0f;
	}
	if (g_m_pos.z < -310.0f)
	{
		g_m_pos.z = -310.0f;
	}
	if (g_m_pos.z > 310.0f)
	{
		g_m_pos.z = 310.0f;
	}
#endif

	if (KeyBoard::IsPress(DIK_RETURN))
	{// リセット
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	D3DXVECTOR3 pos = m_pos;
	pos.y = 0.0f;	//	影は座標を固定しておく->影はジャンプしない
	m_shadow.SetPosition(m_IdxShadow, pos);
}

void Player::Draw()
{
	m_pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorldModel);

	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorldModel, &m_mtxWorldModel, &mtxRot);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorldModel, &m_mtxWorldModel, &mtxTranslate);


	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldModel);

	//現在のマテリアルを取得
	m_pDevice->GetMaterial(&matDef);

	//マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		m_pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
		//テクスチャの設定
		m_pDevice->SetTexture(0, m_pTextureModel);
		//描画
		m_pMeshModel->DrawSubset(nCntMat);

	}
	//マテリアルをデフォルトに戻す
	m_pDevice->SetMaterial(&matDef);
}
