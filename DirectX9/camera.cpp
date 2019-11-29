
#include "camera.h"
#include "myDirect3D.h"
#include "debugproc.h"
#include "input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAM_POS_V_X		(0.0f)					// カメラの視点初期位置(X座標)
#define	CAM_POS_V_Y		(100.0f)				// カメラの視点初期位置(Y座標)
#define	CAM_POS_V_Z		(-500.0f)				// カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X		(0.0f)					// カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y		(0.0f)					// カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z		(0.0f)					// カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(1000.0f)										// ビュー平面のFarZ値
#define VALUE_MOVE_CAMERA (1.0f)
#define VALUE_ROTATE_CAMERA (D3DX_PI * 0.01f)
#define VALUE_ADUJST_DINSTANCE   (2.0f)
//*****************************************************************************
// カメラの構造体
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
CAMERA g_camera;		// カメラ情報

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// カメラの初期化処理
//=============================================================================
void Camera_Initialize(void)
{
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_camera.fDistance = 2.0f;
	float fVecX, fVecZ;
	fVecX = g_camera.posV.x - g_camera.posR.x;
	fVecZ = g_camera.posV.z - g_camera.posR.z;
	g_camera.fDistance = sqrtf(fVecX*fVecX + fVecZ * fVecZ);
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void Camera_Finalize(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void Camera_Update(void)
{
	if (Keyboard_IsPress(DIK_A)) 
	{
		if (Keyboard_IsPress(DIK_W))
		{//左奥移動
			g_camera.posV.x += sinf(-D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(-D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else if (Keyboard_IsPress(DIK_S))
		{//左手前移動
			g_camera.posV.x += sinf(-D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(-D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else
		{//左移動
			g_camera.posV.x += sinf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (Keyboard_IsPress(DIK_D))
	{
		if (Keyboard_IsPress(DIK_W))
		{//右奥移動
			g_camera.posV.x += sinf(D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(D3DX_PI * 0.75f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else if (Keyboard_IsPress(DIK_S))
		{//右手前移動
			g_camera.posV.x += sinf(D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(D3DX_PI * 0.25f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		else
		{//右移動
			g_camera.posV.x += sinf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
			g_camera.posV.z -= cosf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (Keyboard_IsPress(DIK_W))
	{//前移動
		g_camera.posV.x += sinf(-D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(-D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (Keyboard_IsPress(DIK_S))
	{//奥移動
		g_camera.posV.x += sinf(-D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(-D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_Z))
	{//視点旋回「左」
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_C))
	{//視点旋回「右」
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y < D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_Y))
	{//視点移動「上」
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_N))
	{//視点移動「下」
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_Q))
	{//注視点旋回「左」
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_E))
	{//注視点旋回「右」
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y < D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_T))
	{//注視点移動「上」
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_B))
	{//注視点移動「下」
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}
	if (Keyboard_IsPress(DIK_U))
	{//視点と注視点の距離を縮める
		g_camera.fDistance -= VALUE_ADUJST_DINSTANCE;
		if (g_camera.fDistance < 300.0f)
		{
			g_camera.fDistance = 300.0f;
		}
		g_camera.posV.x = g_camera.posR.x- sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z- cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsPress(DIK_M))
	{//視点と注視点の距離を伸ばす
		g_camera.fDistance += VALUE_ADUJST_DINSTANCE;
		if (g_camera.fDistance > 900.0f)
		{
			g_camera.fDistance = 900.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z- cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (Keyboard_IsTrigger(DIK_SPACE))
	{//リセット
		g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
		g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_Z, CAM_POS_R_Z);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		float fVecX, fVecZ;
		fVecX = g_camera.posV.x - g_camera.posR.x;
		fVecZ = g_camera.posV.z - g_camera.posR.z;
		g_camera.fDistance = sqrtf(fVecX*fVecX + fVecZ * fVecZ);
	}
	DebugProc_Print((char*)"[カメラの視点:(%f:%f:%f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
	DebugProc_Print((char*)"[カメラの注視点:(%f:%f:%f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
	DebugProc_Print((char*)"[カメラの向き:(%f)]\n", g_camera.rot.y);
	DebugProc_Print((char*)"[カメラの距離:(%f)]\n", g_camera.fDistance);
	DebugProc_Print((char*)"\n");
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void Camera_SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,				// ビュー平面の視野角
								VIEW_ASPECT,			// ビュー平面のアスペクト比
								VIEW_NEAR_Z,			// ビュー平面のNearZ値
								VIEW_FAR_Z);			// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// カメラの視点
						&g_camera.posR,		// カメラの注視点
						&g_camera.vecU);	// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

CAMERA* GetCamera()
{
	return &g_camera;
}