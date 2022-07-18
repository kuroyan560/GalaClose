#include "Player.h"
#include"../../UsersInput.h"
#include"PanelMgr.h"
#include"../../Model_HSM.h"
#include"../../FbxLoader.h"
#include"Constants.h"
#include"PanelMgr.h"
#include"../../Easing.h"
#include"../../AudioMgr.h"
#include"GameOver.h"

//連打判定
const int REPEAT_JUDGE_TOTAL_TIME = 20;

void Player::ArrowMove()
{
	float angleOffset = -PI * 2 / PanelMgr::Instance()->GetPanelNum();
	float angle = angleOffset * selectPanelIndex + angleOffset / 2.0f;
	float radius = PLAYER_ARROW_RADIUS_OFFSET + (PanelMgr::Instance()->GetMaxLayerNum() - 2) * LAYER_OFFSET_RADIUS;

	auto& trans = arrowModel->GetComponent().transform;
	trans.pos.x = cos(angle) * (radius + pushRadiusOffset);
	trans.pos.y = sin(angle) * (radius + pushRadiusOffset);
	trans.up = MyFunc::NormalizeXMFLOAT3(MyFunc::SubXMFLOAT3({ 0,0,0 }, arrowModel.get()->GetComponent().transform.pos));
	trans.scale.x = 1.0f + pushScaleOffset * PanelMgr::Instance()->GetMaxLayerNum();
	trans.scale.y = 1.0f + pushScaleOffset * PanelMgr::Instance()->GetMaxLayerNum();
	trans.rotate.y = pushAngleOffset;
}

void Player::PushInputUpdate()
{
	//入力を受け付ける
	if (UsersInput::Instance()->OnTrigger(DIK_SPACE) && selectPanel->CanControl()
		&& pushLayerNum < PanelMgr::Instance()->GetMaxLayerNum() - selectPanel->pushedLayer)
	{
		AudioMgr::Instance()->PlayWave(arrowJumpSeHandle[MUSICAL_SCALE_NUM <= pushLayerNum ? pushLayerNum - MUSICAL_SCALE_NUM : pushLayerNum]);
		inputWaitFrame = 0;
		pushLayerNum++;
		
		startRadius = pushRadiusOffset;
		endRadius = pushRadiusOffset + pushLayerNum * PLAYER_ARROW_JUMP_RADIUS_OFFSET;

		startScale = pushScaleOffset;
		endScale = pushLayerNum * PLAYER_ARROW_SCALE_OFFSET;

		startAngle = pushAngleOffset;
		endAngle = pushLayerNum * PLAYER_ARROW_ANGLE_OFFSET;
	}

	//連続押し込み入力の許容フレーム
	if (inputWaitFrame < PUSH_INPUT_WAIT_FRAME)
	{
		//待機時間経過
		inputWaitFrame++;

		//矢印ジャンプ
		pushRadiusOffset = Easing::Ease(Out, Quint, inputWaitFrame, PUSH_INPUT_WAIT_FRAME, startRadius, endRadius);
		pushScaleOffset = Easing::Ease(Out, Elastic, inputWaitFrame, PUSH_INPUT_WAIT_FRAME, startScale, endScale);
		pushAngleOffset = Easing::Ease(Out, Exp, inputWaitFrame, PUSH_INPUT_WAIT_FRAME, startAngle, endAngle);

		//押し込み入力が完了
		if (inputWaitFrame == PUSH_INPUT_WAIT_FRAME && pushLayerNum != 0)
		{
			PanelMgr::Instance()->PushLayer(selectPanelIndex, pushLayerNum);
			//printf("%d 番パネル、%d 枚押し込み\n", selectPanelIndex, pushLayerNum);
			pushLayerNum = 0;
		}
	}
}

bool Player::MoveClockWise()
{
	/*int panelNum = PanelMgr::Instance()->GetPanelNum();

	if (panelNum == 4)
	{
		if (UsersInput::Instance()->OnTrigger(DIK_RIGHT))
		{
			oldKeyCode = DIK_RIGHT;
			return selectPanelIndex == 2 || selectPanelIndex == 3;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_LEFT))
		{
			oldKeyCode = DIK_LEFT;
			return selectPanelIndex == 0 || selectPanelIndex == 1;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_UP))
		{
			oldKeyCode = DIK_UP;
			return selectPanelIndex == 1;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_DOWN))
		{
			oldKeyCode = DIK_DOWN;
			return selectPanelIndex == 3;
		}
	}
	else if (panelNum == 6)
	{
		if (UsersInput::Instance()->OnTrigger(DIK_RIGHT))
		{
			oldKeyCode = DIK_RIGHT;
			return 3 <= selectPanelIndex && selectPanelIndex <= 5;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_LEFT))
		{
			oldKeyCode = DIK_LEFT;
			return 0 <= selectPanelIndex && selectPanelIndex <= 2;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_UP))
		{
			oldKeyCode = DIK_UP;
			return selectPanelIndex == 2 || selectPanelIndex == 3;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_DOWN))
		{
			oldKeyCode = DIK_DOWN;
			return selectPanelIndex == 5 || selectPanelIndex == 0;
		}
	}
	else
	{
		if (UsersInput::Instance()->OnTrigger(DIK_RIGHT))
		{
			oldKeyCode = DIK_RIGHT;
			return 4 <= selectPanelIndex && selectPanelIndex <= 7;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_LEFT))
		{
			oldKeyCode = DIK_LEFT;
			return 0 <= selectPanelIndex && selectPanelIndex <= 3;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_UP))
		{
			oldKeyCode = DIK_UP;
			return 2 <= selectPanelIndex && selectPanelIndex <= 4;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_DOWN))
		{
			oldKeyCode = DIK_DOWN;
			return selectPanelIndex == 0 || selectPanelIndex == 6 || selectPanelIndex == 7;
		}
	}

	return false;*/
	return UsersInput::Instance()->OnTrigger(DIK_LEFT);
}

bool Player::MoveUnClockWise()
{
	/*int panelNum = PanelMgr::Instance()->GetPanelNum();

	if (panelNum == 4)
	{
		if (UsersInput::Instance()->OnTrigger(DIK_RIGHT))
		{
			oldKeyCode = DIK_RIGHT;
			return selectPanelIndex == 0 || selectPanelIndex == 1;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_LEFT))
		{
			oldKeyCode = DIK_LEFT;
			return selectPanelIndex == 2 || selectPanelIndex == 3;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_UP))
		{
			oldKeyCode = DIK_UP;
			return selectPanelIndex == 0;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_DOWN))
		{
			oldKeyCode = DIK_DOWN;
			return selectPanelIndex == 2;
		}
	}
	else if (panelNum == 6)
	{
		if (UsersInput::Instance()->OnTrigger(DIK_RIGHT))
		{
			oldKeyCode = DIK_RIGHT;
			return 0 <= selectPanelIndex && selectPanelIndex <= 2;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_LEFT))
		{
			oldKeyCode = DIK_LEFT;
			return 3 <= selectPanelIndex && selectPanelIndex <= 5;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_UP))
		{
			oldKeyCode = DIK_UP;
			return selectPanelIndex == 5 || selectPanelIndex == 0;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_DOWN))
		{
			oldKeyCode = DIK_DOWN;
			return selectPanelIndex == 2 || selectPanelIndex == 3;
		}
	}
	else
	{
		if (UsersInput::Instance()->OnTrigger(DIK_RIGHT))
		{
			oldKeyCode = DIK_RIGHT;
			return 0 <= selectPanelIndex && selectPanelIndex <= 3;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_LEFT))
		{
			oldKeyCode = DIK_LEFT;
			return 4 <= selectPanelIndex && selectPanelIndex <= 7;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_UP))
		{
			oldKeyCode = DIK_UP;
			return selectPanelIndex == 0 || selectPanelIndex == 6 || selectPanelIndex == 7;
		}
		else if (UsersInput::Instance()->OnTrigger(DIK_DOWN))
		{
			oldKeyCode = DIK_DOWN;
			return 2 <= selectPanelIndex && selectPanelIndex <= 4;
		}
	}

	return false;*/
	return UsersInput::Instance()->OnTrigger(DIK_RIGHT);
}

Player::Player()
{
	Model_HSM* fbxModel;
	fbxModel = FBXLoader::LoadFBXModel("resource/model/arrow/", "arrow.fbx");

	arrowModel.reset(new Model_HSM_Normal(fbxModel));

	dirty.AddInspector(&selectPanelIndex);
	dirty.AddInspector(&inputWaitFrame);
	dirty.AddInspector(&arrowPushTimer);

	std::string dir = "resource/sound/musicalScale/";
	for (int i = 0; i < MUSICAL_SCALE_NUM; ++i)
	{
		arrowJumpSeHandle[i] = AudioMgr::Instance()->LoadAudio(dir + "scale_" + to_string(i) + ".wav");
	}

	selectSe = AudioMgr::Instance()->LoadAudio("resource/sound/selectSE.wav");
}

void Player::Init()
{
	arrowModel->GetComponent().DirtyRegister();
	dirty.Register();

	selectPanelIndex = 0;
	selectPanel = PanelMgr::Instance()->GetPanelByIndex(selectPanelIndex);

	inputWaitFrame = PUSH_INPUT_WAIT_FRAME;
	arrowPushTimer = ARROW_PUSH_LAYER_FRAME;
	pushRadiusOffset = 0.0f;
	pushScaleOffset = 0.0f;
	pushAngleOffset = 0.0f;
	pushLayerNum = 0;

	ArrowMove();
	arrowModel->GetComponent().TransformMatUpdate();
	arrowModel->GetComponent().TransformMapping();

	//repeatTimer = REPEAT_JUDGE_TOTAL_TIME;
}

void Player::Update()
{
	//if (repeatTimer < REPEAT_JUDGE_TOTAL_TIME)
	//{
	//	repeatTimer++;
	//}

	if (PanelMgr::Instance()->AddMaxLayerTrigger())ArrowMove();

	if (!GameOver::Instance()->Flag())
	{
		if (inputWaitFrame == PUSH_INPUT_WAIT_FRAME)
		{
			//連打
			//bool flag = repeatTimer < REPEAT_JUDGE_TOTAL_TIME&& UsersInput::Instance()->OnTrigger(oldKeyCode);
			//if (flag)
			//{
			//	int jjj = 0;
			//}

			if (/*(repeatTimer == REPEAT_JUDGE_TOTAL_TIME && */MoveUnClockWise()/*)
				|| (!clockWise && flag)*/)	//反時計回り
			{
				selectPanelIndex--;
				if (selectPanelIndex < 0)
				{
					selectPanelIndex = PanelMgr::Instance()->GetPanelNum() - 1;
				}
				selectPanel = PanelMgr::Instance()->GetPanelByIndex(selectPanelIndex);
				pushRadiusOffset = -selectPanel->pushedLayer * LAYER_OFFSET_RADIUS;
				arrowPushTimer = ARROW_PUSH_LAYER_FRAME;
				pushRadiusOffset = 0.0f;
				pushScaleOffset = 0.0f;
				pushAngleOffset = 0.0f;
				//printf("選択パネル変更　：%d\n", selectPanelIndex);
				//clockWise = false;
				//repeatTimer = 0;
				AudioMgr::Instance()->PlayWave(selectSe);
			}
			else if (/*(repeatTimer == REPEAT_JUDGE_TOTAL_TIME && */MoveClockWise()/*)
				|| (clockWise && flag)*/) //時計回り
			{
				selectPanelIndex++;
				if (PanelMgr::Instance()->GetPanelNum() - 1 < selectPanelIndex)
				{
					selectPanelIndex = 0;
				}
				selectPanel = PanelMgr::Instance()->GetPanelByIndex(selectPanelIndex);
				pushRadiusOffset = -selectPanel->pushedLayer * LAYER_OFFSET_RADIUS;
				inputWaitFrame = PUSH_INPUT_WAIT_FRAME;
				arrowPushTimer = ARROW_PUSH_LAYER_FRAME;
				pushRadiusOffset = 0.0f;
				pushScaleOffset = 0.0f;
				pushAngleOffset = 0.0f;
				//printf("選択パネル変更　：%d\n", selectPanelIndex);
				//clockWise = true;
				//repeatTimer = 0;
				AudioMgr::Instance()->PlayWave(selectSe);
			}
		}

		PushInputUpdate();
	}

	if (selectPanel->PushedTrigger())
	{
		arrowPushTimer = 0;	//矢印が押し込みモーションに入る

		startRadius = pushRadiusOffset;
		endRadius = -LAYER_OFFSET_RADIUS * selectPanel->pushedLayer;

		startScale = pushScaleOffset;
		endScale = startScale - PLAYER_ARROW_SCALE_OFFSET;

		//回転リセット
		startAngle = 0.0f;
		endAngle = 0.0f;

		if (selectPanel->pushedLayer == 0)
		{
			endRadius = 0.0f;
			endScale = 0.0f;
		}
	}

	if (arrowPushTimer < ARROW_PUSH_LAYER_FRAME)	//矢印が押し込みモーション中
	{
		arrowPushTimer++;

		//矢印押し込み
		pushRadiusOffset = Easing::Ease(Out, Exp, arrowPushTimer, ARROW_PUSH_LAYER_FRAME, startRadius, endRadius);
		pushScaleOffset = Easing::Ease(Out, Back, arrowPushTimer, ARROW_PUSH_LAYER_FRAME, startScale, endScale);
		pushAngleOffset = Easing::Ease(Out, Exp, arrowPushTimer, ARROW_PUSH_LAYER_FRAME, startAngle, endAngle);
	}

	if (dirty.Dirty())
	{
		ArrowMove();
	}
}

void Player::Draw()
{
	arrowModel.get()->Draw();
}

void Player::Finalize()
{
}