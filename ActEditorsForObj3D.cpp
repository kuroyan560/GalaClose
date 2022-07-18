#include"ActEditorsForObj3D.h"
#include <fstream>
#include <sstream>
#include<iostream>
#include<stdlib.h>
#include"MyFunc.h"
#include"WinApp.h"

const std::string ActEditors_Obj3D::EXTENSION = ".ae3d";

bool ActEditors_Obj3D::Load(std::string& ActEditorName, std::string* DirPass)
{
	std::string pass;
	if (DirPass == nullptr)
	{
		pass = ActivityEditor::DIRECTORY_PATH + ActEditorName;
	}
	else
	{
		pass = ActivityEditor::DIRECTORY_PATH + DirPass->c_str() + ActEditorName;
	}

	if (!MyFunc::ExistFile(pass + ActEditors_Obj3D::EXTENSION))
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "モーションエディタ生成時にロードしようとしたけどデータ無かったよ");
		delete this;
	}

	std::ifstream file;
	file.open(pass + EXTENSION);
	if (!file.fail())
	{
		std::string line;
		int now;

		while (getline(file, line))
		{
			std::istringstream line_stream(line);

			std::string key;
			getline(line_stream, key, ' ');

			if (key == "pos")
			{
				now = 0;
			}
			else if (key == "scale")
			{
				now = 1;
			}
			else if (key == "rotation")
			{
				now = 2;
			}
			else if (key == "act")
			{
				Active_3D active;

				getline(file, line);
				active.frame = atoi(line.c_str());

				getline(file, line, ' ');
				active.start.x = atof(line.c_str());
				getline(file, line, ' ');
				active.start.y = atof(line.c_str());
				getline(file, line);
				active.start.z = atof(line.c_str());

				getline(file, line, ' ');
				active.end.x = atof(line.c_str());
				getline(file, line, ' ');
				active.end.y = atof(line.c_str());
				getline(file, line);
				active.end.z = atof(line.c_str());

				getline(file, line, ' ');
				active.p1.x = atof(line.c_str());
				getline(file, line);
				active.p1.y = atof(line.c_str());

				getline(file, line, ' ');
				active.p2.x = atof(line.c_str());
				getline(file, line);
				active.p2.y = atof(line.c_str());

				activityEditor[now]->actives.push_back(active);
			}
			else if (key == "Loop")
			{
				getline(file, line);
				if (line == "true")
				{
					activityEditor[now]->loop = true;
				}
				else
				{
					activityEditor[now]->loop = false;
				}
			}
			else if (key == "InitStart")
			{
				getline(file, line);
				if (line == "true")
				{
					activityEditor[now]->initStart = true;
				}
				else
				{
					activityEditor[now]->initStart = false;
				}
			}
			else if (key == "AddFlag")
			{
				getline(file, line);
				if (line == "true")
				{
					activityEditor[now]->addFlag = true;
				}
				else
				{
					activityEditor[now]->addFlag = false;
				}
			}
		}
		file.close();

		activityEditor[0]->Init();
		activityEditor[1]->Init();
		activityEditor[2]->Init();

		this->name = ActEditorName;

		return true;
	}

	return false;	//失敗
}

ActEditors_Obj3D::ActEditors_Obj3D(std::string Name, XMFLOAT3* Pos, XMFLOAT3* Scale, XMFLOAT3* Rotation, bool Load, std::string* DirPass) :name(Name)
{
	activityEditor[ACTIVE_POS] = new ActivityEditor_3D(Pos);
	activityEditor[ACTIVE_SCALE] = new ActivityEditor_3D(Scale);
	activityEditor[ACTIVE_ROTATION] = new ActivityEditor_3D(Rotation);

	//ロードするなら
	if (Load)
	{
		this->Load(Name, DirPass);
	}
	//新規生成なら
	else
	{
		this->name = Name;
	}
}

ActEditors_Obj3D::ActEditors_Obj3D(std::string Name, HSM::Transform& Transform, bool Load, std::string* DirPass)
{
	activityEditor[ACTIVE_POS] = new ActivityEditor_3D(&Transform.pos);
	activityEditor[ACTIVE_SCALE] = new ActivityEditor_3D(&Transform.scale);
	activityEditor[ACTIVE_ROTATION] = new ActivityEditor_3D(&Transform.rotate);

	//ロードするなら
	if (Load)
	{
		this->Load(Name, DirPass);
	}
	//新規生成なら
	else
	{
		this->name = Name;
	}
}

ActEditors_Obj3D::~ActEditors_Obj3D()
{
	//this->SaveAE();

	for (int i = 0; i < 3; i++)MyFunc::SafetyDelete(this->activityEditor[i]);
}

void ActEditors_Obj3D::Init()
{
	InitStatus();
	for (int i = 0; i < ACTIVE_TYPE_NUM; i++)
	{
		if (activityEditor[i] != nullptr)
		{
			activityEditor[i]->Init();
		}
	}
}

void ActEditors_Obj3D::InitStatus()
{
	//初期位置セット
	for (int i = 0; i < ACTIVE_TYPE_NUM; i++)
	{
		if (activityEditor[i] != nullptr)
		{
			activityEditor[i]->UpdateFromFrame(0, 0);
		}
	}
}

void ActEditors_Obj3D::AllOn()
{
	for (int i = 0; i < 3; i++)
	{
		activityEditor[i]->On();
	}
}

void ActEditors_Obj3D::Interrupt()
{
	for (int i = 0; i < 3; i++)
	{
		if (activityEditor[i] != nullptr)
		{
			activityEditor[i]->Interrupt();
		}
	}
}

void ActEditors_Obj3D::LoadAE(std::string Name)
{
	//名前をリセット
	name.clear();
	for (int i = 0; i < ACTIVE_TYPE_NUM; i++)
	{
		activityEditor[i]->ResetActives();
	}

	if (this->Load(Name))
	{
		//成功したら初期位置セット
		InitStatus();
	}
}

void ActEditors_Obj3D::SaveAE(std::string* DirPass)
{
	//if (MyFunc::ExistFile(ActivityEditor::DIRECTORY_PATH + this->name + ActEditors_Obj3D::EXTENSION))
	//{
	//	bool overWriting;
	//	WinApp::Instance()->CreateMessageWindow("Warning", "同名のファイルあるけど上書きしていい？" + this->name, &overWriting);

	//	if (overWriting == false)
	//	{
	//		WinApp::Instance()->CreateMessageWindow("OK", "セーブしません。違うファイル名で試してね");
	//		return;	//何もせず処理終了
	//	}
	//}

	if (this->activityEditor[0] != nullptr && this->activityEditor[1] != nullptr && this->activityEditor[2] != nullptr)
	{
		//アクティブエディタの情報をレポート
		std::string str[3];
		for (int i = 0; i < 3; i++)
		{
			if (this->activityEditor[i]->Save(str[i]))
			{
				str[i] = ActivityEditor::TYPE[i] + '\n' + str[i];
			}
		}

		std::string text(str[0] + str[1] + str[2]);
		std::ofstream file;
		if (DirPass == nullptr)
		{
			file.open(ActivityEditor::DIRECTORY_PATH + this->name + EXTENSION);
		}
		else
		{
			file.open(ActivityEditor::DIRECTORY_PATH + DirPass->c_str() + this->name + EXTENSION);
		}
		if (!file.fail())
		{
			file << text;
			file.close();
		}
	}
}

bool ActEditors_Obj3D::Finish()
{
	if (activityEditor[ACTIVE_POS]->loop)return false;
	if (activityEditor[ACTIVE_SCALE]->loop)return false;
	if (activityEditor[ACTIVE_ROTATION]->loop)return false;
	return activityEditor[ACTIVE_POS]->Finish() && activityEditor[ACTIVE_SCALE]->Finish() && activityEditor[ACTIVE_ROTATION]->Finish();
}

void ActEditors_Obj3D::SetTarget(XMFLOAT3* Pos, XMFLOAT3* Scale, XMFLOAT3* Rotation)
{
	activityEditor[ACTIVE_POS]->SetTarget(Pos);
	activityEditor[ACTIVE_SCALE]->SetTarget(Scale);
	activityEditor[ACTIVE_ROTATION]->SetTarget(Rotation);
}

void ActEditors_Obj3D::SetTarget(HSM::Transform& Transform)
{
	activityEditor[ACTIVE_POS]->SetTarget(&Transform.pos);
	activityEditor[ACTIVE_SCALE]->SetTarget(&Transform.scale);
	activityEditor[ACTIVE_ROTATION]->SetTarget(&Transform.rotate);
}
