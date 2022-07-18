#include "Queen.h"
#include "PanelMgr.h"
#include"../../Easing.h"
#include"../../SRVDescMgr.h"
#include"../../Object2D.h"
#include"../../WinApp.h"

const int DRAIN_TOTAL_TIME = 40;
const XMFLOAT3 DEFAULT_SCALE = { 0.65f,0.65f,0.65f };

const float NUM_SCALE = 0.05f;

Queen::Queen()
{
	movablePanels;
	//component.transform.scale = { 1.0005f,1.0005f,1.0005f };
	component.transform.scale = DEFAULT_SCALE;

	int handle = SRVDescMgr::Instance()->CreateSRV("resource/img/num_black.png");
	for (int i = 0; i < DRAIN_ENEMY_NUM_MAX_DIGIT; ++i)
	{
		numObj2d[i].reset(new Object2D_Divide("QueenDrainEnemyNumObject2D" + to_string(i), { 10,1 }, { 96,128 },
			handle, { 96,128 }, { 0.0f,0.0f,0.0f }, { 0.5f,0.5f },true));
		numObj2d[i]->scale.x = NUM_SCALE;
		numObj2d[i]->scale.y = NUM_SCALE;
	}

	drainDirty.AddInspector(&drainEnemyNum);
}

void Queen::UniqueInit()
{
}

void Queen::UniqueUpdate()
{
	PanelCheck();
	if (drainTimer < DRAIN_TOTAL_TIME)
	{
		drainTimer++;
		component.transform.scale = Easing::Ease(Out, Elastic, drainTimer, DRAIN_TOTAL_TIME,
			MyFunc::DivXMFLOAT3(DEFAULT_SCALE, 3), DEFAULT_SCALE);
	}

	if (component.TransformDirty())
	{
		for (int i = 0; i < 2; ++i)
		{
			float size = numObj2d[i]->size.x * NUM_SCALE * WinApp::Instance()->GetWinDifferRate().x;
			size *= 0.3f;
			numObj2d[i]->pos.x = component.transform.pos.x - size / 2.0f + size * i;
			numObj2d[i]->pos.y = component.transform.pos.y;
			numObj2d[i]->pos.z = component.transform.pos.z - 5.0f;
		}
	}
}

void Queen::UniqueFinalize()
{
}

void Queen::Draw()
{
	if (!isAlive || !drainEnemyNum)return;

	Enemy::Draw();

	if (drainDirty.Dirty())
	{
		char numBuff[25] = { 0 };
		sprintf_s(numBuff, sizeof(numBuff), "%d", drainEnemyNum);

		for (int i = 0; i < 2; ++i)
		{
			int idx = i;
			if (drainEnemyNum < 10)
			{
				idx = 1 - idx;
			}

			if (numBuff[idx] != '\0')numBuff[idx] -= 48;

			numObj2d[i]->SelectImgNum(numBuff[idx], 0);
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		numObj2d[i]->Draw();
	}
}

void Queen::Spawn(const EnemyGenerateHelper& InitData)
{
	Enemy::Spawn(InitData);

	//自分の位置
	XMFLOAT3 pos;
	pos = { cosf(posAngle), -sinf(posAngle), 0 };
	//move *= (offsetLength * (layer + 1));
	float length = INNER_SPACE_RADIUS + ENEMY_OFFSET_LENGTH * layerNum + ENEMY_OFFSET_LENGTH / 2.0f;
	pos = MyFunc::MulXMFLOAT3(pos, length);

	//中心から自分の位置へのベクトル
	XMFLOAT3 vec = MyFunc::SubXMFLOAT3({ 0.0f,0.0f,0.0f }, pos);
	component.transform.rotate.z = atan2(vec.y, vec.x) / (PI / 180.0f) + 90;
	component.transform.scale = DEFAULT_SCALE;
	defaultScale = DEFAULT_SCALE.x;
	component.TransformMapping();

	beDrained = false;
	bodyEmitFlg = false;
	drainEnemyNum = 0;

	drainTimer = DRAIN_TOTAL_TIME;

	for (int i = 0; i < 2; ++i)
	{
		float size = numObj2d[i]->size.x * NUM_SCALE * WinApp::Instance()->GetWinDifferRate().x;
		size *= 0.3f;
		numObj2d[i]->pos.x = pos.x - size / 2.0f + size * i;
		numObj2d[i]->pos.y = pos.y;
		numObj2d[i]->pos.z = pos.z - 5.0f;
	}

	drainDirty.Register();
}

void Queen::Dead()
{
	Enemy::Dead();
	drainDirty.Remove();
}

void Queen::PanelCheck()
{
	vector<movablePanel> SetPanels;

	movablePanel start{ Getlayer(), GetPanelIndex(), true, true};

	if (!panel.lock()->NowPushed())
	{
		SetPanels.emplace_back(start);

		movablePanel next = start,
			prev = start;

		next.CounterClockWise = false;
		prev.ClockWise = false;

		char nowColor = GetPanel().lock()->GetLayerColor(start.layerNum);
		auto setPanel = PanelMgr::Instance()->GetPanelByIndex(next.panelNum);
		char setColor = setPanel.get()->GetLayerColor(next.layerNum);


		bool nextCheck = false, prevCheck = false;
		while (!nextCheck || !prevCheck)
		{
			//next
			if (!nextCheck)
			{
				auto setPanel = PanelMgr::Instance()->GetPanelByIndex(next.panelNum + 1);
				next.panelNum = setPanel.get()->GetPanelIndex();

				char setColor = setPanel.get()->GetLayerColor(next.layerNum);
				if (setColor == nowColor)
				{
					SetPanels.emplace_back(next);
				}
				else
				{
					nextCheck = true;
				}
			}
			//prev
			if (!prevCheck)
			{
				auto setPanel = PanelMgr::Instance()->GetPanelByIndex(prev.panelNum - 1);

				prev.panelNum = setPanel.get()->GetPanelIndex();
				//場所がかち合ったら抜ける
				if (next.panelNum == prev.panelNum)
				{
					//一周すべて繋がったらどちらからも行けるフラグをオンにする
					for (auto movablePanel : SetPanels)
					{
						movablePanel.ClockWise = true;
						movablePanel.CounterClockWise = true;
					}
					break;
				}
				char setColor = setPanel.get()->GetLayerColor(prev.layerNum);
				if (setColor == nowColor)
				{
					SetPanels.emplace_back(prev);
				}
				else
				{
					prevCheck = true;
				}
			}

		}
	}

	//いける方向を加えていく
	movablePanels = SetPanels;

	int count = movablePanels.size();
}


const vector<movablePanel> &Queen::GetmovablePanels()
{
	return movablePanels;
}

void Queen::AddDrainEnemy()
{
	drainEnemyNum++;
	drainTimer = 0;
}
