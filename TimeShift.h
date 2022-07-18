#pragma once
#include"Singleton.h"
class TimeShift : public Singleton<TimeShift>
{
	friend class Singleton<TimeShift>;
	int timer = 0;
	int span = -1;
public:
	~TimeShift() {};
	void TimeStop(int Span) { timer = Span; }
	void TimeSlow(int Time, int Span) { timer = Time; span = Span; }
	bool Update() 
	{
		if (timer)
		{
			timer--;

			if (span != -1)
			{
				if (timer == 0)
				{
					span = -1;
				}
				else if (timer % span == 0)
				{
					return true;
				}
			}

			return false;
		}
		return true;
	}
	bool NowTimeStop() { return timer; }
};