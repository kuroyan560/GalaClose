#include "Master.h"
#include"UsersInput.h"

#include"imguiApp.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"

#include<sstream>

#include"CollisionMgr.h"

#include"SceneMgr.h"
#include"CameraMgr.h"

#include"WinApp.h"

Master::Master()
{
}

void Master::Init()
{
	if (Object3D::objs.empty())
	{
		selectObjNum = -1;
	}
	else
	{
		if (selectObjNum != -1)
		{
			while (Object3D::objs.size() <= selectObjNum)
			{
				selectObjNum--;
			}
		}
	}
}

void Master::Update(bool& DebugChanged)
{
	if (debugMode)
	{
		changedFlag = false;
		imguiApp::Instance()->AddCommand(ImguiCommand_Master);

		if (preview)
		{
			debugFrame++;
			if (selectObjNum != -1)
			{
				if (editor_3d != nullptr && editor_3d->UpdateFromFrame(nowSelectActive, debugFrame))
				{
					debugFrame = 0;
				}
			}
			else if (selectSpriteNum != -1)
			{
				if (nowSelectEditorMode == ACTIVE_ROTATION)
				{
					if (editor_1d != nullptr && editor_1d->UpdateFromFrame(nowSelectActive, debugFrame))
					{
						debugFrame = 0;
					}
				}
				else
				{
					if (editor_2d != nullptr && editor_2d->UpdateFromFrame(nowSelectActive, debugFrame))
					{
						debugFrame = 0;
					}
				}
			}
		}

		if (activeChanged)
		{
			debugFrame = 0;
			if (!preview)
			{
				if (selectObjNum != -1)
				{
					editor_3d->UpdateFromFrame(nowSelectActive, debugFrame);
				}
				if (selectSpriteNum != -1)
				{
					if (nowSelectEditorMode == ACTIVE_ROTATION)
					{
						editor_1d->UpdateFromFrame(nowSelectActive, debugFrame);
					}
					else
					{
						editor_2d->UpdateFromFrame(nowSelectActive, debugFrame);
					}
				}
			}
			activeChanged = false;
		}

		//マウスカーソル選択
		if (UsersInput::Instance()->OnTrigger(UsersInput::MOUSE::LEFT))
		{
			if (UsersInput::Instance()->Input(DIK_LSHIFT))
			{
				Sprite* sprite = Sprite::HitCheckAll(UsersInput::Instance()->GetMousePos(), selectSpriteNum);
				if (sprite != nullptr)
				{
					selectSpriteNum = sprite->GetID();
					selectObjNum = -1;	//３Dオブジェの方は選択解除
					debugFrame = 0;		//プレビュー用のフレーム数も初期化
				}
			}
			else
			{
				RaycastHit hitInfo;
				if (CollisionMgr::Instance()->Raycast(UsersInput::Instance()->GetMouseRay(), &hitInfo))
				{
					selectObjNum = ((Object3D*)hitInfo.object)->GetObjID();		//選択３Dオブジェクトを変更
					selectSpriteNum = -1;	//スプライトの方は選択解除
					debugFrame = 0;		//プレビュー用のフレーム数も初期化
				}
			}
		}
	}
	else
	{
	}

}

void Master::Set(bool Changed, bool& Flag)
{
	if (!Flag && Changed)
	{
		Flag = true;
	}
}

void ImguiCommand_Master()
{
	Master* master = Master::Instance();

#pragma region シーンチェンジ制御
	SceneMgr* mgr = SceneMgr::Instance();
	ImGui::Begin("SceneMgr");
	master->sceneChanged = false;
	for (int i = 0; i < mgr->scenes.size(); i++)
	{
		if (ImGui::RadioButton(mgr->scenes[i]->name, &mgr->nextScene, i) && !master->sceneChanged)
		{
			master->sceneChanged = true;
		}
	}
	ImGui::End();

#pragma endregion

#pragma region ３Dオブジェクト操作
	if (!Object3D::objs.empty() && master->selectObjNum != -1 && master->selectSpriteNum == -1)
	{
		ImGui::Begin("3DObjects", false, ImGuiWindowFlags_NoFocusOnAppearing);

		//オブジェクトの選択
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
		for (int i = 0; i < Object3D::objs.size(); i++)
		{
			ImGui::RadioButton(Object3D::objs[i]->name.c_str(), &master->selectObjNum, i);
		}
		ImGui::EndChild();

		Object3D* selectObj = Object3D::objs[master->selectObjNum];

		//選択オブジェクトのパラメータ入力
		ImGui::InputText("Name", &selectObj->name[0], 30);
		master->Set(imguiApp::InputFloat3FromVec3("Pos", &selectObj->pos),master->changedFlag);
		master->Set(imguiApp::InputFloat3FromVec3("Scale", &selectObj->scale), master->changedFlag);
		master->Set(imguiApp::InputFloat3FromVec3("Rotation", &selectObj->rotation), master->changedFlag);
		ImGui::Checkbox("IsAlive", &selectObj->isAlive);
		if (selectObj->rotation.x == 360)selectObj->rotation.x = 0;
		if (selectObj->rotation.y == 360)selectObj->rotation.y = 0;
		if (selectObj->rotation.z == 360)selectObj->rotation.z = 0;
		if (master->changedFlag)
		{
			selectObj->MatConstMapping();
		}
		ImGui::End();

		//選択オブジェクトの動き設定
		ImGui::Begin("ActivityEditor");
		if (selectObj->actEditors != nullptr)
		{
			ImGui::Text("name : %s", selectObj->actEditors->name.c_str());
		}
		//入力されたアクティビティエディタ名から読み込み
		ImGui::InputText(ActEditors_Obj3D::EXTENSION.c_str(), master->aeBuff, master->MAX_AE_CHAR_COUNT);
		//アクティビティエディタが存在しないならそのクリエイトボタン
		if (selectObj->actEditors == nullptr)
		{
			if (ImGui::Button("Create"))
			{
				if (master->aeBuff[0] == '\0')
				{
					WinApp::Instance()->CreateMessageWindow("ERROR", "名前を入力してね。");
				}
				else
				{
					selectObj->CreateActiveEditor(master->aeBuff);
				}
			}
		}
		else
		{
			if (ImGui::Button("Delete"))
			{
				selectObj->DeleteActiveEditor();
				selectObj->actEditors = nullptr;
			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				selectObj->actEditors->SaveAE();
			}
		}
		ImGui::SameLine();
		if(ImGui::Button("Load"))
		{
			bool save = false;
			bool loaded = false;
			if (selectObj->actEditors != nullptr)	//既に存在する場合保存するか質問する
			{
				if (selectObj->actEditors->name == master->aeBuff)
				{
					WinApp::Instance()->CreateMessageWindow("ERROR", "既にロードされているものと同じだから何もしないよ。");
					loaded = true;
				}
				else
				{
					WinApp::Instance()->CreateMessageWindow("Save ActiveEditor?", ".aeファイルとしてセーブする？", &save);
				}
			}
			//既に読み込み済ならそもそも処理を行わない、読み込み失敗したらエラー文表示
			if (!loaded && selectObj->LoadActive(master->aeBuff, save) == false)
			{
				WinApp::Instance()->CreateMessageWindow("ERROR", "失敗したよ！");
			}
			master->debugFrame = 0;
		}
		ImGui::RadioButton("Pos", &master->nowSelectEditorMode, ACTIVE_POS);
		ImGui::SameLine();
		ImGui::RadioButton("Scale", &master->nowSelectEditorMode, ACTIVE_SCALE);
		ImGui::SameLine();
		ImGui::RadioButton("Rotation", &master->nowSelectEditorMode, ACTIVE_ROTATION);
		ImGui::SameLine();
		master->Set(ImGui::Checkbox("Preview", &master->preview), master->activeChanged);

		//アクティビティエディタが存在するならそのパラメータ調整
		if(selectObj->actEditors != nullptr)
		{
			master->editor_3d = selectObj->actEditors->activityEditor[master->nowSelectEditorMode];
			ImGui::Checkbox("Loop", &master->editor_3d->loop);
			ImGui::SameLine();
			ImGui::Checkbox("InitStart", &master->editor_3d->initStart);
			ImGui::SameLine();
			std::ostringstream oss;
			oss << "Frame : " << master->debugFrame;
			ImGui::Text(oss.str().c_str());

			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(330, 500), ImGuiWindowFlags_NoTitleBar);
			char c;

			ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
			for (int i = 0; i < master->editor_3d->actives.size(); i++)
			{
				c = '0' + i;

				string str("ActiveNum");
				str += c;
				master->Set(ImGui::RadioButton(str.c_str(), &master->nowSelectActive, i), master->activeChanged);
			}

			Active_3D* act = nullptr;
			if (!selectObj->actEditors->activityEditor[master->nowSelectEditorMode]->actives.empty())
			{
				while (selectObj->actEditors->activityEditor[master->nowSelectEditorMode]->actives.size() <= master->nowSelectActive)
				{
					master->nowSelectActive--;
				}
				act = &selectObj->actEditors->activityEditor[master->nowSelectEditorMode]->actives[master->nowSelectActive];
			}

			if (act != nullptr)
			{
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(280, 130), ImGuiWindowFlags_NoTitleBar);
				master->Set(ImGui::InputInt("frame", &act->frame), master->activeChanged);
				if (act->frame < 1)act->frame = 1;
				master->Set(imguiApp::InputFloat3FromVec3("start", &act->start), master->activeChanged);
				master->Set(imguiApp::InputFloat3FromVec3("end", &act->end), master->activeChanged);
				master->Set(imguiApp::SliderFloat2FromVec2("p1", &act->p1, 0.0f, 1.0f), master->activeChanged);
				master->Set(imguiApp::SliderFloat2FromVec2("p2", &act->p2, 0.0f, 1.0f), master->activeChanged);
				ImGui::EndChild();
			}

			if (ImGui::Button("+"))
			{
				selectObj->actEditors->activityEditor[master->nowSelectEditorMode]->CreateActive(master->nowSelectEditorMode);
				master->nowSelectActive = selectObj->actEditors->activityEditor[master->nowSelectEditorMode]->actives.size() - 1;
			}
			ImGui::SameLine(40);
			if (ImGui::Button("-"))
			{
				selectObj->actEditors->activityEditor[master->nowSelectEditorMode]->DeleteActive(master->nowSelectActive);
				if (master->nowSelectActive != 0)master->nowSelectActive--;
			}

			if (act != nullptr)
			{
				ImGui::SameLine(200);
				if (ImGui::Button("Start"))
				{
					if (master->nowSelectEditorMode == ACTIVE_POS)act->start = selectObj->pos;
					if (master->nowSelectEditorMode == ACTIVE_SCALE)act->start = selectObj->scale;
					if (master->nowSelectEditorMode == ACTIVE_ROTATION)act->start = selectObj->rotation;
				}
				ImGui::SameLine(250);
				if (ImGui::Button("End"))
				{
					if (master->nowSelectEditorMode == ACTIVE_POS)act->end = selectObj->pos;
					if (master->nowSelectEditorMode == ACTIVE_SCALE)act->end = selectObj->scale;
					if (master->nowSelectEditorMode == ACTIVE_ROTATION)act->end = selectObj->rotation;
				}
			}
			ImGui::EndChild();
		}


		ImGui::End();
	}
#pragma endregion

#pragma region スプライト操作
//	if (!Sprite_Normal::STATIC_SPRITES.empty() && master->selectSpriteNum != -1 && master->selectObjNum == -1)
//	{
//		ImGui::Begin("SpriteObjects", false, ImGuiWindowFlags_NoFocusOnAppearing);
//
//		//オブジェクトの選択
//		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
//
//		for (int i = 0; i < Sprite_Normal::STATIC_SPRITES.size(); i++)
//		{
//			ImGui::RadioButton(Sprite_Normal::STATIC_SPRITES[i]->name.c_str(), &master->selectSpriteNum, i);
//		}
//		ImGui::EndChild();
//
//		Sprite_Normal* selectSprite = Sprite_Normal::STATIC_SPRITES[master->selectSpriteNum];
//
//		//選択オブジェクトのパラメータ入力
//		ImGui::InputText("Name", &selectSprite->name[0], 30);
//		master->Set(imguiApp::InputFloat2FromVec2("Pos", &selectSprite->pos), master->changedFlag);
//		master->Set(imguiApp::InputFloat2FromVec2("Scale", &selectSprite->scale), master->changedFlag);
//		master->Set(ImGui::InputFloat("Rotation", &selectSprite->rotateZ), master->changedFlag);
//		if (selectSprite->rotateZ == 360)selectSprite->rotateZ = 0;
//		ImGui::Checkbox("DrawFlag", &selectSprite->draw);
//
//		if (master->changedFlag)
//		{
//			selectSprite->ConstMapping();
//		}
//		ImGui::End();
//
//		//選択オブジェクトの動き設定
//		ImGui::Begin("ActivityEditor");
//		if (selectSprite->actEditors != nullptr)
//		{
//			ImGui::Text("name : %s", selectSprite->actEditors->name.c_str());
//		}
//		//入力されたアクティビティエディタ名から読み込み
//		ImGui::InputText(ActEditors_Sprite::EXTENSION.c_str(), master->aeBuff, master->MAX_AE_CHAR_COUNT);
//
//		//アクティビティエディタが存在しないならそのクリエイトボタン
//		if (selectSprite->actEditors == nullptr)
//		{
//			if (ImGui::Button("Create"))
//			{
//				if (master->aeBuff[0] == '\0')
//				{
//					WinApp::Instance()->CreateMessageWindow("ERROR", "名前を入力してね。");
//				}
//				else
//				{
//					selectSprite->CreateActiveEditor(master->aeBuff);
//				}
//			}
//		}
//		else
//		{
//			if (ImGui::Button("Delete"))
//			{
//				selectSprite->DeleteActiveEditor();
//				selectSprite->actEditors = nullptr;
//			}
//		}
//		ImGui::SameLine();
//		//ロードボタン
//		if (ImGui::Button("Load"))
//		{
//			bool loaded = false;
//			bool save = false;
//			if (selectSprite->actEditors != nullptr)	//既に存在する場合保存するか質問する
//			{
//				//既にロードされているものと同名なら何もしない
//				if (selectSprite->actEditors->GetName() == master->aeBuff)
//				{
//					WinApp::Instance()->CreateMessageWindow("ERROR", "既にロードされているものと同じだから何もしないよ。");
//					loaded = true;
//				}
//				else
//				{
//					WinApp::Instance()->CreateMessageWindow("Save ActiveEditor?", ".aeファイルとしてセーブする？", &save);
//				}
//			}
//			//既に読み込み済ならそもそも読み込みの動作をしない、読み込み失敗したらエラー文表示
//			if (!loaded && selectSprite->LoadActive(master->aeBuff, save) == false)
//			{
//				WinApp::Instance()->CreateMessageWindow("ERROR", "失敗したよ！");
//			}
//			master->debugFrame = 0;
//		}
//
//		ImGui::RadioButton("Pos", &master->nowSelectEditorMode, ACTIVE_POS);
//		ImGui::SameLine();
//		ImGui::RadioButton("Scale", &master->nowSelectEditorMode, ACTIVE_SCALE);
//		ImGui::SameLine();
//		ImGui::RadioButton("Rotation", &master->nowSelectEditorMode, ACTIVE_ROTATION);
//		ImGui::SameLine();
//		master->Set(ImGui::Checkbox("Preview", &master->preview), master->activeChanged);
//
//		//アクティビティエディタが存在するならそのパラメータ調整
//		if(selectSprite->actEditors != nullptr)
//		{
//			if (master->nowSelectEditorMode == ACTIVE_POS || master->nowSelectEditorMode == ACTIVE_SCALE)
//			{
//				master->editor_2d = selectSprite->actEditors->GetEditor2D(master->nowSelectEditorMode);
//				ImGui::Checkbox("Loop", &master->editor_2d->loop);
//				ImGui::SameLine();
//				ImGui::Checkbox("InitStart", &master->editor_2d->initStart);
//				ImGui::SameLine();
//				std::ostringstream oss;
//				oss << "Frame : " << master->debugFrame;
//				ImGui::Text(oss.str().c_str());
//
//				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(330, 500), ImGuiWindowFlags_NoTitleBar);
//				char c;
//
//				ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
//				for (int i = 0; i < master->editor_2d->actives.size(); i++)
//				{
//					c = '0' + i;
//
//					string str("ActiveNum");
//					str += c;
//					master->Set(ImGui::RadioButton(str.c_str(), &master->nowSelectActive, i), master->activeChanged);
//				}
//
//				Active_2D* act = nullptr;
//				if (!master->editor_2d->actives.empty())
//				{
//					while (master->editor_2d->actives.size() <= master->nowSelectActive)
//					{
//						master->nowSelectActive--;
//					}
//					act = &master->editor_2d->actives[master->nowSelectActive];
//				}
//
//				if (act != nullptr)
//				{
//					ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(280, 130), ImGuiWindowFlags_NoTitleBar);
//					master->Set(ImGui::InputInt("frame", &act->frame), master->activeChanged);
//					if (act->frame < 1)act->frame = 1;
//					master->Set(imguiApp::InputFloat2FromVec2("start", &act->start), master->activeChanged);
//					master->Set(imguiApp::InputFloat2FromVec2("end", &act->end), master->activeChanged);
//					master->Set(imguiApp::SliderFloat2FromVec2("p1", &act->p1, 0.0f, 1.0f), master->activeChanged);
//					master->Set(imguiApp::SliderFloat2FromVec2("p2", &act->p2, 0.0f, 1.0f), master->activeChanged);
//					ImGui::EndChild();
//				}
//
//				if (ImGui::Button("+"))
//				{
//					master->editor_2d->CreateActive(master->nowSelectEditorMode);
//					master->nowSelectActive = master->editor_2d->actives.size() - 1;
//				}
//				ImGui::SameLine(40);
//				if (ImGui::Button("-"))
//				{
//					master->editor_2d->DeleteActive(master->nowSelectActive);
//					if (master->nowSelectActive != 0)master->nowSelectActive--;
//				}
//
//				if (act != nullptr)
//				{
//					ImGui::SameLine(200);
//					if (ImGui::Button("Start"))
//					{
//						if (master->nowSelectEditorMode == ACTIVE_POS)act->start = selectSprite->pos;
//						if (master->nowSelectEditorMode == ACTIVE_SCALE)act->start = selectSprite->scale;
//					}
//					ImGui::SameLine(250);
//					if (ImGui::Button("End"))
//					{
//						if (master->nowSelectEditorMode == ACTIVE_POS)act->end = selectSprite->pos;
//						if (master->nowSelectEditorMode == ACTIVE_SCALE)act->end = selectSprite->scale;
//					}
//				}
//				ImGui::EndChild();
//			}
//			else
//			{
//				master->editor_1d = selectSprite->actEditors->actEditor_rotate;
//				ImGui::Checkbox("Loop", &master->editor_1d->loop);
//				ImGui::SameLine();
//				ImGui::Checkbox("InitStart", &master->editor_1d->initStart);
//				ImGui::SameLine();
//				std::ostringstream oss;
//				oss << "Frame : " << master->debugFrame;
//				ImGui::Text(oss.str().c_str());
//
//				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(330, 500), ImGuiWindowFlags_NoTitleBar);
//				char c;
//
//				ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
//				for (int i = 0; i < master->editor_1d->actives.size(); i++)
//				{
//					c = '0' + i;
//
//					string str("ActiveNum");
//					str += c;
//					master->Set(ImGui::RadioButton(str.c_str(), &master->nowSelectActive, i), master->activeChanged);
//				}
//
//				Active_1D* act = nullptr;
//				if (!master->editor_1d->actives.empty())
//				{
//					while (master->editor_1d->actives.size() <= master->nowSelectActive)
//					{
//						master->nowSelectActive--;
//					}
//					act = &master->editor_1d->actives[master->nowSelectActive];
//				}
//
//				if (act != nullptr)
//				{
//					ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(280, 130), ImGuiWindowFlags_NoTitleBar);
//					master->Set(ImGui::InputInt("frame", &act->frame), master->activeChanged);
//					if (act->frame < 1)act->frame = 1;
//					master->Set(ImGui::InputFloat("start", &act->start), master->activeChanged);
//					master->Set(ImGui::InputFloat("end", &act->end), master->activeChanged);
//					master->Set(imguiApp::SliderFloat2FromVec2("p1", &act->p1, 0.0f, 1.0f), master->activeChanged);
//					master->Set(imguiApp::SliderFloat2FromVec2("p2", &act->p2, 0.0f, 1.0f), master->activeChanged);
//					ImGui::EndChild();
//				}
//
//				if (ImGui::Button("+"))
//				{
//					master->editor_1d->CreateActive(master->nowSelectEditorMode);
//					master->nowSelectActive = master->editor_1d->actives.size() - 1;
//				}
//				ImGui::SameLine(40);
//				if (ImGui::Button("-"))
//				{
//					master->editor_1d->DeleteActive(master->nowSelectActive);
//					if (master->nowSelectActive != 0)master->nowSelectActive--;
//				}
//
//				if (act != nullptr)
//				{
//					ImGui::SameLine(200);
//					if (ImGui::Button("Start"))
//					{
//						act->start = selectSprite->rotateZ;
//					}
//					ImGui::SameLine(250);
//					if (ImGui::Button("End"))
//					{
//						act->end = selectSprite->rotateZ;
//					}
//				}
//				ImGui::EndChild();
//			}
//		}
//
//		ImGui::End();
//	}
#pragma endregion

#pragma region カメラ
	CameraMgr* camMgr = CameraMgr::Instance();
	ImGui::Begin("CameraMgr");
	int masterSelectCam = camMgr->nowCameraNum;
	for (int i = 0; i < camMgr->cameras.size(); i++)
	{
		if (ImGui::RadioButton(camMgr->cameras[i]->name.c_str(), &masterSelectCam, i))
		{
			camMgr->SetCam(masterSelectCam);
			Object3D::MatConstMappingAll();
		}
	}
	Camera* nowCam = camMgr->cameras[masterSelectCam];
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(280, 130), ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("eye    x:%.2f y:%.2f z:%.2f", nowCam->cameraInfo.eye.x, nowCam->cameraInfo.eye.y, nowCam->cameraInfo.eye.z);
	ImGui::Text("target x:%.2f y:%.2f z:%.2f", nowCam->target.x, nowCam->target.y, nowCam->target.z);
	ImGui::Text("up     x:%.2f y:%.2f z:%.2f", nowCam->up.x, nowCam->up.y, nowCam->up.z);
	ImGui::EndChild();

	ImGui::End();
#pragma endregion
}