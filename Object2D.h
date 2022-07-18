#pragma once
#include"Object3D.h"
#include<string>

class Object2D : public Object3D
{
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct Vertex
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	static const std::string PIPELINE_KEY;

public:
	static void CreatePipeline();

private:
	static const int VERTEX_COUNT = 4;
	static const enum { LB, RB, LT, RT };

	DirtyFlag sizeDirty;

	//���_�f�[�^�z��
	Vertex vertices[VERTEX_COUNT];
	D3D12_VERTEX_BUFFER_VIEW vbView;	//���_�o�b�t�@�r���[
	ComPtr<ID3D12Resource> vertBuff = nullptr;

	int texHandle;

	XMFLOAT2 anchorPoint;		//��_

	void VertMapping();

public:
	Object2D(string Name, XMFLOAT2 Size, XMFLOAT3 Pos, int TexHandle, XMFLOAT2 AnchorPoint, bool BillBoard = false, bool BillBoardY = false);
	XMFLOAT2 size;

	virtual void UniqueInit()override {};
	virtual void UniqueUpdate()override {};
	virtual void UniqueFinalize()override {};
	void Draw()override;

	//�X�v���C�g�e�N�X�`���͈͐ݒ�
	void ChangeTexExtent(XMFLOAT2 TexPos, XMFLOAT2 TexSize);
};

class Object2D_Divide : public Object2D
{
	//�摜�̕�����
	MyDef::int2 totalSplit;

	//���������Ƃ��̈ꖇ�̃T�C�Y
	MyDef::float2 splitSize;

	//���ݕ`�悵�����摜�̔ԍ�
	MyDef::int2 split = { 0,0 };
public:
	Object2D_Divide(std::string Name, MyDef::int2 SplitNum, MyDef::int2 SplitSize, int TexHandle, XMFLOAT2 Size,
		XMFLOAT3 Pos,XMFLOAT2 AnchorPoint, bool BillBoard = false, bool BillBoardY = false);

	//���Ԃ̉摜��`�悷�邩
	void SelectImgNum(int X, int Y);
};