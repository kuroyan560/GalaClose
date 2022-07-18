#pragma once
#include"GameObjectInterFace.h"

class Model_HSM_Normal;

class CenterPillar : public GameObjectInterFace
{
	std::shared_ptr<Model_HSM_Normal>model;

public:
	CenterPillar();
	void Init()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;
};