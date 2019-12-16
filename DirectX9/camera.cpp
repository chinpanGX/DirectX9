
#include "camera.h"
#include "debugproc.h"
#include "Controller.h"

// マクロ定義
#define	CAM_POS_V_X		(0.0f)											//	カメラの視点初期位置(X座標)
#define	CAM_POS_V_Y		(200.0f)										//	カメラの視点初期位置(Y座標)
#define	CAM_POS_V_Z		(-300.0f)										//	カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X		(0.0f)											//	カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y		(0.0f)											//	カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z		(0.0f)											//	カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							//	ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	//	ビュー平面のアスペクト比
#define	VIEW_NEAR_Z		(10.0f)											//	ビュー平面のNearZ値
#define	VIEW_FAR_Z		(10000.0f)										//	ビュー平面のFarZ値
#define VALUE_MOVE_CAMERA (2.5f)										//	カメラの移動量
#define VALUE_ROTATE_CAMERA (D3DX_PI * 0.01f)							//	カメラの回転量
#define VALUE_ADUJST_DINSTANCE   (2.0f)									//	視点と注視点間の移動量

//	グローバル変数
Camera g_camera;		// カメラ情報

//	スタティック変数affa
LPDIRECT3DDEVICE9	Camera::pDevice;
D3DXMATRIX			Camera::mtxProjection;
D3DXMATRIX			Camera::mtxView;

//	カメラの初期化
void Camera::Init()
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

//	カメラの終了処理
void Camera::Uninit()
{
}

//	カメラの更新処理
void Camera::Update()
{

	if (KeyBoard::IsPress(DIK_A) || GamePad::IsPress(0,LEFTSTICK_LEFT))
	{
		//左移動
		g_camera.posV.x += sinf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(-D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (KeyBoard::IsPress(DIK_D) || GamePad::IsPress(0,LEFTSTICK_RIGHT))
	{
		//右移動
		g_camera.posV.x += sinf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(D3DX_PI * 0.50f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (KeyBoard::IsPress(DIK_W) || GamePad::IsPress(0, LEFTSTICK_UP))
	{
		//前移動
		g_camera.posV.x += sinf(-D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(-D3DX_PI * 1.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (KeyBoard::IsPress(DIK_S) || GamePad::IsPress(0, LEFTSTICK_DOWN))
	{
		//奥移動
		g_camera.posV.x += sinf(-D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;
		g_camera.posV.z -= cosf(-D3DX_PI * 0.0f - g_camera.rot.y) * VALUE_MOVE_CAMERA;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
#if 0
	if (KeyBoard::IsPress(DIK_Z))
	{//視点旋回「左」
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (KeyBoard::IsPress(DIK_C))
	{//視点旋回「右」
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y < D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	if (KeyBoard::IsPress(DIK_Y))
	{//視点移動「上」
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if (KeyBoard::IsPress(DIK_N))
	{//視点移動「下」
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}

	if (KeyBoard::IsPress(DIK_Q))
	{//注視点旋回「左」
		g_camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (KeyBoard::IsPress(DIK_E))
	{//注視点旋回「右」
		g_camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_camera.rot.y < D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (KeyBoard::IsPress(DIK_T))
	{//注視点移動「上」
		g_camera.posV.y += VALUE_MOVE_CAMERA;
	}
	if (KeyBoard::IsPress(DIK_B))
	{//注視点移動「下」
		g_camera.posV.y -= VALUE_MOVE_CAMERA;
	}
	if (KeyBoard::IsPress(DIK_U))
	{//視点と注視点の距離を縮める
		g_camera.fDistance -= VALUE_ADUJST_DINSTANCE;
		if (g_camera.fDistance < 300.0f)
		{
			g_camera.fDistance = 300.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	if (KeyBoard::IsPress(DIK_M))
	{//視点と注視点の距離を伸ばす
		g_camera.fDistance += VALUE_ADUJST_DINSTANCE;
		if (g_camera.fDistance > 900.0f)
		{
			g_camera.fDistance = 900.0f;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	if (KeyBoard::IsTrigger(DIK_RETURN))
	{//リセット
		g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
		g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_Z, CAM_POS_R_Z);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		float fVecX, fVecZ;
		fVecX = g_camera.posV.x - g_camera.posR.x;
		fVecZ = g_camera.posV.z - g_camera.posR.z;
		g_camera.fDistance = sqrtf(fVecX*fVecX + fVecZ * fVecZ);
	}
#endif
	//	文字列の描画
	DebugProc::Print((char*)"[カメラの視点:(%f:%f:%f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
	DebugProc::Print((char*)"[カメラの注視点:(%f:%f:%f)]\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
	DebugProc::Print((char*)"[カメラの向き:(%f)]\n", g_camera.rot.y);
	DebugProc::Print((char*)"[カメラの距離:(%f)]\n", g_camera.fDistance);
	DebugProc::Print((char*)"\n");
}   

void Camera::Set()
{
	pDevice = GetD3DDevice();

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

Camera* GetCamera()
{
	return &g_camera;
}
