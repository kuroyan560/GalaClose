#pragma once
#include<string>
static enum ACT_EDITOR_TYPE { ACTIVE_POS, ACTIVE_SCALE, ACTIVE_ROTATION, ACTIVE_TYPE_NUM };

namespace ActivityEditor
{
	const std::string HEAD = "act";
	const std::string TYPE[ACTIVE_TYPE_NUM] = { "pos","scale","rotation" };
	const std::string DIRECTORY_PATH = "resource/ae/";
}
