//=============================================================================
//
// �e���� [bullet.h]
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Bullet_Initialize(void);
void Bullet_Finalize(void);
void Bullet_Update(void);
void Bullet_Draw(void);
void Bullet_Create(float x, float y, D3DXVECTOR2 dir);







