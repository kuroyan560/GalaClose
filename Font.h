#pragma once
#include<string>
#include<vector>
#include<map>

class Sprite_Normal;

struct FontInitData
{
	std::string dir;
	std::string fontFileName;	//�g���q�Ffnt
	std::wstring loadChars;	//�g�����������̗�(����Ă���)
};

class Font
{
	struct Character //�����̏��݂Ȃ�
	{
		//�e�N�X�`����ł̍��W
		int x;
		int y;
		//�����ƍ���
		int width;
		int height;
		//���̕�����`�悷��Ƃ��A�����W���ǂꂾ�����ɂ��炷�K�v�����邩
		int xadvance;
		//�����e�N�X�`��������摜�y�[�WID(SRVHandle)
		int page;
	};

	std::map<wchar_t, Character> loaded;	//���[�h�ϕ����̏��z��

	std::string name;
	std::vector<int>srvHandle;
	Sprite_Normal* sprite = nullptr; //�t�H���g�摜�̃X�v���C�g(�P�̃t�H���g�ɂ��P�����L)

	void LoadCharData(wchar_t& Chara,std::istringstream& LineStream);

public:
	Font(FontInitData& InitData);
};