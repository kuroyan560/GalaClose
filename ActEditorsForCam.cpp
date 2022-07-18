#include "ActEditorsForCam.h"
#include <fstream>
#include <sstream>
#include<iostream>
#include<stdlib.h>
#include"MyFunc.h"
#include"WinApp.h"
#include"GameCamera.h"
const std::string ActEditorsForCam::EXTENSION = ".ae3d_cam";
const std::string ActEditorsForCam::DIRECTORY_PATH = "camera/";

//bool ActEditorsForCam::Load(std::string& ActEditorName)
//{
//	std::string pass;
//	pass = ActivityEditor::DIRECTORY_PATH + ActEditorsForCam::DIRECTORY_PATH + ActEditorName;
//
//	if (!MyFunc::ExistFile(pass + ActEditorsForCam::EXTENSION))
//	{
//		WinApp::Instance()->CreateMessageWindow("ERROR", "モーションエディタ生成時にロードしようとしたけどデータ無かったよ");
//		delete this;
//	}
//
//	std::ifstream file;
//	file.open(pass + EXTENSION);
//	if (!file.fail())
//	{
//		std::string line;
//		int now;
//
//		while (getline(file, line))
//		{
//			std::istringstream line_stream(line);
//
//			std::string key;
//			getline(line_stream, key, ' ');
//
//			if (key == "eye")
//			{
//				now = 0;
//			}
//			else if (key == "rotate")
//			{
//				now = 1;
//			}
//			else if (key == "act")
//			{
//				if (now == 0)
//				{
//					Active_3D active;
//
//					getline(file, line);
//					active.frame = atoi(line.c_str());
//
//					getline(file, line, ' ');
//					active.start.x = atof(line.c_str());
//					getline(file, line, ' ');
//					active.start.y = atof(line.c_str());
//					getline(file, line);
//					active.start.z = atof(line.c_str());
//
//					getline(file, line, ' ');
//					active.end.x = atof(line.c_str());
//					getline(file, line, ' ');
//					active.end.y = atof(line.c_str());
//					getline(file, line);
//					active.end.z = atof(line.c_str());
//
//					getline(file, line, ' ');
//					active.p1.x = atof(line.c_str());
//					getline(file, line);
//					active.p1.y = atof(line.c_str());
//
//					getline(file, line, ' ');
//					active.p2.x = atof(line.c_str());
//					getline(file, line);
//					active.p2.y = atof(line.c_str());
//
//					eyeActEditor->actives.push_back(active);
//				}
//				else if (now == 1)
//				{
//					Active_2D active;
//
//					getline(file, line);
//					active.frame = atoi(line.c_str());
//
//					getline(file, line, ' ');
//					active.start.x = atof(line.c_str());
//					getline(file, line);
//					active.start.y = atof(line.c_str());
//
//					getline(file, line, ' ');
//					active.end.x = atof(line.c_str());
//					getline(file, line);
//					active.end.y = atof(line.c_str());
//
//					getline(file, line, ' ');
//					active.p1.x = atof(line.c_str());
//					getline(file, line);
//					active.p1.y = atof(line.c_str());
//
//					getline(file, line, ' ');
//					active.p2.x = atof(line.c_str());
//					getline(file, line);
//					active.p2.y = atof(line.c_str());
//
//					rotateActEditor->actives.push_back(active);
//				}
//			}
//			else if (key == "Loop")
//			{
//				getline(file, line);
//				if (line == "true")
//				{
//					if (now == 0)
//					{
//						eyeActEditor->loop = true;
//					}
//					else if (now == 1)
//					{
//						rotateActEditor->loop = true;
//					}
//				}
//				else
//				{
//					if (now == 0)
//					{
//						eyeActEditor->loop = false;
//					}
//					else if (now == 1)
//					{
//						rotateActEditor->loop = false;
//					}
//				}
//			}
//			else if (key == "InitStart")
//			{
//				getline(file, line);
//				if (line == "true")
//				{
//					if (now == 0)
//					{
//						eyeActEditor->initStart = true;
//					}
//					else if(now == 1)
//					{
//						rotateActEditor->initStart = true;
//					}
//				}
//				else
//				{
//					if (now == 0)
//					{
//						eyeActEditor->initStart = false;
//					}
//					else if (now == 1)
//					{
//						rotateActEditor->initStart = false;
//					}
//				}
//			}
//			else if (key == "AddFlag")
//			{
//				getline(file, line);
//				if (line == "true")
//				{
//					if (now == 0)
//					{
//						eyeActEditor->addFlag = true;
//					}
//					else if (now == 1)
//					{
//						rotateActEditor->addFlag = true;
//					}
//				}
//				else
//				{
//					if (now == 0)
//					{
//						eyeActEditor->addFlag = false;
//					}
//					else if (now == 1)
//					{
//						rotateActEditor->addFlag = false;
//					}
//				}
//			}
//		}
//		file.close();
//
//		eyeActEditor->Init();
//		rotateActEditor->Init();
//
//		this->name = ActEditorName;
//
//		return true;
//	}
//
//	return false;	//失敗
//}

ActEditorsForCam::ActEditorsForCam(std::string Name, GameCamera* GameCam/*, bool Load*/) :name(Name)
{
	actEditors[EYE] = new ActivityEditor_3D(&GameCam->cameraInfo.eye);
	actEditors[TARGET] = new ActivityEditor_3D(&GameCam->target);

	////ロードするなら
	//if (Load)
	//{
	//	this->Load(Name);
	//}
	////新規生成なら
	//else
	//{
		this->name = Name;
	//}
}

ActEditorsForCam::~ActEditorsForCam()
{
	//this->SaveAE();
	for (int i = 0; i < 2; ++i)
	{
		MyFunc::SafetyDelete(actEditors[i]);
	}
}

void ActEditorsForCam::Init()
{
	InitStatus();

	for (int i = 0; i < 2; ++i)
	{
		actEditors[i]->Init();
	}
}

void ActEditorsForCam::Update()
{
	for (int i = 0; i < 2; ++i)
	{
		actEditors[i]->Update();
	}
}

void ActEditorsForCam::InitStatus()
{
	//初期位置セット
	for (int i = 0; i < 2; ++i)
	{
		actEditors[i]->UpdateFromFrame(0, 0);
	}
}

void ActEditorsForCam::AllOn()
{
	for (int i = 0; i < 2; ++i)
	{
		actEditors[i]->On();
	}
}

void ActEditorsForCam::Interrupt()
{
	for (int i = 0; i < 2; ++i)
	{
		actEditors[i]->Interrupt();
	}
}

//void ActEditorsForCam::LoadAE(std::string Name)
//{
//	//名前をリセット
//	name.clear();
//	eyeActEditor->ResetActives();
//	rotateActEditor->ResetActives();
//
//	if (this->Load(Name))
//	{
//		//成功したら初期位置セット
//		InitStatus();
//	}
//}
//
//void ActEditorsForCam::SaveAE()
//{
//	if (eyeActEditor != nullptr && rotateActEditor != nullptr)
//	{
//		//アクティブエディタの情報をレポート
//		std::string str[2];
//		if (eyeActEditor->Save(str[0]))
//		{
//			str[0] = "eye\n" + str[0];
//		}
//		if (rotateActEditor->Save(str[1]))
//		{
//			str[1] = "rotate\n" + str[1];
//		}
//
//		std::string text(str[0] + str[1]);
//		std::ofstream file;
//		file.open(ActivityEditor::DIRECTORY_PATH + ActEditorsForCam::DIRECTORY_PATH + this->name + EXTENSION);
//
//		if (!file.fail())
//		{
//			file << text;
//			file.close();
//		}
//	}
//}

bool ActEditorsForCam::Finish()
{
	for (int i = 0; i < 2; ++i)
	{
		if (actEditors[i]->loop)return false;
	}

	if (actEditors[0]->actives.empty() && actEditors[1]->actives.empty())return false;
	return actEditors[0]->Finish() && actEditors[1]->Finish();
}

void ActEditorsForCam::SetTarget(XMFLOAT3* Eye, XMFLOAT3* Target)
{
	actEditors[EYE]->SetTarget(Eye);
	actEditors[TARGET]->SetTarget(Target);
}

//int ActEditorsForCam::GetAllFrame()
//{
//	int frame = 0;
//
//	int activeFrame = 0;
//	for (int j = 0; j < eyeActEditor->actives.size(); ++j)
//	{
//		activeFrame += eyeActEditor->actives[j].frame;
//	}
//	if (frame < activeFrame)
//	{
//		frame = activeFrame;
//	}
//
//	activeFrame = 0;
//	for (int j = 0; j < rotateActEditor->actives.size(); ++j)
//	{
//		activeFrame += rotateActEditor->actives[j].frame;
//	}
//	if (frame < activeFrame)
//	{
//		frame = activeFrame;
//	}
//
//	return frame;
//}

bool ActEditorsForCam::Empty()
{
	return actEditors[0]->actives.empty()
		&& actEditors[1]->actives.empty();
}
