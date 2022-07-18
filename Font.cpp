#include "Font.h"
#include"SRVDescMgr.h"
#include"Sprite.h"
#include"WinApp.h"
#include <fstream>
#include <sstream>
#include<iostream>
#include<algorithm>

void Font::LoadCharData(wchar_t& Chara, std::istringstream& LineStream)
{
	std::string key;
	while (!LineStream.eof())
	{
		getline(LineStream, key, '=');
		key.erase(std::remove(key.begin(), key.end(), ' '), key.end());	//�󔒕����̍폜

		if (key == "x")
		{
			LineStream >> loaded[Chara].x;
		}
		else if (key == "y")
		{
			LineStream >> loaded[Chara].y;
		}
		else if (key == "width")
		{
			LineStream >> loaded[Chara].width;
		}
		else if (key == "height")
		{
			LineStream >> loaded[Chara].height;
		}
		else if (key == "xadvance")
		{
			LineStream >> loaded[Chara].xadvance;
		}
		else if (key == "page")
		{
			LineStream >> loaded[Chara].page;
		}
		else
		{
			int dump;
			LineStream >> dump;
		}
	}
}

Font::Font(FontInitData& InitData)
{
	wstring& loadStr = InitData.loadChars;
	std::sort(loadStr.begin(), loadStr.end());	//�����R�[�h���Ƀ\�[�g
	loadStr.erase(unique(loadStr.begin(), loadStr.end()), loadStr.end());	//�A���ŏd�����Ă��镶�����폜

	int index = 0;	//���ݒT�����̕����C���f�b�N�X

	std::ifstream file;
	file.open((InitData.dir + InitData.fontFileName).c_str());
	if (file.fail())
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "�t�H���g�t�@�C��(.fnt)�̃I�[�v���Ɏ��s");
		DebugBreak();
	}

	std::string line;
	getline(file, line, '\"');
	getline(file, line, '\"');
	name = line;	//�t�H���g��

	getline(file, line);

	while (getline(file, line) && index < loadStr.size())	//EOF�łȂ� && �g�������̏����󂯎�ꂫ��Ă��Ȃ�
	{
		std::istringstream line_stream(line);
		std::string key;
		getline(line_stream, key, '=');

		//�����e�N�X�`���摜�ǂݍ���
		if (key == "page id")
		{
			std::string pageName;
			getline(line_stream, pageName, '\"');
			getline(line_stream, pageName, '\"');
			srvHandle.emplace_back(SRVDescMgr::Instance()->CreateSRV((InitData.dir + pageName).c_str()));
		}
		//�������
		else if (key == "char id")
		{
			int id = 0;
			line_stream >> id;

			//�~���������ƈ�v������f�[�^�ǂݍ���
			if (id == loadStr[index])
			{
				LoadCharData(loadStr[index], line_stream);
				++index; //���̕�����
			}
		}
	}
	file.close();
}