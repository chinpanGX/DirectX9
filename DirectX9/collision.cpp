#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "shadow.h"
#include <d3dx9.h>


bool HitCircle(const CIRCLE* pCircle_a, const CIRCLE* pCircle_b);
bool HitAABB(const AABB* p_BB_a, const AABB* p_BB_b);
void Collision_Bullet_vs_Enemy(void);

//	�o�E���f�B���O�X�t�B�A�̓����蔻�� 
bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
	//��������x�N�g���^�̕ϐ������
	D3DXVECTOR3 dst1(p_circle_a->cx, p_circle_a->cy, p_circle_a->cz);
	D3DXVECTOR3 dst2(p_circle_b->cx, p_circle_b->cy, p_circle_b->cz);
	//��_�Ԃ̃x�N�g�������
	D3DXVECTOR3 distance = dst2 - dst1;
	//������x�N�g���̒��������߂�
	float length = D3DXVec3Length(&distance);
	//���݂��̔��a�𑫂����l�����߂�
	float size = p_circle_a->r + p_circle_b->r;

	//�x�N�g���̒����Ƃ��݂��̔��a�𑫂����l���r����
	//���x�N�g���̒����̕�����������΃q�b�g���Ă���I
	if (length < size){
		return true;
	}
	//�q�b�g���Ă��Ȃ�����
	return false;
}

/*
bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
//��������x�N�g���^�̕ϐ������
D3DXVECTOR3 dst1(p_circle_a->cx, p_circle_a->cy);
D3DXVECTOR3 dst2(p_circle_b->cx, p_circle_b->cy);
//��_�Ԃ̃x�N�g�������
D3DXVECTOR3 distance = dst2 - dst1;
//������x�N�g���̒��������߂�
float length = D3DXVec3LengthSq(&distance);
//���݂��̔��a�𑫂����l�����߂�
float size = (p_circle_a->r + p_circle_b->r) * (p_circle_a->r + p_circle_b->r);

//�x�N�g���̒����Ƃ��݂��̔��a�𑫂����l���r����
//���x�N�g���̒����̕�����������΃q�b�g���Ă���I
if (length < (p_circle_a->r + p_circle_b->r)){
return true;
}
//�q�b�g���Ă��Ȃ�����
return false;
}
*/

void Collision_Initialize(void)
{
}

void Collision_Update(void)
{
	Collision_Bullet_vs_Enemy();
}

void Collision_Bullet_vs_Enemy(void) 
{
#if 0
	Bullet* pBullet = new Bullet;
	pBullet->GetBullet();
	ENEMY* pEnemy = GetEnemy(); 
	Shadow* pShadow = new Shadow;
	pShadow->GetShadow();

	for (int i = 0; i < MAX_BULLET; i++)
	{
		// �e�͗L�����H
		if (pBullet->IsEnable(i) == false) {
			continue;
		}

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			// �G�l�~�[�͗L�����H
			if (Enemy_IsEnable(j) == false) {
				continue;
			}

			// �e�̃R���W�����ƃG�l�~�[�̃R���W����
			if (HitCircle(pBullet->GetCollision(i), Enemy_GetCollision(j)))
			{ 
				// �������Ă�

				// �G�̏��ŏ���
           		pShadow->Release(pEnemy[j].idxShadow);
				pEnemy[j].bUse = false;

				// �e�̏��ŏ���
				pShadow->Release(pBullet[i].m_nIdxShadowBullet);
				pBullet[i].m_bUse = false;

				// ���̒e�̏����͏I��
				break;
			}
		}
	}
#endif

	Bullet* pBullet;
	pBullet->GetBullet();
	Enemy* pEnemy;
	pEnemy->GetEnemy();
	Shadow* pShadow;
	pShadow->GetShadow();

	for (int i = 0; i < MAX_BULLET; i++)
	{
		// �e�͗L�����H
		if (pBullet->IsEnable(i) == false) {
			continue;
		}

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			// �G�l�~�[�͗L�����H
			if (pEnemy->IsEnable(j) == false) {
				continue;
			}

			// �e�̃R���W�����ƃG�l�~�[�̃R���W����
			if (HitAABB(pBullet->GetCollisionBox(i), pEnemy->GetAABBCollision(j)))
			{
				// �������Ă�

				// �G�̏��ŏ���
				pShadow->Release(pEnemy[j]->m_idxShadow);
				pEnemy[j]->m_bUse = false;

				// �e�̏��ŏ���
				pShadow->Release(pBullet[i].m_nIdxShadowBullet);
				pBullet[i].m_bUse = false;

				// ���̒e�̏����͏I��
				break;
			}
		}
	}
}

//	AABB�̓����蔻��
bool HitAABB(const AABB* p_BB_a, const AABB* p_BB_b)
{
	D3DXVECTOR3 minA, minB;	//	�ŏ��_
	D3DXVECTOR3 maxA, maxB;	//	�ő�_

	//	A��box�ŏ��_
	minA.x = p_BB_a->cx - p_BB_a->sx;
	minA.y = p_BB_a->cy - p_BB_a->sy;
	minA.z = p_BB_a->cz - p_BB_a->sz;

	//	A��box�ő�_
	maxA.x = p_BB_a->cx + p_BB_a->sx;
	maxA.y = p_BB_a->cy + p_BB_a->sy;
	maxA.z = p_BB_a->cz + p_BB_a->sz;

	//	B��box�ŏ��_
	minB.x = p_BB_b->cx - p_BB_b->sx;
	minB.y = p_BB_b->cy - p_BB_b->sy;
	minB.z = p_BB_b->cz - p_BB_b->sz;
	   			  
	//	B��box�ő�_ 
	maxB.x = p_BB_b->cx + p_BB_b->sx;
	maxB.y= p_BB_b->cy + p_BB_b->sy;
	maxB.z = p_BB_b->cz + p_BB_b->sz;

	//	X���̔�r
	if (maxA.x > minB.x && minA.x < maxB.x)
	{
		//	Y���̔�r
		if (maxA.y > minB.y && minA.y < maxB.y)
		{
			//	Z���̔�r
			if (maxA.z > minB.z && minA.z < maxB.z)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}