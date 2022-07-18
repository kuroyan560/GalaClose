#pragma once
#include<string>
#include<DirectXMath.h>
#include<forward_list>
#include<vector>
using namespace DirectX;
using namespace std;

namespace MyDef
{
	using float3 = XMFLOAT3;
	using float2 = XMFLOAT2;
	using float4 = XMFLOAT4;
	using matrix = XMMATRIX;

	struct int2
	{
		int x;
		int y;
	};

	//vector�Ń|�C���^�������Ƃɂ���Ē��ڃA�N�Z�X�o����悤�ɂ���(�P�v�f���ǂ̃T�C�Y�ł����ڃA�N�Z�X)
	template<typename T>
	class wrap_forward_list
	{
		std::forward_list<T>objs;
		std::vector<T*>pointer;
	public:
		std::forward_list<T>begin()
		{
			return objs.begin();
		}
		std::forward_list<T>end()
		{
			return objs.end();
		}
		void emplace_back()
		{
			objs.emplace_front();
			pointer.emplace_back(&(*objs.begin()));
		}
		T* operator[](int n) { return pointer[n]; }
		int size()
		{
			return pointer.size();
		}
		bool empty()
		{
			return pointer.empty();
		}
	};
}

namespace MyFunc
{
	template<typename T>
	void SafetyDelete(T* Ptr)
	{
		if (Ptr != nullptr)delete Ptr;
		Ptr = nullptr;
	}
	wstring GetWideStrFromStr(const string& Str);

	string GetExtension(const string& Path);

	XMFLOAT4 Lerp(XMFLOAT4 start, XMFLOAT4 end, float t);
	XMFLOAT3 Lerp(XMFLOAT3 start, XMFLOAT3 end, float t);
	XMFLOAT2 Lerp(XMFLOAT2 start, XMFLOAT2 end, float t);
	float Lerp(float start, float end, float t);

	XMFLOAT4 Liner(XMFLOAT4 start, XMFLOAT4 end, float t);
	XMFLOAT3 Liner(XMFLOAT3 start, XMFLOAT3 end, float t);
	XMFLOAT2 Liner(XMFLOAT2 start, XMFLOAT2 end, float t);
	float Liner(float start, float end, float t);

	float GetDistXMFLOAT3(XMFLOAT3 from, XMFLOAT3 to);

	XMFLOAT2 NormalizeXMFLOAT2(XMFLOAT2 Tmp);
	XMFLOAT3 NormalizeXMFLOAT3(XMFLOAT3 Tmp);

	XMFLOAT2 AddXMFLOAT2(XMFLOAT2 a, XMFLOAT2 b);
	XMFLOAT2 SubXMFLOAT2(XMFLOAT2 Target, XMFLOAT2 Value);
	XMFLOAT2 MulXMFLOAT2(XMFLOAT2 a, XMFLOAT2 b);
	XMFLOAT2 MulXMFLOAT2(XMFLOAT2 Target, float Value);
	XMFLOAT2 DivXMFLOAT2(XMFLOAT2 Target, XMFLOAT2 Value);

	XMFLOAT3 AddXMFLOAT3(XMFLOAT3 a, XMFLOAT3 b);
	XMFLOAT3 SubXMFLOAT3(XMFLOAT3 Target, XMFLOAT3 Value);
	XMFLOAT3 MulXMFLOAT3(XMFLOAT3 a, XMFLOAT3 b);
	XMFLOAT3 MulXMFLOAT3(XMFLOAT3 Target, float Value);
	XMFLOAT3 DivXMFLOAT3(XMFLOAT3 Target, float Value);

	XMFLOAT2 GetCenterXMFLOAT2(XMFLOAT2 From, XMFLOAT2 To);
	XMFLOAT3 GetCenterXMFLOAT3(XMFLOAT3 From, XMFLOAT3 To);

	//�Q�_��ʂ钼���̌X��
	float GetLineSlope(XMFLOAT2 From, XMFLOAT2 To);

	XMFLOAT2 ConvertWorldToScreen(XMFLOAT3 WorldPos);
	XMFLOAT3 ConvertScreenToWorld(DirectX::XMFLOAT2 ScreenPos, float Z);

	//���S���W����ʓ��ɉf���Ă��邩
	bool InScreen(XMFLOAT3 WorldPos);

	bool ExistFile(const string FilePass);

	//�x�W�G�p
	float GetYFromXOnBezier(float x, const XMFLOAT2& a, const XMFLOAT2& b, uint8_t n);
	//�x�W�G(����_�w��)
	float GetBezierFromControlPoint(float t, int ControlPointNum, float* ControlPointArray);


	//����
	int GetRand(int Min, int Max);
	int GetRand(int Max);
	float GetRand(float Min, float Max);
	//���������_��
	int GetRandPlusMinus();
	//��̒l����O�㗐���Z�o
	int GetRandFromCenter(int CenterNum, int Range);


	//�w�肵�����̐��������o��
	int GetSpecifiedDigitNum(int From, int Digit);
	//�������Z�o
	int GetDigit(int Num);

	//���K��
	float Clamp(float Min, float Max, float Value);
}