#pragma once

#include <d3dx9.h>
#include "shadow.h"

typedef struct
{
	float cx, cy, cz;	//中心座標
	float r;			//半径

}CIRCLE;

typedef struct
{
	float cx, cy, cz;	//	中心座標
	float sx, sy, sz;	//	短形サイズ

}AABB;

void Collision_Initialize(void);
void Collision_Update(void);
