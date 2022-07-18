#pragma once
//‘€ì•û–@‚Ì’è‹`
static class HowToOperate
{
	enum { KEY_BOARD_MOUSE, X_BOX_CONTROLLER };
	static int NOW_CONTROLLER;

	static bool CAN_OPERATE;

public:
	static void Update(bool CanOperate);
};

