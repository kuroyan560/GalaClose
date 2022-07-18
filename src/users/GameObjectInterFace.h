#pragma once
#include<vector>
#include<memory>
struct GameObjectInterFace
{
	static std::vector<GameObjectInterFace*>objs;

	GameObjectInterFace() { objs.emplace_back(this); }
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
};