#pragma once
#include"MyFunc.h"
#include"Singleton.h"
using namespace MyDef;

class UIMgr : public Singleton<UIMgr>
{
	friend class Singleton<UIMgr>;
	UIMgr();

public:
	~UIMgr();

	void Init();
	void Update();
	void Draw();
};