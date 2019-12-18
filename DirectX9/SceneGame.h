/*===================================================

	[SceneGame.h]
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
#include "billboard.h"
#include "shadow.h"
#include "bullet.h"

class SceneGame : public SceneBase
{
private:
	Camera			m_Camera;
	Player			m_Player;
	Field			m_Field;
	Bullet			m_Bullet;
	Shadow			m_Shadow;
	Enemy			m_Enemy;

public:
	void Init();		//	初期化処理
	void Uninit();		//	終了処理
	void Update();		//	更新処理
	void Draw();		//	描画処理
};

