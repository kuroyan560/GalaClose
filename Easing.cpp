#include "Easing.h"
#include<cmath>

Easing::EasingFunction Easing::easing[EASE_CHANGE_TYPE_NUM][EASING_TYPE_NUM] =
{
    {QuadIn,CubicIn,QuartIn,QuintIn,SineIn,ExpIn,CircIn,ElasticIn,BackIn,BounceIn},
     {QuadOut,CubicOut,QuartOut,QuintOut,SineOut,ExpOut,CircOut,ElasticOut,BackOut,BounceOut},
     {QuadInOut,CubicInOut,QuartInOut,QuintInOut,SineInOut,ExpInOut,CircInOut,ElasticInOut,BackInOut,BounceInOut}
};

const float PI = 3.14159265359f;

float Easing::Lerp(float start, float end, float t)
{
    return (1 - t) * start + t * end;
}

float Easing::QuadIn(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime;
    return max * t * t + min;
}

float Easing::QuadOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime;
    return -max * t * (t - 2) + min;
}

float Easing::QuadInOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime / 2;
    if (t < 1) return max / 2 * t * t + min;

    t = t - 1;
    return -max / 2 * (t * (t - 2) - 1) + min;
}

float Easing::CubicIn(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime;
    return max * t * t * t + min;
}

float Easing::CubicOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t = t / totaltime - 1;
    return max * (t * t * t + 1) + min;
}

float Easing::CubicInOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime / 2;
    if (t < 1) return max / 2 * t * t * t + min;

    t = t - 2;
    return max / 2 * (t * t * t + 2) + min;
}

float Easing::QuartIn(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime;
    return max * t * t * t * t + min;
}

float Easing::QuartOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t = t / totaltime - 1;
    return -max * (t * t * t * t - 1) + min;
}

float Easing::QuartInOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime / 2;
    if (t < 1) return max / 2 * t * t * t * t + min;

    t = t - 2;
    return -max / 2 * (t * t * t * t - 2) + min;
}

float Easing::QuintIn(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime;
    return max * t * t * t * t * t + min;
}

float Easing::QuintOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t = t / totaltime - 1;
    return max * (t * t * t * t * t + 1) + min;
}

float Easing::QuintInOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime / 2;
    if (t < 1) return max / 2 * t * t * t * t * t + min;

    t = t - 2;
    return max / 2 * (t * t * t * t * t + 2) + min;
}

float Easing::SineIn(float t, float totaltime, float min, float max)
{
    max -= min;
    return -max * cos(t * (PI * 90 / 180) / totaltime) + max + min;
}

float Easing::SineOut(float t, float totaltime, float min, float max)
{
    max -= min;
    return max * sin(t * (PI * 90 / 180) / totaltime) + min;
}

float Easing::SineInOut(float t, float totaltime, float min, float max)
{
    max -= min;
    return -max / 2 * (cos(t * PI / totaltime) - 1) + min;
}

float Easing::ExpIn(float t, float totaltime, float min, float max)
{
    max -= min;
    return t == 0.0 ? min : max * pow(2, 10 * (t / totaltime - 1)) + min;
}

float Easing::ExpOut(float t, float totaltime, float min, float max)
{
    max -= min;
    return t == totaltime ? max + min : max * (-pow(2, -10 * t / totaltime) + 1) + min;
}

float Easing::ExpInOut(float t, float totaltime, float min, float max)
{
    if (t == 0.0f) return min;
    if (t == totaltime) return max;
    max -= min;
    t /= totaltime / 2;

    if (t < 1) return max / 2 * pow(2, 10 * (t - 1)) + min;

    t = t - 1;
    return max / 2 * (-pow(2, -10 * t) + 2) + min;
}

float Easing::CircIn(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime;
    return -max * (sqrt(1 - t * t) - 1) + min;
}

float Easing::CircOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t = t / totaltime - 1;
    return max * sqrt(1 - t * t) + min;
}

float Easing::CircInOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime / 2;
    if (t < 1) return -max / 2 * (sqrt(1 - t * t) - 1) + min;

    t = t - 2;
    return max / 2 * (sqrt(1 - t * t) + 1) + min;
}

float Easing::ElasticIn(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime;

    float s = 1.70158f;
    float p = totaltime * 0.3f;
    float a = max;

    if (t == 0) return min;
    if (t == 1) return min + max;

    if (a < abs(max))
    {
        a = max;
        s = p / 4;
    }
    else
    {
        s = p / (2 * PI) * asin(max / a);
    }

    t = t - 1;
    return -(a * pow(2, 10 * t) * sin((t * totaltime - s) * (2 * PI) / p)) + min;
}

float Easing::ElasticOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime;

    float s = 1.70158f;
    float p = totaltime * 0.3f; ;
    float a = max;

    if (t == 0) return min;
    if (t == 1) return min + max;

    if (a < abs(max))
    {
        a = max;
        s = p / 4;
    }
    else
    {
        s = p / (2 * PI) * asin(max / a);
    }

    return a * pow(2, -10 * t) * sin((t * totaltime - s) * (2 * PI) / p) + max + min;
}

float Easing::ElasticInOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime / 2;

    float s = 1.70158f;
    float p = totaltime * (0.3f * 1.5f);
    float a = max;

    if (t == 0) return min;
    if (t == 2) return min + max;

    if (a < abs(max))
    {
        a = max;
        s = p / 4;
    }
    else
    {
        s = p / (2 * PI) * asin(max / a);
    }

    if (t < 1)
    {
        return -0.5f * (a * pow(2, 10 * (t -= 1)) * sin((t * totaltime - s) * (2 * PI) / p)) + min;
    }

    t = t - 1;
    return a * pow(2, -10 * t) * sin((t * totaltime - s) * (2 * PI) / p) * 0.5f + max + min;
}

float Easing::BackIn(float t, float totaltime, float min, float max)
{
    float s = 1.70158f;
    max -= min;
    t /= totaltime;
    return max * t * t * ((s + 1) * t - s) + min;
}

float Easing::BackOut(float t, float totaltime, float min, float max)
{
    float s = 1.70158f;
    max -= min;
    t = t / totaltime - 1;
    return max * (t * t * ((s + 1) * t + s) + 1) + min;
}

float Easing::BackInOut(float t, float totaltime, float min, float max)
{
    float s = 1.70158f;
    max -= min;
    s *= 1.525f;
    t /= totaltime / 2;
    if (t < 1) return max / 2 * (t * t * ((s + 1) * t - s)) + min;

    t = t - 2;
    return max / 2 * (t * t * ((s + 1) * t + s) + 2) + min;
}

float Easing::BounceIn(float t, float totaltime, float min, float max)
{
    max -= min;
    return max - BounceOut(totaltime - t, totaltime, 0, max) + min;
}

float Easing::BounceOut(float t, float totaltime, float min, float max)
{
    max -= min;
    t /= totaltime;

    if (t < 1.0f / 2.75f)
    {
        return max * (7.5625f * t * t) + min;
    }
    else if (t < 2.0f / 2.75f)
    {
        t -= 1.5f / 2.75f;
        return max * (7.5625f * t * t + 0.75f) + min;
    }
    else if (t < 2.5f / 2.75f)
    {
        t -= 2.25f / 2.75f;
        return max * (7.5625f * t * t + 0.9375f) + min;
    }
    else
    {
        t -= 2.625f / 2.75f;
        return max * (7.5625f * t * t + 0.984375f) + min;
    }
}

float Easing::BounceInOut(float t, float totaltime, float min, float max)
{
    if (t < totaltime / 2)
    {
        return BounceIn(t * 2, totaltime, 0, max - min) * 0.5f + min;
    }
    else
    {
        return BounceOut(t * 2 - totaltime, totaltime, 0, max - min) * 0.5f + min + (max - min) * 0.5f;
    }
}

float Easing::Ease(EASE_CHANGE_TYPE EaseChangeType, EASING_TYPE EasingType, float T, float TotalTime, float Min, float Max)
{
    return easing[EaseChangeType][EasingType](T, TotalTime, Min, Max);
}

XMFLOAT2 Easing::Ease(EASE_CHANGE_TYPE EaseChangeType, EASING_TYPE EasingType, float T, float TotalTime, XMFLOAT2 Min, XMFLOAT2 Max)
{
    XMFLOAT2 result;
    result.x = easing[EaseChangeType][EasingType](T, TotalTime, Min.x, Max.x);
    result.y = easing[EaseChangeType][EasingType](T, TotalTime, Min.y, Max.y);
    return result;
}

XMFLOAT3 Easing::Ease(EASE_CHANGE_TYPE EaseChangeType, EASING_TYPE EasingType, float T, float TotalTime, XMFLOAT3 Min, XMFLOAT3 Max)
{
    XMFLOAT3 result;
    result.x = easing[EaseChangeType][EasingType](T, TotalTime, Min.x, Max.x);
    result.y = easing[EaseChangeType][EasingType](T, TotalTime, Min.y, Max.y);
    result.z = easing[EaseChangeType][EasingType](T, TotalTime, Min.z, Max.z);
    return result;
}

float Easing::Linear(float t, float totaltime, float min, float max)
{
    return (max - min) * t / totaltime + min;
}
