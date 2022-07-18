#include "ActivityEditor1D.h"
#include"MyFunc.h"

const std::string ActivityEditor_1D::EXTENSION = ".ae1d";
std::vector<ActivityEditor_1D*>ActivityEditor_1D::staticActives;

void ActivityEditor_1D::ActivesInit()
{
	for (auto itr : staticActives)
	{
		itr->Init();
	}
}

void ActivityEditor_1D::ActivesUpdate()
{
	for (auto itr : staticActives)
	{
		itr->Update();
	}
}

void ActivityEditor_1D::Init()
{
	now = 0;
	if (initStart)
	{
		onFlag = true;
		frame = 0;
	}
	offsetFrame = 0;
}

void ActivityEditor_1D::Update()
{
	if (target != nullptr)
	{
		if (offsetFrame)
		{
			//中断フレーム数カウント
			offsetFrame++;
		}
		if (onFlag)
		{
			if (now == actives.size() && loop)
			{
				now = 0;
				frame = 0;
			}
			if (!actives.empty() && now < actives.size())
			{
				if (actives[now].frame == frame)
				{
					now++;
					frame = 0;
				}

				if (now < actives.size())
				{
					float t = (float)frame / (float)actives[now].frame;
					t = MyFunc::GetYFromXOnBezier(t, actives[now].p1, actives[now].p2, 12);	//最低12回以上刻み
					*target = MyFunc::Lerp(actives[now].start, actives[now].end, t);
				}
			}
		}
		frame++;
	}
}

void ActivityEditor_1D::CreateActive(int NowSelectMode)
{
	if (actives.empty())
	{
		actives.push_back(Active_1D(target,NowSelectMode));
	}
	else
	{
		int num = actives.size() - 1;
		actives.push_back(Active_1D(actives[num].end, actives[num].frame));
	}
}

void ActivityEditor_1D::DeleteActive(int Num)
{
	actives.erase(actives.begin() + Num);
}

bool ActivityEditor_1D::Save(std::string& TextStr)
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
			snprintf(buf, bufIndexMax, "%f", actives[i].start);
			str = buf;
			TextStr += str;

			TextStr += '\n';

			//終点
			snprintf(buf, bufIndexMax, "%f", actives[i].end);
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
	}
	else
	{
		return false;
	}
	return true;
}

bool ActivityEditor_1D::UpdateFromFrame(int ActiveIndex, int Frame)
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

void ActivityEditor_1D::On()
{
	onFlag = true;
	if (!offsetFrame)
	{
		frame = 0;
	}
	else
	{
		frame += offsetFrame;
		offsetFrame = 0;
	}
}

void ActivityEditor_1D::Interrupt()
{
	onFlag = false;
	offsetFrame++;
}
