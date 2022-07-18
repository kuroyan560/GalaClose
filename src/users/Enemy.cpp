#include "Enemy.h"
#include <math.h>
#include"Layer.h"
#include"../../MyFunc.h"
#include"PanelMgr.h"
#include"Constants.h"
#include"EnemyGenerateHelper.h"

#include"../../DebugText.h"
#include "QueenMgr.h"
#include "../../MyFunc.h"

#include"../../AudioMgr.h"

#include"EnemyKillEvent.h"
#include"../../EffectMgr.h"
#include"EnemyBody.h"
#include"../../Easing.h"
/*const float Enemy::baseSpeed = 3.14f / 180;
const float Enemy::round = baseSpeed * 360;

const float Enemy::offsetLength = 100*/;
#include"GameOver.h"

int Enemy::ENEMY_KILL_SE = 0;

//スポーン演出の時間
const int SPAWN_EFFECT_TOTAL_TIME = 30;

void Enemy::LoadAudios()
{
	ENEMY_KILL_SE = AudioMgr::Instance()->LoadAudio("resource/sound/enemyKillSE.wav");
	AudioMgr::Instance()->ChangeVolume(ENEMY_KILL_SE, 0.7f);
}
Enemy::Enemy()
	:enemyExplosion(&component.transform.pos)
{
	isClockWise = false;
	posAngle = 0;
	//panelIndex = 0;
	//layer = 0;
	layerNum = 0;
	isAlive = false;
	//type = 0;
	moveSpeed = 0;
}

void Enemy::UniqueUpdate()
{
	if (!panel.lock()->NowPushed())
	{
		CheckQueen();
	}
}

void Enemy::Init()
{
	isAlive = false;
	bodyEmitTimer = ENEMY_BODY_EMIT_SPAN;
	//type = panelType;
	UniqueInit();
	spawnTimer = SPAWN_EFFECT_TOTAL_TIME;
}

void Enemy::Update()
{
	//死んでいたら関数を抜ける
	if (!isAlive)return;

	if (spawnTimer < SPAWN_EFFECT_TOTAL_TIME)
	{
		spawnTimer++;
		component.transform.scale = Easing::Ease(Out, Back, 
			spawnTimer, SPAWN_EFFECT_TOTAL_TIME, { 0.0f,0.0f,0.0f }, { defaultScale,defaultScale,defaultScale });
	}

	float lengthOffset = 0.0f;

	//押し込み中は回転はしない
	if (panel.lock()->NowPushed())
	{
		float rate = 0.0f;
		rate = panel.lock()->GetPushedRate();
		/*	if (panel.lock()->pushedLayer <= 1 || PanelMgr::Instance()->GetMaxLayerNum() == panel.lock()->pushedLayer + PanelMgr::Instance()->GetMaxLayerNum() - 1)
			{
				rate = panel.lock()->GetPushedRate();
			}
			else
			{
				rate = panel.lock()->GetPushedRate() - 1.0f;
			}*/
		lengthOffset = -LAYER_OFFSET_RADIUS * rate;
		if (lengthOffset < -LAYER_OFFSET_RADIUS * (layerNum + 1))
		{
			lengthOffset = -LAYER_OFFSET_RADIUS * (layerNum + 1);
		}
	}
	else if (panel.lock()->GetOffsetLayerNumByAddLayer())
	{
		lengthOffset = LAYER_OFFSET_RADIUS * (panel.lock()->GetOffsetLayerNumByAddLayer() + panel.lock()->GetPushedRate());
	}
	else
	{
		if (GetEnemyType() == JACK && !GameOver::Instance()->Flag())
		{
			float AddSpeed = moveSpeed / (layerNum + 1);
			if (layerNum == -1)AddSpeed = moveSpeed;

			if (isClockWise)
			{
				posAngle += AddSpeed;
			}
			else
			{
				posAngle -= AddSpeed;
			}

			if (posAngle > PI * 2)
			{
				posAngle -= PI * 2;
			}
			if (posAngle < 0)
			{
				posAngle += PI * 2;
			}
		}
		enemyExplosion.Update();
	}

	//一旦中心の座標に
	XMFLOAT3 oldPos = component.transform.pos;
	oldPos.x -= enemyExplosion.GetShakeRand().x;
	oldPos.y -= enemyExplosion.GetShakeRand().y;
	component.transform.pos = { 0,0,0 };
	//drawPos = { 0,0,0 };
	//中心からどれだけ動いたか
	XMFLOAT3 move;

	move = { cosf(posAngle), -sinf(posAngle), 0 };


	//move *= (offsetLength * (layer + 1));
	float length = INNER_SPACE_RADIUS + ENEMY_OFFSET_LENGTH * layerNum + ENEMY_OFFSET_LENGTH / 2.0f;
	length += lengthOffset;	//押し込みによるずれを反映させる
	move = MyFunc::MulXMFLOAT3(move, length);

	//drawPos = MyFunc::AddXMFLOAT3(drawPos, move);
	component.transform.pos = MyFunc::AddXMFLOAT3(component.transform.pos, move);

	if (type == JACK && !GameOver::Instance()->Flag())
	{
		component.transform.up = MyFunc::NormalizeXMFLOAT3(MyFunc::SubXMFLOAT3(oldPos, component.transform.pos));
	}

	Collision();

	if (panel.lock()->PushedTriggerRelative())
	{
		Down(panel.lock()->PushedTriggerRelative());
	}
	if (panel.lock()->AddLayerTrigger())
	{
		layerNum += panel.lock()->GetOffsetLayerNumByAddLayer();
	}

	if (bodyEmitTimer)
	{
		bodyEmitTimer--;
		if (bodyEmitTimer == 0)
		{
			bodyEmitTimer = ENEMY_BODY_EMIT_SPAN;
			if (bodyEmitFlg)
			{
				EnemyBody::PrimitiveParticleMgr::Instance()->ChangeColor(ENEMY_COLOR[type]);
				EnemyBody::PrimitiveParticleMgr::Instance()->Emit(component.transform.pos);
			}
		}
	}

	enemyExplosion.AffectComponent(this->component);

	UniqueUpdate();
}

void Enemy::Finalize()
{
	UniqueFinalize();
}

void Enemy::Draw()
{
	//敵の描画はEnemyMgrで一括で行われる
	//if(isAlive)
	//DebugText::Instance()->FormatPrint(0, 16 * 1.5f * 5, 1.5f, "layerNum %d", (int)layerNum);
}

void Enemy::Down(const int &PushedNum)
{
	layerNum -= PushedNum;
	if (layerNum < 0)
	{
		//Dead();
		EnemyKillEvent::Instance()->PushKilledEnemy(this);
	}
}

void Enemy::Spawn(const EnemyGenerateHelper &InitData)
{
	type = InitData.type;
	component.transform.scale = { 0.0f,0.0f,0.0f };
	if (type == JACK)
	{
		defaultScale = 0.65f;
	}
	else if (type == KING)
	{
		defaultScale = 0.65f;
	}
	else if(type == PORN)
	{
		defaultScale = 0.2f;
	}
	spawnTimer = 0;
	enemyExplosion.Reset(type);

	component.DirtyRegister();
	component.DirtyRecord();
	isAlive = true;
	this->isClockWise = InitData.isClockWise;
	this->posAngle = InitData.posAngle;
	this->panel = PanelMgr::Instance()->GetPanelByAngle(InitData.posAngle, InitData.isClockWise);
	this->layerNum = InitData.layerNum - panel.lock()->GetOffsetLayerNumByAddLayer();

	moveSpeed = ENEMY_BASE_SPEED_STANDARD * ENEMY_BASE_SPEED[type] * InitData.speedRate;

	bodyEmitFlg = true;
	beDrained = false;
	component.masterColor.color.w = 1.0f;
	component.masterColor.flash = 0.0f;
}

void Enemy::Dead()
{
	component.DirtyRemove();
	isAlive = false;
	AudioMgr::Instance()->PlayWave(ENEMY_KILL_SE);
	EffectMgr::Instance()->EnemyDeadEffect(ENEMY_COLOR[type], component.transform.pos);
}

void Enemy::Collision()
{
	if (IsOutRange(panel.lock()->GetStartAngle(), panel.lock()->GetEndAngle()))
	{
		Reverce();
	}
}

char Enemy::GetType()
{
	return panel.lock()->GetLayerColor(layerNum);
}

int Enemy::GetPanelIndex()
{
	//return panelIndex;
	return panel.lock()->GetPanelIndex();
}

const int &Enemy::Getlayer()
{
	return layerNum;
}

const float &Enemy::GetAngle()
{
	return posAngle;
}


void Enemy::SetLayer(int LayerNum)
{
	layerNum = LayerNum;
}

//void Enemy::SetType(char num)
//{
//	type = num;
//}

bool Enemy::IsOutRange(float startAngle, float EndAngle)
{
	//指定の範囲内にいるなら
	if ((posAngle >= startAngle) && (posAngle <= EndAngle))
	{//falseを変えす
		return false;
	}
	//そうでないならtrueを返す
	return true;
}

void Enemy::Reverce()
{
	char nowColor = panel.lock()->GetLayerColor(layerNum);
	auto nextPanel = PanelMgr::Instance()->GetPanelByIndex(panel.lock()->GetPanelIndex() + (isClockWise ? 1 : -1));
	char nextColor = nextPanel.get()->GetLayerColor(layerNum);

	//属性の違うパネルに接触したら、もしくは移動先のパネルに入れないなら
	//if (type != nextPanelType)
	if (nowColor != nextColor || !nextPanel->CanEnter())
	{
		if (isClockWise)
		{
			posAngle = panel.lock()->GetEndAngle();
			if(GetEnemyType() == JACK)isClockWise = false;
		}
		else
		{
			posAngle = panel.lock()->GetStartAngle();
			if(GetEnemyType() == JACK)isClockWise = true;
		}
	}
	else
	{//同じ属性だった場合
		panel = nextPanel;

		//if (isClockWise)
		//{
		//	panel = PanelMgr::Instance()->GetPanel(panel.lock()->GetPanelIndex() + 1);
		//	//panelIndex++;
		//}
		//else
		//{
		//	panel = PanelMgr::Instance()->GetPanel(panel.lock()->GetPanelIndex() - 1);
		//	//panelIndex;
		//}

		//if (panelIndex > maxCountIndex)
		//{
		//	panelIndex = 0;
		//}
		//else if(panelIndex < 0)
		//{
		//	panelIndex = maxCountIndex;
		//}
	}
}

void Enemy::panelVisit(const int queenNum)
{
	std::vector<std::weak_ptr<Queen>> queens = QueenMgr::Instance()->QueenDataSend();
	std::weak_ptr<Queen> queen = queens[queenNum];
	//auto SetPanel = PanelMgr::Instance()->GetPanelByIndex(panelNum);
	////パネルの始点と終点をもらう
	//float startAngle = SetPanel.get()->GetStartAngle(),
	//	endAngle = SetPanel.get()->GetStartAngle(),
	//始点と終点のどちらが大きいかで向きを変える
	const int startPunelIndex = panel.lock()->GetPanelIndex();
	int endPanelIndex;
	for (int i = 0; i < queen.lock()->GetmovablePanels().size(); i++)
	{
		if (queen.lock()->GetmovablePanels()[i].panelNum == startPunelIndex)
		{
			endPanelIndex = i;
			break;
		}
	}
	float sAngle = queen.lock()->GetAngle(), eAngle = queen.lock()->GetAngle(), SPAngle, EPAngle;
	if (posAngle < queen.lock()->GetAngle())
	{
		eAngle = queen.lock()->GetAngle() - (2 * PI);
	}

	if (posAngle > queen.lock()->GetAngle())
	{
		sAngle = queen.lock()->GetAngle() + (2 * PI);
	}

	//始点の反対側をとる
	SPAngle = sAngle - posAngle;
	EPAngle = posAngle - eAngle;

	//クイーンに吸われたか
	bool queenDrained = false;

	if (!queen.lock()->GetmovablePanels()[endPanelIndex].ClockWise)
	{//反時計回り出来なかったら

			posAngle = MyFunc::Lerp(posAngle, sAngle, 0.05f);
			if (abs(posAngle - sAngle) < 0.1f)queenDrained = true;
			isClockWise = true;
	}
	else if (!queen.lock()->GetmovablePanels()[endPanelIndex].CounterClockWise)
	{//時計回り出来なかったら

			posAngle = MyFunc::Lerp(posAngle, eAngle, 0.05f);
			if (abs(posAngle - eAngle) < 0.1f)queenDrained = true;
			isClockWise = false;
	}
	else if (queen.lock()->GetmovablePanels()[endPanelIndex].ClockWise 
		&& queen.lock()->GetmovablePanels()[endPanelIndex].CounterClockWise)
	{//両方できたら
		if (SPAngle < EPAngle)
		{
			posAngle = MyFunc::Lerp(posAngle, sAngle, 0.05f);
			if (abs(posAngle - sAngle) < 0.1f)queenDrained = true;
			isClockWise = true;
		}
		else
		{
			posAngle = MyFunc::Lerp(posAngle, eAngle, 0.05f);
			if (abs(posAngle - eAngle) < 0.1f)queenDrained = true;
			isClockWise = false;
		}
	}

	if (queenDrained && !beDrained)
	{
		bodyEmitFlg = false;
		beDrained = true;
		component.masterColor.color.w = 0.0f;
		queen.lock()->AddDrainEnemy();
		enemyExplosion.Reset(type, false);	//爆誕解除
	}

	if (posAngle >= (PI * 2))
	{
		posAngle -= (PI * 2);
	}

	if (posAngle < 0)
	{
		posAngle += (PI * 2);
	}
}

void Enemy::CheckQueen()
{
	std::vector<std::weak_ptr<Queen>> queens = QueenMgr::Instance()->QueenDataSend();

	if (queens.empty())return;
	//適当に大きい値
	float minLength = 10000.0f;
	float nowLength = 0;

	const int ind = panel.lock()->GetPanelIndex();
	const int num = Getlayer();

	int queenNum = 0;
	bool queenFlag = false;
	for (int i = 0; i < queens.size(); i++)
	{
		for (int j = 0; j < queens[i].lock()->GetmovablePanels().size(); j++)
		{

			if ((num == queens[i].lock()->GetmovablePanels()[j].layerNum) 
				&& (ind == queens[i].lock()->GetmovablePanels()[j].panelNum))
			{
				nowLength = angleLength(posAngle, queens[i].lock()->GetAngle());
				if (minLength > nowLength)
				{
					//どのクイーンが近いか記憶する
					queenNum = i;
					queenFlag = true;
					minLength = nowLength;
				}
			}
		}
	}
	if (queenFlag)
	{
		//auto panel = PanelMgr::Instance()->GetPanelByAngle(queens[queenNum].posAngle, true);
		panelVisit(queenNum);
	}
}
float Enemy::angleLength(float a, float b)
{
	float sAngle = b, eAngle = b, SPAngle, EPAngle;
	if (a < b)
	{
		eAngle = b - (2 * PI);
	}

	if (a > b)
	{
		sAngle = b + (2 * PI);
	}

	//始点の反対側をとる
	SPAngle = sAngle - posAngle;
	EPAngle = posAngle - eAngle;

	if (SPAngle < EPAngle)
	{
		return SPAngle;
	}

	return EPAngle;
}
