#include "PanelMgr.h"
#include"Panel.h"
#include"Constants.h"
#include"../../SRVDescMgr.h"
#include"../../Sprite.h"
#include"../../Easing.h"
#include"../../WinApp.h"
#include"EnemyMgr.h"
#include"Player.h"
#include"QueenMgr.h"
#include"../../AudioMgr.h"
#include"GameOver.h"

PanelMgr::PanelMgr()
{
	Panel::LoadAudios();
	panelChangeSprite.reset(new Sprite_Normal(
		"パネルチェンジスプライト", SRVDescMgr::Instance()->CreateSRV("resource/img/panelChange.png"),
		MyDef::int2{ 1010,218 }, { 0,0 }, false, false, { 0.5f,0.5f }));

	lvUpSE = AudioMgr::Instance()->LoadAudio("resource/sound/lvUpSE.wav");
}

void PanelMgr::Init(int PanelNum, int MaxLayerNum, bool ColorDifference, bool ScoreAtack)
{
	if (ScoreAtack)ColorDifference = true;

	panelAngleOffset = (PI * 2.0f) / (float)PanelNum;
	for (int i = 0; i < PanelNum; ++i)
	{
		float startAngle = i * panelAngleOffset;
		float endAngle = (i + 1) * panelAngleOffset;
		panels.emplace_back(new Panel(startAngle, endAngle, MaxLayerNum, i, ColorDifference));
	}
	//panels.emplace_back(new Panel(PI / 2.0f, PI / 2.0f - (0 + 1) * panelAngleOffset, MaxLayerNum, 0));
	maxLayerNum = MaxLayerNum;
	oldMaxLayerNum = maxLayerNum;

	panelChangeFlag = -1;

	scoreAtack = ScoreAtack;
}

void PanelMgr::Update()
{
	oldMaxLayerNum = maxLayerNum;
	for (auto panel : panels)
	{
		panel.get()->Update();
	}

	//スコア制のみ
	if (scoreAtack)
	{
		if (panelChangeFlag == 0)
		{
			panelChangeSprite->GetComponent().transform.rotate.x
				= Easing::Ease(Out, Exp, panelChangeTimer, PANEL_CHANGE_EASE_TOTAL_TIME_0, 360 * 3, 10);
			panelChangeSprite->GetComponent().transform.scale.x
				= Easing::Ease(Out, Exp, panelChangeTimer, PANEL_CHANGE_EASE_TOTAL_TIME_0, 0.0f, 1.0f);
			panelChangeSprite->GetComponent().transform.scale.y
				= Easing::Ease(Out, Exp, panelChangeTimer, PANEL_CHANGE_EASE_TOTAL_TIME_0, 0.0f, 1.0f);

			if (panelChangeTimer < PANEL_CHANGE_EASE_TOTAL_TIME_0)
			{
				panelChangeTimer++;
			}
			if (panelChangeTimer == PANEL_CHANGE_EASE_TOTAL_TIME_0)
			{
				panelChangeTimer = 0;
				panelChangeFlag = 1;
			}
		}
		else if (panelChangeFlag == 1)
		{
			panelChangeSprite->GetComponent().color.color.w
				= Easing::Ease(In, Sine, panelChangeTimer, PANEL_CHANGE_EASE_TOTAL_TIME_1, 1.0f, 0.0f);
			panelChangeSprite->GetComponent().transform.pos.y
				= Easing::Ease(In, Quart, panelChangeTimer, PANEL_CHANGE_EASE_TOTAL_TIME_1,
					WinApp::Instance()->GetWinCenter().y, WinApp::Instance()->GetWinCenter().y + 60);

			if (panelChangeTimer < PANEL_CHANGE_EASE_TOTAL_TIME_1)
			{
				panelChangeTimer++;
			}
			if (panelChangeTimer == PANEL_CHANGE_EASE_TOTAL_TIME_1)
			{
				panelChangeFlag = -1;
			}
		}
	}
}

void PanelMgr::Draw()
{
	for (auto panel : panels)
	{
		panel.get()->Draw();
	}
}

void PanelMgr::UIDraw()
{
	if (panelChangeFlag != -1)
	{
		panelChangeSprite->Draw();
	}
}

void PanelMgr::Finalize()
{
	panels.clear();
}

void PanelMgr::PushLayer(const int& PanelIndex, const int& LayerNum)
{
	if (PanelIndex < 0)return;
	if (panels.size() <= PanelIndex)return;

	panels[PanelIndex].get()->Pushed(LayerNum);
}

void PanelMgr::ResetLayer(const int& PanelIndex)
{
	if (PanelIndex < 0)return;
	if (panels.size() <= PanelIndex)return;

	panels[PanelIndex].get()->PushReset();
}

void PanelMgr::AllResetLayer()
{
	for (auto panel : panels)
	{
		panel.get()->PushReset();
	}
}

void PanelMgr::AddMaxLayer()
{
	if (LAYER_NUM_MAX_LIMIT <= maxLayerNum)
	{
		printf("レイヤー数は既に最大");
		return;
	}

	maxLayerNum++;
	for (auto panel : panels)
	{
		panel.get()->AddLayer();
	}

	EnemyMgr::Instance()->AllDead();
	QueenMgr::Instance()->AllDead();
	for (int i = 0; i < maxLayerNum - 1; ++i)
	{
		EnemyMgr::Instance()->RandGenerate(i);
	}
}

std::shared_ptr<Panel> PanelMgr::GetPanelByIndex(int PanelIndex)
{
	if (PanelIndex < 0)PanelIndex = panels.size() - 1;
	else if (panels.size() <= PanelIndex)PanelIndex = 0;
	return panels[PanelIndex];
}

std::shared_ptr<Panel> PanelMgr::GetPanelByAngle(float Angle, bool IsClockWise)
{
	if (Angle < 0)
	{
		Angle += PI * 2;
	}
	else if (PI * 2 < Angle)
	{
		Angle -= PI * 2;
	}

	for (auto& panel : panels)
	{
		if (IsClockWise)
		{
			if ((panel->GetStartAngle() <= Angle && Angle < panel->GetEndAngle()) || Angle == PI * 2)
			{
				return panel;
			}
		}
		else
		{
			if (panel->GetStartAngle() < Angle && Angle <= panel->GetEndAngle() || Angle == 0.0f)//反時計回り
			{
				return panel;
			}
		}
	}

	DebugBreak();
}

int PanelMgr::AddPanelAndLayerReset()
{
	if (GameOver::Instance()->Flag())return 0;

	int result = 0;

	if (maxLayerNum < LAYER_NUM_MAX_LIMIT)
	{
		AddMaxLayer();
	}
	else
	{
		int num = panels.size();
		if (num < 8)
		{
			this->Finalize();
			this->Init(num + 2, 1, true, true);
			panelChangeFlag = 0;
			panelChangeTimer = 0;
			EnemyMgr::Instance()->AllDead();
			QueenMgr::Instance()->AllDead();
			Player::Instance()->Init();
			//EnemyMgr::Instance()->RandGenerate(0);
			panelChangeSprite->GetComponent().transform.pos = WinApp::Instance()->GetWinCenter();
			panelChangeSprite->GetComponent().color.color.w = 1.0f;
			panelChangeSprite->GetComponent().MatMapping();
			panelChangeSprite->GetComponent().ColorMapping();
			EnemyMgr::Instance()->RandGenerate(0);

			AudioMgr::Instance()->PlayWave(lvUpSE);
		}
		else
		{
			result = 1;
		}
	}

	return result;
}
