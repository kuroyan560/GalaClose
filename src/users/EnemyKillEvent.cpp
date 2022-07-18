#include "EnemyKillEvent.h"
#include"Constants.h"
#include"Enemy.h"
#include"Score.h"

void EnemyKillEvent::Init()
{
	killTimer = ENEMY_KILL_SPAN;
	waitTimer = ENEMY_ALL_KILL_SPAN;
	killedEnemys.clear();
	killEnemyNum = 0;
}

void EnemyKillEvent::Update()
{
	if (waitTimer < ENEMY_ALL_KILL_SPAN)
	{
		waitTimer++;
	}

	//Ž€–S‰‰oŽžŠÔŒv‘ª
	if (killTimer < ENEMY_KILL_SPAN)
	{
		killTimer++;
	}
	else
	{
		//Ž€–S‰‰o‚É“ü‚é
		if (!killedEnemys.empty())
		{
			//ˆê‘Ì‚¸‚ÂŽ€–S‰‰o
			killedEnemys.back()->Dead();
			killedEnemys.pop_back();
			killTimer = 0;
			Score::Instance()->Add();
			killEnemyNum++;

			if(killedEnemys.empty())	waitTimer = 0;
		}
	}
}

bool EnemyKillEvent::NowEvent()
{
	return !killedEnemys.empty() || waitTimer < ENEMY_ALL_KILL_SPAN;
}