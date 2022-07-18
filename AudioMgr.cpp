#include "AudioMgr.h"

using namespace std;

AudioMgr::AudioMgr()
{
	HRESULT result;

	//XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//�}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

AudioMgr::~AudioMgr()
{
	xAudio2.Reset();
	for (auto itr = audios.begin(); itr != audios.end(); ++itr)
	{
		itr->Unload();
	}
}

void AudioMgr::Update()
{
	for (auto& audio : audios)
	{
		audio.playTrigger = false;
	}
}

bool AudioMgr::NowPlay(const int& Handle)
{
	auto itr = audios.begin();
	for (int i = 0; i < Handle; i++)
	{
		itr++;
	}

	if (itr->pSourceVoice != nullptr)
	{
		XAUDIO2_VOICE_STATE state;
		itr->pSourceVoice->GetState(&state);
		return !(state.pCurrentBufferContext == nullptr);
	}
	else return false;
}

void AudioMgr::ChangeVolume(const int& Handle, float Volume)
{
	auto itr = audios.begin();
	for (int i = 0; i < Handle; i++)
	{
		itr++;
	}
	itr->volume = Volume;
	if (itr->pSourceVoice != nullptr)
	{
		itr->pSourceVoice->SetVolume(itr->volume);
	}
}

float AudioMgr::GetVolume(const int& Handle)
{
	auto itr = audios.begin();
	for (int i = 0; i < Handle; i++)
	{
		itr++;
	}
	return itr->volume;
}

int AudioMgr::LoadAudio(string FileName)
{
	//if (!audios.empty())
	//{
	//	int i = 0;
	//	for (auto itr = audios.begin(); itr != audios.end(); itr++)
	//	{
	//		if (itr->filePass == FileName)
	//		{
	//			return i;
	//		}
	//		i++;
	//	}
	//}

	//audios.emplace_back(FileName);

	//HRESULT result;

	////�@�t�@�C���I�[�v��
	//	//�t�@�C�����̓X�g���[���̃C���X�^���X
	//std::ifstream file;
	////.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	//file.open(FileName.c_str(), std::ios_base::binary);
	////�t�@�C���I�[�v�����s�����o����
	//if (file.fail())
	//{
	//	assert(0);
	//}

	////�A.wav�f�[�^���
	////RITF�w�b�_�[�̓ǂݍ���
	//file.read((char*)&audios.back().riff, sizeof(audios.back().riff));
	////�t�@�C����RIFF���`�F�b�N
	//if (strncmp(audios.back().riff.chunk.id, "RIFF", 4) != 0)
	//{
	//	assert(0);
	//}

	////Format�`�����N�̓ǂݍ���
	//file.read((char*)&audios.back().format, sizeof(audios.back().format));
	////�t�@�C����Format���`�F�b�N
	//if (strncmp(audios.back().format.chunk.id, "fmt", 3) != 0)
	//{
	//	assert(0);
	//}

	////Data�`�����N�̓ǂݍ���
	//file.read((char*)&audios.back().data, sizeof(audios.back().data));
	////�t�@�C����Data���`�F�b�N
	//if (strncmp(audios.back().data.id, "data", 4) != 0)
	//{
	//	assert(0);
	//}

	////Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	//audios.back().pBuffer = new char[audios.back().data.size];
	//file.read(audios.back().pBuffer, audios.back().data.size);

	////Wave�t�@�C�������
	//file.close();

	//return audios.size() - 1;
	if (!audios.empty())
	{
		int i = 0;
		for (auto itr = audios.begin(); itr != audios.end(); itr++)
		{
			if (itr->filePass == FileName)
			{
				return i;
			}
			i++;
		}
	}

	audios.emplace_back(FileName);

	HRESULT result;

	//�@�t�@�C���I�[�v��
		//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(FileName.c_str(), std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	if (file.fail())
	{
		assert(0);
	}

	while (1)
	{
		Chunk chunk;
		file.read((char*)&chunk, sizeof(Chunk));

		if (strncmp(chunk.id, "RIFF", 4) == 0)
		{
			audios.back().riff.chunk = chunk;
			file.read(audios.back().riff.type, sizeof(audios.back().riff.type));
		}
		else if (strncmp(chunk.id, "fmt ", 4) == 0)
		{
			audios.back().format.chunk = chunk;
			file.read((char*)&audios.back().format.fmt, chunk.size);
		}
		else if (strncmp(chunk.id, "data", 4) == 0)
		{
			audios.back().data = chunk;
			//Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
			audios.back().pBuffer = new char[audios.back().data.size];
			file.read(audios.back().pBuffer, audios.back().data.size);
			break;
		}
		else
		{
			//�s�K�v�ȏ��Ȃ疳��
			file.seekg(chunk.size, ios::cur);
		}
	}

	//Wave�t�@�C�������
	file.close();

	return audios.size() - 1;
}

int AudioMgr::PlayWave(const int& Handle, bool LoopFlag)
{
	HRESULT result;

	auto itr = audios.begin();
	for (int i = 0; i < Handle; i++)
	{
		itr++;
	}

	//�����ɓ����������Đ����Ȃ�
	if (itr->playTrigger)
	{
		return -1;
	}

	//���[�v�Đ��ōĐ����悤�Ƃ�������ɗ���Ă�����̂��~
	if (LoopFlag && NowPlay(Handle))
	{
		itr->pSourceVoice->Stop();
	}

//�B�T�E���h�Đ�
	WAVEFORMATEX wfex{};
	//�g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&wfex, &itr->format.fmt, sizeof(itr->format.fmt));
	wfex.wBitsPerSample = itr->format.fmt.nBlockAlign * 8 / itr->format.fmt.nChannels;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = xAudio2->CreateSourceVoice(&itr->pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result)
	{
		return -1;
	}
	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)itr->pBuffer;
	buf.pContext = itr->pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = itr->data.size;
	if (LoopFlag)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	
	//���ʂ̐ݒ�
	itr->pSourceVoice->SetVolume(itr->volume);

	//�g�`�f�[�^�̍Đ�
	result = itr->pSourceVoice->SubmitSourceBuffer(&buf);
	result = itr->pSourceVoice->Start();

	itr->playTrigger = true;
}

void AudioMgr::StopWave(const int& Handle)
{
	auto itr = audios.begin();
	for (int i = 0; i < Handle; i++)
	{
		itr++;
	}
	if (itr->pSourceVoice != nullptr)
	{
		itr->pSourceVoice->Stop();
		itr->pSourceVoice->FlushSourceBuffers();
	}
}

void AudioMgr::AudioData::Unload()
{
	if (pBuffer != nullptr)delete[] pBuffer;
}
