#pragma once
#include"Singleton.h"
#include <Windows.h>
#include <tchar.h>
#include <sstream>

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
class Fps : public Singleton<Fps>
{
	// 本当はグローバルにしない方が良い
	const float MIN_FREAM_TIME = 1.0f / 60;
	float frameTime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	// fpsを取得するなら0で初期化しないとゴミが混ざってマイナスから始まったりする(かも知れない)
	float fps = 0;

	Fps();
	friend class Singleton<Fps>;

public:
	bool Run();
};