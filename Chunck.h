#pragma once
#include <cstdint>
#include <xaudio2.h>

//�`�����N�w�b�_
struct Chunk
{
	char id[4];		//�`�����N����ID
	int32_t size;	//�`�����N�̃T�C�Y
};

//RTFF�w�b�_�`�����N
struct RiffHeader
{
	Chunk chunk;	//"RIFF"
	char type[4];	//"WAVE"
};

//FMT�`�����N
struct FormatChunck
{
	Chunk chunk;	//"fmt "
	WAVEFORMAT fmt;	//�g�`�t�H�[�}�b�g
};