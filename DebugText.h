#pragma once
#include <wrl.h>
#include <string>
#include<stdio.h>
#include"Sprite.h"
#include"Singleton.h"

class DebugText : public Singleton<DebugText>
{
	friend class Singleton<DebugText>;
	DebugText();

	static const int MAX_CHAR_COUNT = 256;	//最大文字数
	static const int FONT_WIDTH = 16;
	static const int FONT_HEIGHT = 16;
	static const int FONT_LINE_COUNT = 16;

	//スプライトデータの配列
	//int spriteDataHandle[MAX_CHAR_COUNT];
	bool presentFlag[MAX_CHAR_COUNT];
	Sprite_Normal* sprites[MAX_CHAR_COUNT];

	//スプライトデータ配列の添字番号
	int spriteIndex = 0;

public:
	~DebugText();

	void Init();	//毎フレーム処理
	void Print(const std::string& Text, float X, float Y, float Scale = 1.0f);

	template <typename ... Args>
	void FormatPrint(float X, float Y, float Scale,const std::string& fmt, Args ... args);
};

template<typename ...Args>
inline void DebugText::FormatPrint(float X, float Y, float Scale, const std::string& fmt, Args ... args)
{
	size_t len = std::snprintf(nullptr, 0, fmt.c_str(), args ...);
	std::vector<char> buf(len + 1);
	std::snprintf(&buf[0], len + 1, fmt.c_str(), args ...);
	Print(std::string(&buf[0], &buf[0] + len), X, Y, Scale);
}
