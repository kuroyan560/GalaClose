#include "DebugText.h"
#include"SRVDescMgr.h"

DebugText::DebugText()
{
	int handle = SRVDescMgr::Instance()->CreateSRV("resource/img/debugfont.jpg");
	for (int i = 0; i < MAX_CHAR_COUNT; i++)
	{
		sprites[i] = new Sprite_Normal("DebugText", handle, XMFLOAT2{ (float)FONT_WIDTH,(float)FONT_HEIGHT }, { 0, 0 }, false, false, { 0,0 });
	}
	Init();
}

DebugText::~DebugText()
{
	for (int i = 0; i < MAX_CHAR_COUNT; i++)
	{
		delete sprites[i];
	}
}

void DebugText::Init()
{
	spriteIndex = 0;
	for (int i = 0; i < _countof(sprites); i++)
	{
		presentFlag[i] = false;
	}
}

void DebugText::Print(const std::string& Text, float X, float Y, float Scale)
{
	for (int i = 0; i < Text.size(); i++)
	{
		//�ő啶�����މ�
		if (MAX_CHAR_COUNT <= spriteIndex)
		{
			break;
		}

		//�ꕶ�������o���i��ASCII�R�[�h�ł������藧���Ȃ��j
		const unsigned char& character = Text[i];

		//ASCII�R�[�h�̂Q�i����΂����ԍ����v�Z
		int fontIndex = character;
		if (0x7f <= character)
		{
			fontIndex = 0;
		}

		int fontIndexX = fontIndex % FONT_LINE_COUNT;
		int fontIndexY = fontIndex / FONT_LINE_COUNT;

		//���W�v�Z
		sprites[spriteIndex]->GetComponent().transform.pos = { X + FONT_WIDTH * Scale * i,Y };
		sprites[spriteIndex]->ChangeSize({ FONT_WIDTH * Scale,FONT_HEIGHT * Scale });
		sprites[spriteIndex]->ChangeTexExtent(
			{ (float)(fontIndexX * FONT_WIDTH),(float)(fontIndexY * FONT_HEIGHT) },
			{ FONT_WIDTH, FONT_HEIGHT });

		//�\����Ԃ�
		presentFlag[spriteIndex] = true;

		//�������P�i�߂�
		spriteIndex++;
	}

	for (int i = 0; presentFlag[i] == true; i++)
	{
		sprites[i]->Draw();
	}
}