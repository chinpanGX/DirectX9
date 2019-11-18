/*======================================================

	[Camera.cpp]
	Author : 出合翔太

======================================================*/

#include "main.h"
#include "Camera.h"
#include "input.h"
#include "debugproc.h"
D3DXVECTOR3			Camera::posV;
D3DXVECTOR3			Camera::posR;
D3DXVECTOR3			Camera::vecU;
D3DXVECTOR3			Camera::rot;
float				Camera::fDistance;
D3DXMATRIX			Camera::mtxProjection;
D3DXMATRIX			Camera::mtxView;
LPDIRECT3DDEVICE9	Camera::pDevice;

//#	カメラの初期化
void Camera::Init()
{
	posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);	//カメラ位置
	posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);	//注視点
	vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						//カメラの上ベクトル
}

//#	カメラの終了処理
void Camera::Uninit()
{

}

//# カメラの更新処理
void Camera::Update()
{
	if (Input::KB_IsPress(DIK_A))
	{
		if (Input::KB_IsPress(DIK_W))
		{
			posV.x += sinf(-D3DX_PI * 0.75f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(-D3DX_PI * 0.75f - rot.y) * MOVE_CAMERA;
		}
		else if (Input::KB_IsPress(DIK_S))
		{
			posV.x += sinf(-D3DX_PI * 0.25f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(-D3DX_PI * 0.25f - rot.y) * MOVE_CAMERA;
		}
		else
		{
			posV.x += sinf(-D3DX_PI * 0.55f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(-D3DX_PI * 0.55f - rot.y) * MOVE_CAMERA;
		}
		posR.x = posV.x + sinf(rot.y) * fDistance;
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}
	else if (Input::KB_IsPress(DIK_D))
	{
		if (Input::KB_IsPress(DIK_W))
		{
			posV.x += sinf(-D3DX_PI * 0.75f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(-D3DX_PI * 0.75f - rot.y) * MOVE_CAMERA;
		}
		else if (Input::KB_IsPress(DIK_S))
		{
			posV.x += sinf(-D3DX_PI * 0.25f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(-D3DX_PI * 0.25f - rot.y) * MOVE_CAMERA;
		}
		else
		{
			posV.x += sinf(-D3DX_PI * 0.55f - rot.y) * MOVE_CAMERA;
			posV.z -= cosf(-D3DX_PI * 0.55f - rot.y) * MOVE_CAMERA;
		}
		posR.x = posV.x + sinf(rot.y) * fDistance;
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}
	else if (Input::KB_IsPress(DIK_W))
	{
		posV.x += sinf(-D3DX_PI * 1.0f - rot.y) * MOVE_CAMERA;
		posV.z -= cosf(-D3DX_PI * 1.0f - rot.y) * MOVE_CAMERA;

		posR.x = posV.x + sinf(rot.y) * fDistance;
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}
	else if (Input::KB_IsPress(DIK_S))
	{
		posV.x += sinf(-D3DX_PI * 0.0f - rot.y) * MOVE_CAMERA;
		{
			rot.y -= ROTATE_CAMERA;
			if (rot.y > D3DX_PI)
			{
				rot.y -= D3DX_PI * 2.0f;
			}
			posR.x = posV.x - sinf(rot.y) * fDistance;
			posR.z = posV.z - cosf(rot.y) * fDistance;
		}
		posV.z -= cosf(-D3DX_PI * 0.0f - rot.y) * MOVE_CAMERA;

		posR.x = posV.x + sinf(rot.y) * fDistance;
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}

	if (Input::KB_IsPress(DIK_Z))
	{
		rot.y += ROTATE_CAMERA;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		posR.x = posV.x - sinf(rot.y) * fDistance;
		posR.z = posV.z - cosf(rot.y) * fDistance;
	}
	if (Input::KB_IsPress(DIK_C))
	{
		rot.y -= MOVE_CAMERA;
		if (rot.y < D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		posR.x = posV.x - sinf(rot.y) * fDistance;
		posR.z = posV.z - cosf(rot.y) * fDistance;
	}

	if (Input::KB_IsPress(DIK_Y))
	{
		posV.y += MOVE_CAMERA;
	}
	if (Input::KB_IsPress(DIK_N))
	{
		posV.y -= MOVE_CAMERA;
	}

	if (Input::KB_IsPress(DIK_E))
	{
		rot.y += ROTATE_CAMERA;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		posR.x = posV.x - sinf(rot.y) * fDistance;
		posR.z = posV.z - cosf(rot.y) * fDistance;
	}
	if (Input::KB_IsPress(DIK_Q))
	{
		rot.y -= ROTATE_CAMERA;
		if (rot.y < D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		posR.x = posV.x + sinf(rot.y) * fDistance;
		posR.z = posV.z + cosf(rot.y) * fDistance;
	}

	if (Input::KB_IsPress(DIK_T))
	{
		posR.y += MOVE_CAMERA;
	}
	if (Input::KB_IsPress(DIK_B))
	{
		posR.y -= MOVE_CAMERA;
	}


	if (Input::KB_IsPress(DIK_U))
	{
		
	}
	if (Input::KB_IsPress(DIK_M))
	{
		
	}


	if (Input::KB_IsPress(DIK_SPACE))
	{
		posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);	//カメラ位置
		posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);	//注視点
		vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);							//カメラの上ベクトル

		float x, z;
		x = posV.x - posR.x;
		z = posV.z - posR.z;
		fDistance = sqrtf(x * x + z * z);
	}

	DebugProc_Print((char*)"カメラの視点 [%f : %f : %f]\n", posV.x, posV.y, posV.z);
	DebugProc_Print((char*)"カメラの注視点 [%f : %f : %f]\n", posR.x, posR.y, posR.z);
	DebugProc_Print((char*)"カメラの向き [%f]\n", rot.y);
	DebugProc_Print((char*)"\n");
}

//#	カメラの設定
void Camera::Set()
{
	//デバイスの取得
	pDevice = GetD3DDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,				// ビュー平面の視野角
		VIEW_ASPECT,			// ビュー平面のアスペクト比
		VIEW_NEAR_Z,			// ビュー平面のNearZ値
		VIEW_FAR_Z);			// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);	//	単位行列:行列の初期化するときに使う

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView,
		&posV,		// カメラの視点
		&posR,		// カメラの注視点
		&vecU);	// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
}
