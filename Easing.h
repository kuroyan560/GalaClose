#pragma once
#include<functional>
#include<DirectXMath.h>
using namespace DirectX;

static const enum EASE_CHANGE_TYPE
{
    In, Out, InOut, EASE_CHANGE_TYPE_NUM
};
static const enum EASING_TYPE
{
    Quad, Cubic, Quart, Quint, Sine, Exp, Circ, Elastic, Back, Bounce, EASING_TYPE_NUM
};

static class Easing
{
private:
    static float Lerp(float start, float end, float t);

    static float QuadIn(float t, float totaltime, float min, float max);
    static float QuadOut(float t, float totaltime, float min, float max);
    static float QuadInOut(float t, float totaltime, float min, float max);

    static float CubicIn(float t, float totaltime, float min, float max);
    static float CubicOut(float t, float totaltime, float min, float max);
    static float CubicInOut(float t, float totaltime, float min, float max);

    static float QuartIn(float t, float totaltime, float min, float max);
    static float QuartOut(float t, float totaltime, float min, float max);
    static float QuartInOut(float t, float totaltime, float min, float max);

    static float QuintIn(float t, float totaltime, float min, float max);
    static float QuintOut(float t, float totaltime, float min, float max);
    static float QuintInOut(float t, float totaltime, float min, float max);

    static float SineIn(float t, float totaltime, float min, float max);
    static float SineOut(float t, float totaltime, float min, float max);
    static float SineInOut(float t, float totaltime, float min, float max);

    static float ExpIn(float t, float totaltime, float min, float max);
    static float ExpOut(float t, float totaltime, float min, float max);
    static float ExpInOut(float t, float totaltime, float min, float max);

    static float CircIn(float t, float totaltime, float min, float max);
    static float CircOut(float t, float totaltime, float min, float max);
    static float CircInOut(float t, float totaltime, float min, float max);

    static float ElasticIn(float t, float totaltime, float min, float max);
    static float ElasticOut(float t, float totaltime, float min, float max);
    static float ElasticInOut(float t, float totaltime, float min, float max);

    static float BackIn(float t, float totaltime, float min, float max);
    static float BackOut(float t, float totaltime, float min, float max);
    static float BackInOut(float t, float totaltime, float min, float max);

    static float BounceIn(float t, float totaltime, float min, float max);
    static float BounceOut(float t, float totaltime, float min, float max);
    static float BounceInOut(float t, float totaltime, float min, float max);

    //ƒ‰ƒ€ƒ_Ž®‚Ì‚½‚ß‚ÌŒ^
    using EasingFunction = std::function<float(float t, float totaltime, float min, float max)>;
    static EasingFunction easing[EASE_CHANGE_TYPE_NUM][EASING_TYPE_NUM];
public:
    static float Ease(EASE_CHANGE_TYPE EaseChangeType, EASING_TYPE EasingType, float T, float TotalTime, float Min, float Max);
    static XMFLOAT2 Ease(EASE_CHANGE_TYPE EaseChangeType, EASING_TYPE EasingType, float T, float TotalTime, XMFLOAT2 Min, XMFLOAT2 Max);
    static XMFLOAT3 Ease(EASE_CHANGE_TYPE EaseChangeType, EASING_TYPE EasingType, float T, float TotalTime, XMFLOAT3 Min, XMFLOAT3 Max);
    static float Linear(float t, float totaltime, float min, float max);
};