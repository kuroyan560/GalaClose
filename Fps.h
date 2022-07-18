#pragma once
#include"Singleton.h"
#include <Windows.h>
#include <tchar.h>
#include <sstream>

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
class Fps : public Singleton<Fps>
{
	// �{���̓O���[�o���ɂ��Ȃ������ǂ�
	const float MIN_FREAM_TIME = 1.0f / 60;
	float frameTime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	// fps���擾����Ȃ�0�ŏ��������Ȃ��ƃS�~���������ă}�C�i�X����n�܂����肷��(�����m��Ȃ�)
	float fps = 0;

	Fps();
	friend class Singleton<Fps>;

public:
	bool Run();
};