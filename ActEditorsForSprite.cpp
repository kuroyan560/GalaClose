#include "ActEditorsForSprite.h"
#include <fstream>
#include <sstream>
#include<iostream>
#include<stdlib.h>
#include"MyFunc.h"
#include"WinApp.h"

const std::string ActEditors_Sprite::EXTENSION = ".aesp";

bool ActEditors_Sprite::Load(std::string& ActEditorName)
{
	//フレーム
	std::string pass;
	pass = ActivityEditor::DIRECTORY_PATH + ActEditorName;

	std::ifstream file;
	file.open(pass + EXTENSION);
	if (!file.fail())
	{
		std::string line;
		int now;
		ActivityEditor_2D* editor_2d = nullptr;

		while (getline(file, line))
		{
			std::istringstream line_stream(line);

			std::string key;
			getline(line_stream, key, ' ');

			if (key == "pos")
			{
				now = ACTIVE_POS;
				editor_2d = actEditor_pos;
			}
			else if (key == "scale")
			{
				now = ACTIVE_SCALE;
				editor_2d = actEditor_scale;
			}
			else if (key == "rotation")
			{
				now = ACTIVE_ROTATION;
			}
			else if (key == "act")
			{
				//posかscale(XMFLOAT2)
				if (now == ACTIVE_POS || now == ACTIVE_SCALE)
				{
					Active_2D active;
					//フレーム
					getline(file, line);
					active.frame = atoi(line.c_str());

					//始点
					getline(file, line, ' ');
					active.start.x = atof(line.c_str());
					getline(file, line, ' ');
					active.start.y = atof(line.c_str());

					//終点
					getline(file, line, ' ');
					active.end.x = atof(line.c_str());
					getline(file, line, ' ');
					active.end.y = atof(line.c_str());

					//ベジエ制御点１
					getline(file, line, ' ');
					active.p1.x = atof(line.c_str());
					getline(file, line);
					active.p1.y = atof(line.c_str());

					//ベジエ制御点２
					getline(file, line, ' ');
					active.p2.x = atof(line.c_str());
					getline(file, line);
					active.p2.y = atof(line.c_str());

					editor_2d->actives.push_back(active);
				}
				//rotate(float)
				else
				{
					Active_1D active;

					//フレーム
					getline(file, line);
					active.frame = atoi(line.c_str());

					//始点
					getline(file, line, ' ');
					active.start = atof(line.c_str());

					//終点
					getline(file, line, ' ');
					active.end = atof(line.c_str());

					//ベジエ制御点１
					getline(file, line, ' ');
					active.p1.x = atof(line.c_str());
					getline(file, line);
					active.p1.y = atof(line.c_str());

					//ベジエ制御点２
					getline(file, line, ' ');
					active.p2.x = atof(line.c_str());
					getline(file, line);
					active.p2.y = atof(line.c_str());

					actEditor_rotate->actives.push_back(active);
				}
			}
			else if (key == "Loop")
			{
				getline(file, line);
				if (line == "true")
				{
					//posかscale(XMFLOAT2)
					if (now == ACTIVE_POS || now == ACTIVE_SCALE)
					{
						editor_2d->loop = true;
					}
					//rotate(float)
					else
					{
						actEditor_rotate->loop = true;
					}

				}
				else
				{
					//posかscale(XMFLOAT2)
					if (now == ACTIVE_POS || now == ACTIVE_SCALE)
					{
						editor_2d->loop = false;
					}
					//rotate(float)
					else
					{
						actEditor_rotate->loop = false;
					}
				}
			}
			else if (key == "InitStart")
			{
				getline(file, line);
				if (line == "true")
				{
					//posかscale(XMFLOAT2)
					if (now == ACTIVE_POS || now == ACTIVE_SCALE)
					{
						editor_2d->initStart = true;
					}
					//rotate(float)
					else
					{
						actEditor_rotate->initStart = true;
					}

				}
				else
				{
					//posかscale(XMFLOAT2)
					if (now == ACTIVE_POS || now == ACTIVE_SCALE)
					{
						editor_2d->initStart = false;
					}
					//rotate(float)
					else
					{
						actEditor_rotate->initStart = false;
					}
				}
			}
		}
		file.close();

		actEditor_pos->Init();
		actEditor_scale->Init();
		actEditor_rotate->Init();

		return true;
	}

	return false;	//失敗
}

ActEditors_Sprite::ActEditors_Sprite(std::string Name, XMFLOAT2* Pos, XMFLOAT2* Scale, float* Rotation) :name(Name)
{
	actEditor_pos = new ActivityEditor_2D(Pos);
	actEditor_scale = new ActivityEditor_2D(Scale);
	actEditor_rotate = new ActivityEditor_1D(Rotation);
}

ActEditors_Sprite::~ActEditors_Sprite()
{
	this->SaveAE();

	MyFunc::SafetyDelete(this->actEditor_pos);
	MyFunc::SafetyDelete(this->actEditor_scale);
	MyFunc::SafetyDelete(this->actEditor_rotate);
}

void ActEditors_Sprite::InitStatus()
{
	//初期位置セット
	if (actEditor_pos != nullptr)
	{
		actEditor_pos->UpdateFromFrame(0, 0);
	}
	if (actEditor_scale != nullptr)
	{
		actEditor_scale->UpdateFromFrame(0, 0);
	}
	if (actEditor_rotate != nullptr)
	{
		actEditor_rotate->UpdateFromFrame(0, 0);
	}
}

void ActEditors_Sprite::AllOn()
{
	if (actEditor_pos != nullptr)
	{
		actEditor_pos->On();
	}
	if (actEditor_scale != nullptr)
	{
		actEditor_scale->On();
	}
	if (actEditor_rotate != nullptr)
	{
		actEditor_rotate->On();
	}
}

void ActEditors_Sprite::Interrupt()
{
	if (actEditor_pos != nullptr)
	{
		actEditor_pos->Interrupt();
	}
	if (actEditor_scale != nullptr)
	{
		actEditor_scale->Interrupt();
	}
	if (actEditor_rotate != nullptr)
	{
		actEditor_rotate->Interrupt();
	}
}

void ActEditors_Sprite::LoadAE(std::string Name)
{
	//名前をリセット
	name.clear();
	actEditor_pos->ResetActives();
	actEditor_scale->ResetActives();
	actEditor_rotate->ResetActives();

	if (this->Load(Name))
	{
		//成功したら初期位置セット
		InitStatus();
		//名前を保存
		name = Name;
	}
}

void ActEditors_Sprite::SaveAE()
{
	if (MyFunc::ExistFile(ActivityEditor::DIRECTORY_PATH + this->name + ActEditors_Sprite::EXTENSION))
	{
		bool overWriting;
		WinApp::Instance()->CreateMessageWindow("Warning", "同名のファイルあるけど上書きしていい？" + this->name, &overWriting);

		if (overWriting == false)
		{
			WinApp::Instance()->CreateMessageWindow("OK", "セーブしません。違うファイル名で試してね");
			return;	//何もせず処理終了
		}
	}

	if (this->actEditor_pos != nullptr && this->actEditor_scale != nullptr && this->actEditor_rotate != nullptr)
	{
		//アクティブエディタの情報をレポート
		std::string str[3];
		if (this->actEditor_pos->Save(str[ACTIVE_POS]))
		{
			str[ACTIVE_POS] = ActivityEditor::TYPE[ACTIVE_POS] + '\n' + str[ACTIVE_POS];
		}
		if (this->actEditor_scale->Save(str[ACTIVE_SCALE]))
		{
			str[ACTIVE_SCALE] = ActivityEditor::TYPE[ACTIVE_SCALE] + '\n' + str[ACTIVE_SCALE];
		}
		if (this->actEditor_rotate->Save(str[ACTIVE_ROTATION]))
		{
			str[ACTIVE_ROTATION] = ActivityEditor::TYPE[ACTIVE_ROTATION] + '\n' + str[ACTIVE_ROTATION];
		}

		std::string text(str[ACTIVE_POS] + str[ACTIVE_SCALE] + str[ACTIVE_ROTATION]);
		if (!text.empty())
		{
			std::ofstream file;
			file.open(ActivityEditor::DIRECTORY_PATH + this->name + EXTENSION);
			if (!file.fail())
			{
				file << text;
				file.close();
			}
		}
		//中身空っぽのとき何もしない
		else
		{
			WinApp::Instance()->CreateMessageWindow("ERROR", "中身空っぽだったから保存しないよ！");
		}
	}
}
