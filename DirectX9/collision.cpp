#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "shadow.h"
#include <d3dx9.h>


bool HitCircle(const CIRCLE* pCircle_a, const CIRCLE* pCircle_b);
bool HitAABB(const AABB* p_BB_a, const AABB* p_BB_b);
void Collision_Bullet_vs_Enemy(void);

//	バウンディングスフィアの当たり判定 
bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
	//引数からベクトル型の変数を作る
	D3DXVECTOR3 dst1(p_circle_a->cx, p_circle_a->cy, p_circle_a->cz);
	D3DXVECTOR3 dst2(p_circle_b->cx, p_circle_b->cy, p_circle_b->cz);
	//二点間のベクトルを作る
	D3DXVECTOR3 distance = dst2 - dst1;
	//作ったベクトルの長さを求める
	float length = D3DXVec3Length(&distance);
	//お互いの半径を足した値を求める
	float size = p_circle_a->r + p_circle_b->r;

	//ベクトルの長さとお互いの半径を足した値を比較する
	//→ベクトルの長さの方が小さければヒットしている！
	if (length < size){
		return true;
	}
	//ヒットしていなかった
	return false;
}

/*
bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
//引数からベクトル型の変数を作る
D3DXVECTOR3 dst1(p_circle_a->cx, p_circle_a->cy);
D3DXVECTOR3 dst2(p_circle_b->cx, p_circle_b->cy);
//二点間のベクトルを作る
D3DXVECTOR3 distance = dst2 - dst1;
//作ったベクトルの長さを求める
float length = D3DXVec3LengthSq(&distance);
//お互いの半径を足した値を求める
float size = (p_circle_a->r + p_circle_b->r) * (p_circle_a->r + p_circle_b->r);

//ベクトルの長さとお互いの半径を足した値を比較する
//→ベクトルの長さの方が小さければヒットしている！
if (length < (p_circle_a->r + p_circle_b->r)){
return true;
}
//ヒットしていなかった
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
		// 弾は有効か？
		if (pBullet->IsEnable(i) == false) {
			continue;
		}

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			// エネミーは有効か？
			if (Enemy_IsEnable(j) == false) {
				continue;
			}

			// 弾のコリジョンとエネミーのコリジョン
			if (HitCircle(pBullet->GetCollision(i), Enemy_GetCollision(j)))
			{ 
				// 当たってる

				// 敵の消滅処理
           		pShadow->Release(pEnemy[j].idxShadow);
				pEnemy[j].bUse = false;

				// 弾の消滅処理
				pShadow->Release(pBullet[i].m_nIdxShadowBullet);
				pBullet[i].m_bUse = false;

				// この弾の処理は終了
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
		// 弾は有効か？
		if (pBullet->IsEnable(i) == false) {
			continue;
		}

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			// エネミーは有効か？
			if (pEnemy->IsEnable(j) == false) {
				continue;
			}

			// 弾のコリジョンとエネミーのコリジョン
			if (HitAABB(pBullet->GetCollisionBox(i), pEnemy->GetAABBCollision(j)))
			{
				// 当たってる

				// 敵の消滅処理
				pShadow->Release(pEnemy[j]->m_idxShadow);
				pEnemy[j]->m_bUse = false;

				// 弾の消滅処理
				pShadow->Release(pBullet[i].m_nIdxShadowBullet);
				pBullet[i].m_bUse = false;

				// この弾の処理は終了
				break;
			}
		}
	}
}

//	AABBの当たり判定
bool HitAABB(const AABB* p_BB_a, const AABB* p_BB_b)
{
	D3DXVECTOR3 minA, minB;	//	最小点
	D3DXVECTOR3 maxA, maxB;	//	最大点

	//	Aのbox最小点
	minA.x = p_BB_a->cx - p_BB_a->sx;
	minA.y = p_BB_a->cy - p_BB_a->sy;
	minA.z = p_BB_a->cz - p_BB_a->sz;

	//	Aのbox最大点
	maxA.x = p_BB_a->cx + p_BB_a->sx;
	maxA.y = p_BB_a->cy + p_BB_a->sy;
	maxA.z = p_BB_a->cz + p_BB_a->sz;

	//	Bのbox最小点
	minB.x = p_BB_b->cx - p_BB_b->sx;
	minB.y = p_BB_b->cy - p_BB_b->sy;
	minB.z = p_BB_b->cz - p_BB_b->sz;
	   			  
	//	Bのbox最大点 
	maxB.x = p_BB_b->cx + p_BB_b->sx;
	maxB.y= p_BB_b->cy + p_BB_b->sy;
	maxB.z = p_BB_b->cz + p_BB_b->sz;

	//	X軸の比較
	if (maxA.x > minB.x && minA.x < maxB.x)
	{
		//	Y軸の比較
		if (maxA.y > minB.y && minA.y < maxB.y)
		{
			//	Z軸の比較
			if (maxA.z > minB.z && minA.z < maxB.z)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}