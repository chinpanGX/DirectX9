/*=============================================================================

	[cpp]
	Author : 出合翔太

=============================================================================*/
#include "Player.h"
#include "camera.h"
#include "Controller.h"
#include "shadow.h"

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


HRESULT Player::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pDevice = GetD3DDevice();

	//位置・向き・移動の初期設定
	posModel = pos;
	rotModel = rot;
	rotDestModel = rot;
	moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//Xファイルの読み込み
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

	//	影の作成
	m_IdxShadow = m_shadow.Create(posModel, 0.1f, 0.1f);

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
	Camera *pCamera;
	float fDiffRotY;

	// カメラの取得
	pCamera = GetCamera();

	if (KeyBoard::IsPress(DIK_A) || GamePad::IsPress(0,LEFTSTICK_LEFT))
	{
		if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0,LEFTSTICK_UP))
		{// 左奥移動
			moveModel.x += sinf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(-D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.75f;
		}
		else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
		{// 左手前移動
			moveModel.x += sinf(-D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(-D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.25f;
		}
		else
		{// 左移動
			moveModel.x += sinf(-D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(-D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.50f;
		}
	}
	else if (KeyBoard::IsPress(DIK_D) || GamePad::IsPress(0, LEFTSTICK_RIGHT))
	{
		if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0, LEFTSTICK_UP))
		{// 右奥移動
			moveModel.x += sinf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(D3DX_PI * 0.75f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.75f;
		}
		else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
		{// 右手前移動
			moveModel.x += sinf(D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(D3DX_PI * 0.25f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.25f;
		}
		else
		{// 右移動
			moveModel.x += sinf(D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;
			moveModel.z -= cosf(D3DX_PI * 0.50f - pCamera->rot.y) * VALUE_MOVE_MODEL;

			rotDestModel.y = pCamera->rot.y - D3DX_PI * 0.50f;
		}
	}
	else if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0, LEFTSTICK_UP))
	{// 前移動
		moveModel.x += sinf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		moveModel.z -= cosf(D3DX_PI * 1.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;

		rotDestModel.y = pCamera->rot.y + D3DX_PI * 1.0f;
	}
	else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
	{// 後移動
		moveModel.x += sinf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;
		moveModel.z -= cosf(D3DX_PI * 0.0f - pCamera->rot.y) * VALUE_MOVE_MODEL;

		rotDestModel.y = pCamera->rot.y + D3DX_PI * 0.0f;
	}

	if (KeyBoard::IsPress(DIK_Q) || GamePad::IsPress(0,PS4RIGHTSTICK_LEFT))
	{// 左回転
		rotDestModel.y -= VALUE_ROTATE_MODEL;
		if (rotDestModel.y < -D3DX_PI)
		{
			rotDestModel.y += D3DX_PI * 2.0f;
		}
	}
	if (KeyBoard::IsPress(DIK_E) || GamePad::IsPress(0,PS4RIGHTSTICK_RIGHT))
	{// 右回転
		rotDestModel.y += VALUE_ROTATE_MODEL;
		if (rotDestModel.y > D3DX_PI)
		{
			rotDestModel.y -= D3DX_PI * 2.0f;
		}
	}

	// 目的の角度までの差分
	fDiffRotY = rotDestModel.y - rotModel.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	rotModel.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (rotModel.y > D3DX_PI)
	{
		rotModel.y -= D3DX_PI * 2.0f;
	}
	if (rotModel.y < -D3DX_PI)
	{
		rotModel.y += D3DX_PI * 2.0f;
	}

	/// 位置移動
	posModel.x += moveModel.x;
	posModel.z += moveModel.z;

	// 移動量に慣性をかける
	moveModel.x += (0.0f - moveModel.x) * RATE_MOVE_MODEL;
	moveModel.z += (0.0f - moveModel.z) * RATE_MOVE_MODEL;

#if 0
	// 範囲チェック
	if (g_posModel.x < -310.0f)
	{
		g_posModel.x = -310.0f;
	}
	if (g_posModel.x > 310.0f)
	{
		g_posModel.x = 310.0f;
	}
	if (g_posModel.z < -310.0f)
	{
		g_posModel.z = -310.0f;
	}
	if (g_posModel.z > 310.0f)
	{
		g_posModel.z = 310.0f;
	}
#endif

	if (KeyBoard::IsPress(DIK_RETURN))
	{// リセット
		posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	D3DXVECTOR3 pos = posModel;
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
	D3DXMatrixIdentity(&mtxWorldModel);

	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotModel.y, rotModel.x, rotModel.z);
	D3DXMatrixMultiply(&mtxWorldModel, &mtxWorldModel, &mtxRot);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, posModel.x, moveModel.y, posModel.z);
	D3DXMatrixMultiply(&mtxWorldModel, &mtxWorldModel, &mtxTranslate);


	//ワールドマトリックスの設定
	m_pDevice->SetTransform(D3DTS_WORLD, &mtxWorldModel);

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

D3DXVECTOR3 Player::GetPos()
{
	return posModel;
}
