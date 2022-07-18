#include "HowToOperate.h"
#include"UsersInput.h"

int HowToOperate::NOW_CONTROLLER = KEY_BOARD_MOUSE;
bool HowToOperate::CAN_OPERATE = true;

void HowToOperate::Update(bool CanOperate)
{
    CAN_OPERATE = CanOperate;
}