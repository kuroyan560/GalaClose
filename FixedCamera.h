#pragma once
#include"Camera.h"
class FixedCamera :public Camera
{
public:
	FixedCamera()	:Camera("FixedCamera") {};

	void Init()override {  };
	void Update()override {};
};