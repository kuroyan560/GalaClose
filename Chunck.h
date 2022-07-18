#pragma once
#include <cstdint>
#include <xaudio2.h>

//チャンクヘッダ
struct Chunk
{
	char id[4];		//チャンク毎のID
	int32_t size;	//チャンクのサイズ
};

//RTFFヘッダチャンク
struct RiffHeader
{
	Chunk chunk;	//"RIFF"
	char type[4];	//"WAVE"
};

//FMTチャンク
struct FormatChunck
{
	Chunk chunk;	//"fmt "
	WAVEFORMAT fmt;	//波形フォーマット
};