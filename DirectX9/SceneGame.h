/*===================================================

	[Scene.h]
	Author : 出合翔太

===================================================*/

#pragma once
#include "SceneBase.h"
#include "camera.h"
#include "debugproc.h"
#include "field.h"
#include "light.h"
#include "player.h"
#include "texture.h"
#include "box.h"
#include "Grid.h"

class SceneGame : public SceneBase
{
private:
	Camera	m_Camera;	//	Camera型インスタンス
	Player	m_Player;	//	Player型インスタンス
	Field	m_Field;	//	Field型インスタンス
	Grid	m_Grid;		//	Grig型インスタンス
	Box		m_Box;		//	Boxがtインスタンス

public:
	void Init();		//	初期化処理
	void Uninit();		//	終了処理
	void Update();		//	更新処理
	void Draw();		//	描画処理
};

