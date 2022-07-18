#include "EnemyExplosion.h"
#include"../../Sprite.h"
#include"../../SRVDescMgr.h"
#include"Constants.h"
#include"EnemyExplosionEffect.h"
#include"GameOver.h"
#include"../../Model_HSM.h"
#include"../../Easing.h"

const int EnemyExplosion::FLASH_SPAN[STATUS_NUM] = { 0,60,40,30 };

EnemyExplosion::EnemyExplosion(DirectX::XMFLOAT3* EnemyPos)
{
	//alert.reset(new Sprite_Normal("EnemyAlert", SRVDescMgr::Instance()->CreateSRV("resource/img/enemyAlert.png"),
	//	MyDef::int2{ 128, 128 }, { 0,0 }, false, false, { 0.5f,0.5f }));

	enemyPos = EnemyPos;
}

void EnemyExplosion::Reset(const ENEMY_TYPE& Type, bool On)
{
	on = On;
	lifeSpan = ENEMY_LIFE_SPAN_STANDARD * ENEMY_LIFE_SPAN[Type];
	status = DEFAULT;
	timer = 0;
	flashTimer = 0;
	enemyType = Type;

	shakeRand = { 0.0f,0.0f };
}

void EnemyExplosion::Update()
{
	if (on && timer < lifeSpan)
	{
		timer++;
		if (timer == lifeSpan)
		{
			//printf("エネミー爆発\n");
			GameOver::Instance()->Launch(*enemyPos);
		}
	}

	if (status == DEFAULT)
	{
		if (lifeSpan - lifeSpan * 0.80f < timer)
		{
			status = ATTENTION;
		}
	}
	else if (status == ATTENTION)
	{
		if (lifeSpan - lifeSpan * 0.5f < timer)
		{
			status = WARNING;
		}
	}
	else if (status == WARNING)
	{
		if (lifeSpan - lifeSpan * 0.25f < timer)
		{
 			status = DANGER;
		}
	}

	//輪っか出てくる
	if (WARNING <= status)
	{
		if (flashTimer < FLASH_SPAN[status])
		{
			flashTimer++;
		}
		if (FLASH_SPAN[status] <= flashTimer)
		{
			flashTimer = 0;

			if (status == DANGER)
			{
				EnemyExplosionEffect::AlertParticleMgr::Instance()->ChangeColor(ENEMY_COLOR[enemyType]);
				EnemyExplosionEffect::AlertParticleMgr::Instance()->Emit(*enemyPos);
			}
		}
	}
}

void EnemyExplosion::AffectComponent(Model_HSM_Component& Component)
{
	const float RAND = 0.3f;
	//震える
	if (ATTENTION <= status)
	{
		shakeRand = { MyFunc::GetRand(-RAND, RAND) ,MyFunc::GetRand(-RAND, RAND) };

		Component.transform.pos.x += shakeRand.x;
		Component.transform.pos.y += shakeRand.y;
	}
	//点滅
	if (WARNING <= status)
	{
		Component.masterColor.flash = Easing::Ease(Out, Exp, flashTimer, FLASH_SPAN[status], 1.0f, 0.0f);
	}
}
