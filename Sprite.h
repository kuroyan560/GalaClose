#pragma once
#include<wrl.h>

#include <d3d12.h>
#include"d3dx12.h"
#include<string>

#include <DirectXMath.h>
using namespace DirectX;

#include"DirtyFlag.h"
#include"ActEditorsForSprite.h"

#include"MyFunc.h"

namespace SPRITE
{
	static const enum ROOT_PARAM
	{
		CONST_BUFF,
		TEXTURE
	};
	//���_�f�[�^
	struct Vertex
	{
		XMFLOAT3 pos;	//xyz���W
		XMFLOAT2 uv;	//uv���W
	};
	//�萔�o�b�t�@�f�[�^
	struct Color
	{
		XMFLOAT4 color = { 1,1,1,1 };
	};
	struct Transform
	{
		XMFLOAT2 pos = { 0,0 };
		XMFLOAT2 rotate = { 0,0 }; //Y��Z( x �� z �Ƃ��Ĉ���) //�x��
		XMFLOAT2 scale = { 1,1 };
		XMMATRIX mat = XMMatrixIdentity();
	};
	struct Const
	{
		Color color;
		XMMATRIX mat = XMMatrixIdentity();
	};
}

class Sprite_Component;

//���_
class Sprite
{
protected:
	friend void ImguiCommand_Master();
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const std::string PIPELINE_KEY;
	//�����A����A�E���A�E��
	static const enum { LB, LT, RB, RT };

	static std::vector<Sprite*>STATIC_SPRITES;
	static int MOUSE_IMG_ID;	//�}�E�X�X�v���C�g��ID
public:
	static void CreatePipeline();
	//�ŏ���Hit�������̂�Ԃ��i�T�����ǂ�ID����n�߂邩�j
	static Sprite* HitCheckAll(XMFLOAT2 ScreenPos, int StartID);
	static void RegisterSprite(Sprite* Tmp, bool ForCorsor = false);	//�N���X�̐ÓI�z��ɓo�^
	
protected:
	int id;

	//���\�[�X�֌W
	static const int INDEX_NUM = 4;
	SPRITE::Vertex vertices[INDEX_NUM];
	ComPtr<ID3D12Resource> vertBuff = nullptr;	//���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView;			//���_�o�b�t�@�r���[

	std::string name;
	XMFLOAT2 anchorPoint;		//��_
	XMFLOAT2 size;

	void VertMapping();
public:
	Sprite(std::string Name, int Handle, XMFLOAT2 Size,bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);
	Sprite(std::string Name, int Handle, MyDef::int2 Size,bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);

	bool flipX;	//���E���]
	bool flipY;	//�㉺���]
	bool draw = true;
	int texHandle;

	//�X�v���C�g�̎��ʔԍ�
	int GetID() { return id; }

	//�w�肵���ꏊ�ɃX�v���C�g�����邩�i�����蔻��j
	virtual bool HitCheck(XMFLOAT2 ScreenPos);

	//�X�v���C�g�e�N�X�`���͈͐ݒ�
	void ChangeTexExtent(XMFLOAT2 TexPos, XMFLOAT2 TexSize);

	//�T�C�Y�ύX
	void ChangeSize(XMFLOAT2 Size);

	//�Q�Ƃ���e�N�X�`���n���h���ύX
	void ChangeTexHandle(int Handle)
	{
		texHandle = Handle;
	}

	void PreDraw();
	void Draw(Sprite_Component* Component);
};

class Sprite_Component
{
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Resource> constBuff = nullptr;	//�萔�o�b�t�@
	DirtyFlag transformDirty;
	DirtyFlag colorDirty;

public:
	SPRITE::Color color;
	SPRITE::Transform transform;

	Sprite_Component();

	void DirtyRegister();

	void ColorMapping();
	void MatMapping();
	void SetConstBuff();
};

class Sprite_Normal : public Sprite
{
	Sprite_Component component;
public:
	Sprite_Normal(std::string Name, int Handle, XMFLOAT2 Size,
		XMFLOAT2 Pos, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);	//�J�[�\���p�̂��̂̓X�v���C�g�I���ł��Ȃ悤�ɂ���
	Sprite_Normal(std::string Name, int Handle, MyDef::int2 Size,
		XMFLOAT2 Pos, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);	//�J�[�\���p�̂��̂̓X�v���C�g�I���ł��Ȃ悤�ɂ���
	~Sprite_Normal() {};

	void Draw();

	//�R���|�[�l���g�̃Q�b�^
	Sprite_Component& GetComponent() { return component; }

	//�w�肵���ꏊ�ɃX�v���C�g�����邩�i�����蔻��j
	bool HitCheck(XMFLOAT2 ScreenPos)override;

	void ConstMapping();

	//�A���t�@�l�擾
	float& GetAlpha() { return component.color.color.w; }
	//�F�x�����␳�i�O�`�Q�T�T�j
	void ChangeColor(int R, int G, int B);
};

class Sprite_Divide :public Sprite_Normal
{
	//�摜�̕�����
	MyDef::int2 totalSplit;

	//���������Ƃ��̈ꖇ�̃T�C�Y
	MyDef::float2 splitSize;

	//���ݕ`�悵�����摜�̔ԍ�
	MyDef::int2 split = { 0,0 };
public:
	Sprite_Divide(std::string Name, MyDef::int2 SplitNum, MyDef::int2 SplitSize, int Handle, XMFLOAT2 Size,
		XMFLOAT2 Pos, bool FlipX, bool FlipY, XMFLOAT2 AnchorPoint);

	//���Ԃ̉摜��`�悷�邩
	void SelectImgNum(int X, int Y);
};