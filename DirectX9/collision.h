#pragma once

#include <d3dx9.h>
#include "shadow.h"

typedef struct
{
	float cx, cy, cz;	//���S���W
	float r;			//���a

}CIRCLE;

typedef struct
{
	float cx, cy, cz;	//	���S���W
	float sx, sy, sz;	//	�Z�`�T�C�Y

}AABB;

void Collision_Initialize(void);
void Collision_Update(void);
