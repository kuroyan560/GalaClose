#pragma once
#include<vector>

class Scene
{
protected:
	Scene(const char* SceneName) :name(SceneName) {};

public:
	virtual ~Scene() {};
	const char* name;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
};

