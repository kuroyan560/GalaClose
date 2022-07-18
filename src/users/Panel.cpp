#include "Panel.h"
#include"../../Easing.h"
#include"Constants.h"
#include"../../MyFunc.h"
#include"EnemyMgr.h"
#include"../../GameCamera.h"
#include"../../AudioMgr.h"
#include"EnemyKillEvent.h"
#include"Score.h"
#include"GameOver.h"

int Panel::PUSH_SE = 0;
int Panel::PUSH_RESET_SE = 0;

void Panel::LoadAudios()
{
	PUSH_SE = AudioMgr::Instance()->LoadAudio("resource/sound/pushSE.wav");
	PUSH_RESET_SE = AudioMgr::Instance()->LoadAudio("resource/sound/pushResetSE.wav");
}

void Panel::OneLayerPush()
{
	if (pushedLayer == layers.size() || pushedTimer != PUSH_TOTAL_TIME)return;

	oldPushedLayer = pushedLayer;
	pushedLayer++;

	if (layers.size() <= pushedLayer)
	{
		pushedLayer = layers.size();		//反対側へのめり込み防止
		resetTimer = ALL_PUSHED_UNTIL_RESET_TOTAL_TIME;
	}

	pushedTimer = 0;
	pushed = true;

	AudioMgr::Instance()->PlayWave(PUSH_SE);

	//パネルの内側に敵がいる
	//if (2 <= EnemyMgr::Instance()->ExistEnemy(panelIndex, pushedLayer - oldPushedLayer - 1))
	//{
	//	GameCamera::Instance()->EnemyKillZoom(panelIndex);
	//}
}

Panel::Panel(const float& StartAngle, const float& EndAngle, const int& LayerNum, const int& PanelIndex, const bool& ColorDifference)
	:startAngle(StartAngle), endAngle(EndAngle), pushedTimer(PUSH_TOTAL_TIME), panelIndex(PanelIndex)
{
	for (int i = 1; i < LayerNum + 1; ++i)
	{
		 unsigned int color = (PanelIndex % 2 == 0) ? (i % 2 == 0 ? 0 : 1) : (i % 2 != 0 ? 0 : 1);
		 if (!ColorDifference)color = 0;
		 layers.emplace_back(Layer(StartAngle, EndAngle, i, color, ColorDifference));
	}
}

void Panel::Update()
{
	float oldPushRate = pushedRate;

	if (!GameOver::Instance()->Flag())
	{
		if (pushBuffSubTimer)pushBuffSubTimer--;

		if (pushBuff && !pushed && !pushBuffSubTimer && !EnemyKillEvent::Instance()->NowEvent())
		{
			OneLayerPush();
			pushBuff--;
		}

		if (pushedTimer < PUSH_TOTAL_TIME)
		{
			pushedTimer++;
			//pushedRate = Easing::Ease(Out, Elastic, pushedTimer, PUSH_TOTAL_TIME, (float)oldPushedLayer, (float)pushedLayer);
			pushedRate = Easing::Linear(pushedTimer, PUSH_TOTAL_TIME, (float)oldPushedLayer, (float)pushedLayer);
			if (pushedTimer == PUSH_TOTAL_TIME)
			{
				pushed = false;
				offsetLayerNumByAddLayer = 0;

				//if (pushedLayer != 0)	//押し込みリセット時はカメラシェイクしない
				//{
				GameCamera::Instance()->CamShake();
				//}

				if (pushBuff)
				{
					pushBuffSubTimer = PUSH_LAYER_OFFSET_TOTAL_TIME;
				}
				oldPushedLayer = pushedLayer;
			}
		}
		else
		{
			if (resetTimer && !EnemyKillEvent::Instance()->NowEvent())	//全押し込みからリセットまでの時間
			{
				resetTimer--;
				if (resetTimer == 0)
				{
					PushReset();
				}
			}
		}
	}
	else
	{
		GameOver::Instance()->AffectPushedRate(oldPushedLayer, panelIndex, &pushedRate);
	}

	if (oldPushRate != pushedRate);
	{
		for (auto layer : layers)
		{
			layer.Update(pushedRate);
		}
	}
}

void Panel::Draw()
{
	for (auto layer : layers)
	{
		layer.Draw();
	}
}

//void Panel::Pushed(const int& LayerNum)
//{
//	if (pushedLayer == layers.size() || pushedTimer != PUSH_TOTAL_TIME)return;
//
//	oldPushedLayer = pushedLayer;
//	pushedLayer += LayerNum;
//
//	if (layers.size() <= pushedLayer)
//	{
//		pushedLayer = layers.size();		//反対側へのめり込み防止
//		resetTimer = ALL_PUSHED_UNTIL_RESET_TOTAL_TIME;
//	}
//
//	pushedTimer = 0;
//	pushed = true;
//
//	//パネルの内側に敵がいる
//	if (EnemyMgr::Instance()->ExistEnemy(panelIndex, pushedLayer - oldPushedLayer - 1))
//	{
//		GameCamera::Instance()->EnemyKillZoom(panelIndex);
//	}
//}

void Panel::Pushed(const int& LayerNum)
{
	Score::Instance()->ComboReset();
	pushBuff = LayerNum;
}

void Panel::PushReset()
{
	oldPushedLayer = pushedLayer;
	pushedLayer = 0;
	pushedTimer = 0;
	AudioMgr::Instance()->PlayWave(PUSH_RESET_SE);
	//GameCamera::Instance()->ReturnToDefault();
}

void Panel::AddLayer()
{
	unsigned int color = layers.back().GetColor() ? 0 : 1;
	layers.emplace_back(Layer(startAngle, endAngle, layers.size() + 1, color, pushedRate));

	if (pushedLayer)
	{
		offsetLayerNumByAddLayer = pushedLayer;
		PushReset();
	}
	else
	{
		offsetLayerNumByAddLayer = 0;
		oldPushedLayer = 1;
		pushedLayer = 0;
		pushedTimer = 0;
	}
}

void Panel::RegisterLayerBeforeExplosion()
{
	oldPushedLayer = pushedLayer;
	for (auto& layer : layers)
	{
		layer.SetExplosionOffset();
	}
}

const char& Panel::GetLayerColor(const int& LayerIndex)
{
	if (layers.size() <= LayerIndex + pushedLayer)	//押し込まれているためレイヤーが存在しない
	{
		return -1;
	}
	// TODO: return ステートメントをここに挿入します
	return layers[LayerIndex + pushedLayer].GetColor();
}
