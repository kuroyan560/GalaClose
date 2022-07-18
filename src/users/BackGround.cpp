#include "BackGround.h"
#include"../../FbxLoader.h"
#include"Constants.h"
#include"../../Easing.h"

const float POS_OFFSET = 90.0f;
const int WAVE_TOTAL_TIME = 80;
const int SPIN_TOTAL_TIME = 60;
const int EMIT_WAVE_SPAN = 120;
//const float X_OFFSET_MIN = 60.0f;
//const float X_OFFSET_MAX = 70.0f;
//const float Y_OFFSET_MIN = 13.0f;
//const float Y_OFFSET_MAX = 20.0f;
//const int Z_MIN[BackGround::CROSS_NUM] = { 10,35,60 };
//const int Z_MAX[BackGround::CROSS_NUM] = { 20,45,70 };

BackGround::BackGround()
{
	crossModel.reset((Model_HSM_ForIncrease*)FBXLoader::LoadFBXModel("resource/model/cross/", "cross.fbx"));

	float startOffset = 0.0f;
	if (CROSS_NUM % 2 == 0)
	{
		startOffset = CROSS_NUM / 2.0f * -POS_OFFSET + POS_OFFSET / 2.0f;
	}
	else
	{
		startOffset = (int)(CROSS_NUM / 2.0f) * -POS_OFFSET;
	}

	for (int x = 0; x < CROSS_NUM; ++x)
	{
		for (int y = 0; y < CROSS_NUM; ++y)
		{
			for (int z = 0; z < CROSS_NUM; ++z)
			{
				crosses[x][y][z].masterColor.shadow = true;
				crosses[x][y][z].transform.pos = { startOffset + x * POS_OFFSET,startOffset + y * POS_OFFSET, z * POS_OFFSET + 50 };
			}
		}
	}
}

void BackGround::Init()
{
	//bool left = MyFunc::GetRand(1);
	//bool up = MyFunc::GetRand(1);
	for (int x = 0; x < CROSS_NUM; ++x)
	{
		easeTimer[x] = SPIN_TOTAL_TIME;
		//crosses[i].transform.pos.x = MyFunc::GetRand(X_OFFSET_MIN, X_OFFSET_MAX) * (left ? -1 : 1);
		//crosses[i].transform.pos.y = MyFunc::GetRand(Y_OFFSET_MIN, Y_OFFSET_MAX) * (up ? -1 : 1);
		//crosses[i].transform.pos.z = MyFunc::GetRand(Z_MIN[i], Z_MAX[i]);

		//left = !left;
		//up = !up;

		for (int y = 0; y < CROSS_NUM; ++y)
		{
			for (int z = 0; z < CROSS_NUM; ++z)
			{
				int color = MyFunc::GetRand(2);
				float lightness = MyFunc::Liner(1.1f, 1.5f, (float)z / (float)CROSS_NUM);

				crosses[x][y][z].masterColor.color.x = GLAY_COLORS[color].x * lightness;
				crosses[x][y][z].masterColor.color.y = GLAY_COLORS[color].y * lightness;
				crosses[x][y][z].masterColor.color.z = GLAY_COLORS[color].z * lightness;
			}
		}
	}

	waveTimer = WAVE_TOTAL_TIME;
	emitWaveTimer = 0;
}

void BackGround::Update()
{
	emitWaveTimer++;
	if (emitWaveTimer == EMIT_WAVE_SPAN)
	{
		Wave();
		emitWaveTimer = 0;
	}


	for (int x = 0; x < CROSS_NUM; ++x)
	{
		if (easeTimer[x] < SPIN_TOTAL_TIME)
		{
			for (int y = 0; y < CROSS_NUM; ++y)
			{
				for (int z = 0; z < CROSS_NUM; ++z)
				{
					crosses[x][y][z].transform.rotate.z = Easing::Ease(InOut, Quint, easeTimer[x], SPIN_TOTAL_TIME, 0.0f, 360.0f);
				}
			}
			easeTimer[x]++;
		}

		if (waveTimer == x * (WAVE_TOTAL_TIME / CROSS_NUM))
		{
			easeTimer[x] = 0;
		}
	}
	if (waveTimer < WAVE_TOTAL_TIME)
	{
		waveTimer++;
	}
}

void BackGround::Draw()
{
	crossModel->PreDraw();
	for (int x = 0; x < CROSS_NUM; ++x)
	{
		for (int y = 0; y < CROSS_NUM; ++y)
		{
			for (int z = 0; z < CROSS_NUM; ++z)
			{
				crossModel->DrawByComponent(crosses[x][y][z]);
			}
		}
	}
}

void BackGround::Wave()
{
	waveTimer = 0;
}