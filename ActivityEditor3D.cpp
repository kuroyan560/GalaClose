#include"ActivityEditor3D.h"
#include"MyFunc.h"
#include "Object3D.h"

std::vector<ActivityEditor_3D*> ActivityEditor_3D::staticActives;

void ActivityEditor_3D::ActivesInit()
{
	for (auto itr : staticActives)
	{
		itr->Init();
	}
}

void ActivityEditor_3D::ActivesUpdate()
{
	for (auto act : staticActives)
	{
		act->Update();
	}
}

void ActivityEditor_3D::Init()
{
	now = 0;
	if (initStart)
	{
		onFlag = true;
		frame = 0;
	}
	else
	{
		onFlag = false;
	}
	offsetFrame = 0;
}

void ActivityEditor_3D::Update()
{
	if (target != nullptr)
	{
		if (offsetFrame)
		{
			//中断フレーム数カウント
			//offsetFrame++;
		}
		if (onFlag)
		{
			if (now == actives.size() && loop)
			{
				now = 0;
				frame = 0;
				//stopOffset = 0;
			}
			if (!actives.empty() && now < actives.size())
			{
				//オフセットがインクリメント中は動きが止まる（Stop()呼び出し中）
				//startFrame += stopOffset;

				if (actives[now].frame == frame)
				{
					now++;
					frame = 0;
					//stopOffset = 0;
				}

				if (!Finish())
				{
					float t = (float)frame / (float)actives[now].frame;
					t = MyFunc::GetYFromXOnBezier(t, actives[now].p1, actives[now].p2, 12);	//最低12回以上刻み

					if (addFlag)
					{
						*target = MyFunc::AddXMFLOAT3(*target, MyFunc::Lerp(actives[now].start, actives[now].end, t));
					}
					else
					{
						*target = MyFunc::Lerp(actives[now].start, actives[now].end, t);
					}
				}
			}
		}
		frame++;
	}
}

void ActivityEditor_3D::CreateActive(int NowSelectMode)
{
	if (actives.empty())
	{
		actives.push_back(Active_3D(target, NowSelectMode));
	}
	else
	{
		int num = actives.size() - 1;
		actives.push_back(Active_3D(&actives[num]));
	}
}

void ActivityEditor_3D::DeleteActive(int Num)
{
	actives.erase(actives.begin() + Num);
}

ActivityEditor_3D::ActivityEditor_3D(XMFLOAT3* Target):target(Target)
{
	 staticActives.push_back(this);
}

ActivityEditor_3D::~ActivityEditor_3D()
{
	remove(staticActives.begin(), staticActives.end(), this);
}

bool ActivityEditor_3D::Save(std::string& TextStr)
{
	if (!actives.empty())
	{
		std::string str;
		static const int bufIndexMax = 32;
		char buf[bufIndexMax];

		for (int i = 0; i < actives.size(); i++)
		{
			TextStr += ActivityEditor::HEAD + '\n';

			//フレーム
			snprintf(buf, bufIndexMax, "%d", actives[i].frame);
			str = buf;
			TextStr += str + '\n';

			//始点
			snprintf(buf, bufIndexMax, "%f", actives[i].start.x);
			str = buf;
			TextStr += str + ' ';

			snprintf(buf, bufIndexMax, "%f", actives[i].start.y);
			str = buf;
			TextStr += str + ' ';

			snprintf(buf, bufIndexMax, "%f", actives[i].start.z);
			str = buf;
			TextStr += str;

			TextStr += '\n';

			//終点
			snprintf(buf, bufIndexMax, "%f", actives[i].end.x);
			str = buf;
			TextStr += str + ' ';

			snprintf(buf, bufIndexMax, "%f", actives[i].end.y);
			str = buf;
			TextStr += str + ' ';

			snprintf(buf, bufIndexMax, "%f", actives[i].end.z);
			str = buf;
			TextStr += str;

			TextStr += '\n';

			//制御点１
			snprintf(buf, bufIndexMax, "%f", actives[i].p1.x);
			str = buf;
			TextStr += str + ' ';

			snprintf(buf, bufIndexMax, "%f", actives[i].p1.y);
			str = buf;
			TextStr += str;

			TextStr += '\n';

			//制御点２
			snprintf(buf, bufIndexMax, "%f", actives[i].p2.x);
			str = buf;
			TextStr += str + ' ';

			snprintf(buf, bufIndexMax, "%f", actives[i].p2.y);
			str = buf;
			TextStr += str;

			TextStr += '\n';

			TextStr += '\n';
		}

		if (loop)
		{
			TextStr += "Loop\ntrue\n\n";
		}
		else
		{
			TextStr += "Loop\nfalse\n\n";
		}

		if (initStart)
		{
			TextStr += "InitStart\ntrue\n\n";
		}
		else
		{
			TextStr += "InitStart\nfalse\n\n";
		}

		if (addFlag)
		{
			TextStr += "AddFlag\ntrue\n\n";
		}
		else
		{
			TextStr += "AddFlag\nfalse\n\n";
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool ActivityEditor_3D::UpdateFromFrame(int ActiveIndex, int Frame)
{
	if (actives.empty())return false;

	if (0 <= ActiveIndex && ActiveIndex < actives.size())
	{
		float t = (float)Frame / (float)actives[ActiveIndex].frame;
		t = MyFunc::GetYFromXOnBezier(t, actives[ActiveIndex].p1, actives[ActiveIndex].p2, 12);	//最低12回以上刻み
		*target = MyFunc::Lerp(actives[ActiveIndex].start, actives[ActiveIndex].end, t);
	}
	if (actives[ActiveIndex].frame == Frame - 1)return true;
	return false;
}

void ActivityEditor_3D::UpdateFromFrame(int Frame)
{
	if (actives.empty())return;

	int frame = 0;
	int index = 0;
	int subFrame = 0;

	for (; index < actives.size(); ++index)
	{
		frame += actives[index].frame;
		if (Frame < frame)break;
		subFrame += actives[index].frame;
	}

	Frame -= subFrame;

	if (index != actives.size())
	{
		float t = (float)Frame / (float)actives[index].frame;
		t = MyFunc::GetYFromXOnBezier(t, actives[index].p1, actives[index].p2, 12);	//最低12回以上刻み
		*target = MyFunc::Lerp(actives[index].start, actives[index].end, t);
	}
	else
	{
		*target = actives.back().end;
	}
}

void ActivityEditor_3D::On()
{
	onFlag = true;
	if (!offsetFrame)
	{
		frame = 0;
	}
	else
	{
		frame = offsetFrame;
		offsetFrame = 0;
	}
}

void ActivityEditor_3D::Interrupt()
{
	onFlag = false;
	offsetFrame = frame;
}

bool ActivityEditor_3D::Finish()
{
	return actives.size() <= now;
}
