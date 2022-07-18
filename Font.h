#pragma once
#include<string>
#include<vector>
#include<map>

class Sprite_Normal;

struct FontInitData
{
	std::string dir;
	std::string fontFileName;	//拡張子：fnt
	std::wstring loadChars;	//使いたい文字の列挙(被っても良)
};

class Font
{
	struct Character //文字の所在など
	{
		//テクスチャ上での座標
		int x;
		int y;
		//横幅と高さ
		int width;
		int height;
		//次の文字を描画するとき、左座標をどれだけ横にずらす必要があるか
		int xadvance;
		//文字テクスチャがある画像ページID(SRVHandle)
		int page;
	};

	std::map<wchar_t, Character> loaded;	//ロード済文字の情報配列

	std::string name;
	std::vector<int>srvHandle;
	Sprite_Normal* sprite = nullptr; //フォント画像のスプライト(１つのフォントにつき１つを共有)

	void LoadCharData(wchar_t& Chara,std::istringstream& LineStream);

public:
	Font(FontInitData& InitData);
};