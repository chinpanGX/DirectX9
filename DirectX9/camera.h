/*-------------------------------------------------------

	[camera.h]
	Author : 出合翔太　

--------------------------------------------------------*/
#pragma once

#include "main.h"

//	Cameraクラス
class Camera
{
private:
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 vecU;			// 上方向ベクトル
	float fDistance;
	static D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	static LPDIRECT3DDEVICE9 pDevice;
public:
	static D3DXMATRIX mtxView;			// ビューマトリックス
	D3DXVECTOR3 rot;
public:
	void Init();	//	初期化処理
	void Uninit();	//	終了処理
	void Update();	//	更新処理			
	void Set();		//	描画処理

	//Camera* GetCamera();	//ゲッター
};

Camera* GetCamera();	//ゲッター