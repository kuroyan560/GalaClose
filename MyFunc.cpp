#include"MyFunc.h"
#include<Windows.h>
#include <cassert>
#include"CameraMgr.h"
#include"WinApp.h"

#include<fstream>
#include<iostream>

std::wstring MyFunc::GetWideStrFromStr(const std::string& Str)
{
	auto num1 = MultiByteToWideChar(
		CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		Str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(
		CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		Str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);

	return wstr;
}

std::string MyFunc::GetExtension(const std::string& Path)
{
	int idx = Path.rfind('.');
	return Path.substr(idx + 1, Path.length() - idx - 1);
}

XMFLOAT4 MyFunc::Lerp(XMFLOAT4 start, XMFLOAT4 end, float t)
{
	return XMFLOAT4(
		Lerp(start.x, end.x, t),
		Lerp(start.y, end.y, t),
		Lerp(start.z, end.z, t),
		Lerp(start.w, end.w, t));
}

XMFLOAT3 MyFunc::Lerp(XMFLOAT3 start, XMFLOAT3 end, float t)
{
	return XMFLOAT3(
		Lerp(start.x, end.x, t),
		Lerp(start.y, end.y, t),
		Lerp(start.z, end.z, t));
}

XMFLOAT2 MyFunc::Lerp(XMFLOAT2 start, XMFLOAT2 end, float t)
{
	return XMFLOAT2(
		Lerp(start.x, end.x, t),
		Lerp(start.y, end.y, t));
}

float MyFunc::Lerp(float start, float end, float t)
{
	return (1 - t) * start + t * end;
}

XMFLOAT4 MyFunc::Liner(XMFLOAT4 start, XMFLOAT4 end, float t)
{
	return XMFLOAT4(
		Liner(start.x, end.x, t),
		Liner(start.y, end.y, t),
		Liner(start.z, end.z, t),
		Liner(start.w, end.w, t));
}

XMFLOAT3 MyFunc::Liner(XMFLOAT3 start, XMFLOAT3 end, float t)
{
	return XMFLOAT3(
		Liner(start.x, end.x, t),
		Liner(start.y, end.y, t),
		Liner(start.z, end.z, t));
}

XMFLOAT2 MyFunc::Liner(XMFLOAT2 start, XMFLOAT2 end, float t)
{
	return XMFLOAT2(
		Liner(start.x, end.x, t),
		Liner(start.y, end.y, t));
}

float MyFunc::Liner(float start, float end, float t)
{
	return (end - start) * t + start;
}

float MyFunc::GetDistXMFLOAT3(XMFLOAT3 from, XMFLOAT3 to)
{
	float result = pow(to.x - from.x, 2) + pow(to.y - from.y, 2) + pow(to.z - from.z, 2);
	result = sqrt(result);
	return result;
}

XMFLOAT2 MyFunc::NormalizeXMFLOAT2(XMFLOAT2 Tmp)
{
	float dist = pow(Tmp.x, 2) + pow(Tmp.y, 2);
	dist = sqrt(dist);
	return XMFLOAT2(Tmp.x / dist, Tmp.y / dist);
}

XMFLOAT3 MyFunc::NormalizeXMFLOAT3(XMFLOAT3 Tmp)
{
	float dist = pow(Tmp.x, 2) + pow(Tmp.y, 2) + pow(Tmp.z, 2);
	dist = sqrt(dist);
	return XMFLOAT3(Tmp.x / dist, Tmp.y / dist, Tmp.z / dist);
}

XMFLOAT2 MyFunc::AddXMFLOAT2(XMFLOAT2 a, XMFLOAT2 b)
{
	return XMFLOAT2(a.x + b.x, a.y + b.y);
}

XMFLOAT2 MyFunc::SubXMFLOAT2(XMFLOAT2 Target, XMFLOAT2 Value)
{
	return XMFLOAT2(Target.x - Value.x, Target.y - Value.y);
}

XMFLOAT2 MyFunc::MulXMFLOAT2(XMFLOAT2 a, XMFLOAT2 b)
{
	return XMFLOAT2(a.x * b.x, a.y * b.y);
}

XMFLOAT2 MyFunc::MulXMFLOAT2(XMFLOAT2 Target, float Value)
{
	return XMFLOAT2(Target.x * Value, Target.y * Value);
}

XMFLOAT2 MyFunc::DivXMFLOAT2(XMFLOAT2 Target, XMFLOAT2 Value)
{
	return XMFLOAT2(Target.x / Value.x, Target.y / Value.y);
}

XMFLOAT3 MyFunc::AddXMFLOAT3(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

XMFLOAT3 MyFunc::SubXMFLOAT3(XMFLOAT3 Target, XMFLOAT3 Value)
{
	return XMFLOAT3(Target.x - Value.x, Target.y - Value.y, Target.z - Value.z);
}

XMFLOAT3 MyFunc::MulXMFLOAT3(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x * b.x, a.y * b.y, a.z * b.z);
}

XMFLOAT3 MyFunc::MulXMFLOAT3(XMFLOAT3 Target, float Value)
{
	return XMFLOAT3(Target.x * Value, Target.y * Value, Target.z * Value);
}

XMFLOAT3 MyFunc::DivXMFLOAT3(XMFLOAT3 Target, float Value)
{
	if (Value == 0.0f)return XMFLOAT3(0, 0, 0);
	return XMFLOAT3(Target.x / Value, Target.y / Value, Target.z / Value);
}

XMFLOAT2 MyFunc::GetCenterXMFLOAT2(XMFLOAT2 From, XMFLOAT2 To)
{
	XMFLOAT2 result = MyFunc::SubXMFLOAT2(To, From);
	result = MyFunc::MulXMFLOAT2(result, 0.5f);
	result = MyFunc::AddXMFLOAT2(From, result);
	return result;
}

XMFLOAT3 MyFunc::GetCenterXMFLOAT3(XMFLOAT3 From, XMFLOAT3 To)
{
	XMFLOAT3 result = MyFunc::SubXMFLOAT3(To, From);
	result = MyFunc::MulXMFLOAT3(result, 0.5f);
	result = MyFunc::AddXMFLOAT3(From, result);
	return result;
}

float MyFunc::GetLineSlope(XMFLOAT2 From, XMFLOAT2 To)
{
	return (To.y - From.y) / (To.x - From.x);
}

XMFLOAT2 MyFunc::ConvertWorldToScreen(XMFLOAT3 WorldPos)
{
	XMVECTOR pos = XMLoadFloat3(&WorldPos);
	//ビュー行列適応
	pos = XMVector3Transform(pos, CameraMgr::Instance()->GetView());
	//プロジェクション行列適応
	pos = XMVector3TransformCoord(pos, CameraMgr::Instance()->GetProjection());

	//ビューポート行列
	XMMATRIX viewPortMat = XMMatrixIdentity();
	viewPortMat.r[0].m128_f32[0] = (float)WinApp::WIN_WIDTH / 2.0f;
	viewPortMat.r[1].m128_f32[1] = (float)-WinApp::WIN_HEIGHT / 2.0f;
	viewPortMat.r[3].m128_f32[0] = (float)WinApp::WIN_WIDTH / 2.0f;
	viewPortMat.r[3].m128_f32[1] = (float)WinApp::WIN_HEIGHT / 2.0f;
	//ビューポート行列適応
	pos = XMVector3Transform(pos, viewPortMat);

	XMFLOAT2 result;
	result.x = pos.m128_f32[0];
	result.y = pos.m128_f32[1];
	//result.z = pos.m128_f32[2];

	return result;
}

XMFLOAT3 MyFunc::ConvertScreenToWorld(DirectX::XMFLOAT2 ScreenPos, float Z)
{
	XMMATRIX invView, invPrj, invViewPort;
	invView = XMMatrixInverse(nullptr, CameraMgr::Instance()->GetView());
	invPrj = XMMatrixInverse(nullptr, CameraMgr::Instance()->GetProjection());

	invViewPort = XMMatrixIdentity();
	invViewPort.r[0].m128_f32[0] = (float)WinApp::WIN_WIDTH / 2.0f;
	invViewPort.r[1].m128_f32[1] = (float)-WinApp::WIN_HEIGHT / 2.0f;
	invViewPort.r[3].m128_f32[0] = (float)WinApp::WIN_WIDTH / 2.0f;
	invViewPort.r[3].m128_f32[1] = (float)WinApp::WIN_HEIGHT / 2.0f;
	invViewPort = XMMatrixInverse(nullptr, invViewPort);

	XMVECTOR pos;
	pos = XMLoadFloat3(&XMFLOAT3(ScreenPos.x, ScreenPos.y, Z));
	/*
		※授業資料05_03.射影変換　透視投影変換を参照
		z(0.0f ~ 1.0f)
		0.0f で proj のnear面
		1.0f で far面の位置
	*/
	pos = XMVector3Transform(pos, invViewPort);
	pos = XMVector3TransformCoord(pos, invPrj);
	//Coordでw除算を行ってくれる
	/*
		w除算をすることによって遠くのものがすぼまっていき、
		画面内に収まるようにしてくれる
		Coordがないとスクリーンから見切れた、みたいになる
	*/
	pos = XMVector3Transform(pos, invView);

	XMFLOAT3 result;
	result.x = pos.m128_f32[0];
	result.y = pos.m128_f32[1];
	result.z = pos.m128_f32[2];
	//result.x = base.r[0].m128_f32[0];
	//result.y = base.r[0].m128_f32[1];

	return result;
}

bool MyFunc::InScreen(XMFLOAT3 WorldPos)
{
	XMFLOAT2 center = MyFunc::ConvertWorldToScreen(WorldPos);
	if (center.x < 0)return false;
	if (WinApp::WIN_WIDTH < center.x)return false;
	if (center.y < 0)return false;
	if (WinApp::WIN_HEIGHT < center.y)return false;
	return true;
}

bool MyFunc::ExistFile(const string FilePass)
{
	std::ifstream ifs(FilePass);
	return ifs.is_open();
}

float MyFunc::GetYFromXOnBezier(float x, const XMFLOAT2& a, const XMFLOAT2& b, uint8_t n)
{
	if (a.x == a.y && b.x == b.y)
	{
		return x;		//計算不要
	}

	float t = x;
	const float k0 = 1 + 3 * a.x - 3 * b.x;	//t^3の係数
	const float k1 = 3 * b.x - 6 * a.x;	//t^2の係数
	const float k2 = 3 * a.x;	//tの係数

	constexpr float epsilon = 0.0005f;	//誤差許容範囲

	//tを近似で求める
	for (int i = 0; i < n; i++)
	{
		//f(t)を求める
		auto ft = k0 * t * t * t + k1 * t * t + k2 * t - x;
		//もし結果が０に近い（誤差の範囲内）なら打ち切る
		if (ft <= epsilon && ft >= -epsilon)
		{
			break;
		}
		t -= ft / 2;	//刻む
	}

	//求めたいtは既に求めているのでyを計算
	auto r = 1 - t;
	return t * t * t + 3 * t * t * r * b.y + 3 * t * r * r * a.y;
}

//階乗を求める
int GetFactorial(int num)
{
	int result = 1;
	for (int i = num; 0 < i; --i)
	{
		result *= i;
	}
	return result;
}

//n個の中からk個を選ぶときの組み合わせの数
int GetSelectPatternNum(int n, int k)
{
	if (k == 0)return 1;

	//分子
	int upper = GetFactorial(n);
	//分母
	int a = GetFactorial(k);
	int b = GetFactorial(n - k);
	int lower = a * b;

	return upper / lower;
}

float MyFunc::GetBezierFromControlPoint(float t, int ControlPointNum, float* ControlPointArray)
{
	//ControlPointNum == n

	float result = 0;
	for (int i = 0; i < ControlPointNum; ++i)
	{
		int coefficient = GetSelectPatternNum(ControlPointNum, i);	//二項係数
		float a = pow(t, i);	// t の i 乗
		float b = pow((1 - t), (ControlPointNum - i));	// (1-t) の n-i 乗
		result += coefficient * a * b * ControlPointArray[i];		//制御点の重み付けをして加算(Σ)
	}
	return result;
}

int MyFunc::GetRand(int Min, int Max)
{
	int result = (Max - Min + 1) * rand() / RAND_MAX + Min;
	return result;
}

int MyFunc::GetRand(int Max)
{
	return GetRand(0, Max);
}

float MyFunc::GetRand(float Min, float Max)
{
	float result = (Max - Min) * rand() / RAND_MAX + Min;
	return result;
}

int MyFunc::GetRandPlusMinus()
{
	int r = GetRand(0, 1);
	if (r)return 1;
	else return -1;
}

int MyFunc::GetRandFromCenter(int CenterNum, int Range)
{
	return CenterNum + GetRand(0, Range) * GetRandPlusMinus();
}


int MyFunc::GetSpecifiedDigitNum(int From, int Digit)
{
	int result = 0;
	for (int i = Digit; 0 <= i; i--)
	{
		result = From % 10;				//6-i桁目の数字を記録
		From /= 10;					//桁を減らす
	}
	return result;
}

int MyFunc::GetDigit(int Num)
{
	int result = 0;
	while (Num != 0)
	{
		Num /= 10;
		result++;
	}
	return result;
}

float MyFunc::Clamp(float Min, float Max, float Value)
{
	if (Max < Value)Value = Max;
	if (Value < Min)Value = Min;
	return Value;
}
