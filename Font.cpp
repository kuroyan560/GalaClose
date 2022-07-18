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
		key.erase(std::remove(key.begin(), key.end(), ' '), key.end());	//空白文字の削除

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
	std::sort(loadStr.begin(), loadStr.end());	//文字コード順にソート
	loadStr.erase(unique(loadStr.begin(), loadStr.end()), loadStr.end());	//連続で重複している文字を削除

	int index = 0;	//現在探索中の文字インデックス

	std::ifstream file;
	file.open((InitData.dir + InitData.fontFileName).c_str());
	if (file.fail())
	{
		WinApp::Instance()->CreateMessageWindow("ERROR", "フォントファイル(.fnt)のオープンに失敗");
		DebugBreak();
	}

	std::string line;
	getline(file, line, '\"');
	getline(file, line, '\"');
	name = line;	//フォント名

	getline(file, line);

	while (getline(file, line) && index < loadStr.size())	//EOFでない && 使う文字の情報を受け取れきれていない
	{
		std::istringstream line_stream(line);
		std::string key;
		getline(line_stream, key, '=');

		//文字テクスチャ画像読み込み
		if (key == "page id")
		{
			std::string pageName;
			getline(line_stream, pageName, '\"');
			getline(line_stream, pageName, '\"');
			srvHandle.emplace_back(SRVDescMgr::Instance()->CreateSRV((InitData.dir + pageName).c_str()));
		}
		//文字情報
		else if (key == "char id")
		{
			int id = 0;
			line_stream >> id;

			//欲しい文字と一致したらデータ読み込み
			if (id == loadStr[index])
			{
				LoadCharData(loadStr[index], line_stream);
				++index; //次の文字へ
			}
		}
	}
	file.close();
}