#include "Audio.h"
#include<fstream>
#include<cassert>
ComPtr<IXAudio2> Audio::xAudio2 = {};
IXAudio2MasteringVoice* Audio::masterVoice = {};
std::map < std::string, IXAudio2SourceVoice* > Audio::soundVoices = {};
XAudio2VoiceCallback voiceCallback = {};
std::map<std::string, File> Audio::soundFiles = {};

Audio::Audio()
{
}


Audio::~Audio()
{
}

void Audio::Initialize()
{
	HRESULT result;
	//XAudio�G���W���̃C���X�^���X���쐬
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//�}�X�^�[�{�C�X���쐬
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

void Audio::LoadFile(const std::string& keyName, const std::string& fileName)
{

	File waveFile;

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	std::string str = "Resources/Sound/" + fileName;
	const char* filename = str.c_str();
	file.open(filename, std::ios_base::binary);

	//�t�@�C���I�[�v�����s�����o����
	if (file.fail())assert(0);

	//RIFF�w�b�_�[�̓ǂݍ���
	file.read((char*)&waveFile.riffheader, sizeof(waveFile.riffheader));
	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(waveFile.riffheader.chunk.id, "RIFF", 4) != 0)assert(0);

	//Format�`�����N�̓ǂݍ���
	file.read((char*)&waveFile.fChunk, sizeof(waveFile.fChunk));
	if (strncmp(waveFile.fChunk.chunk.id, "fmt ", 4) != 0)assert(0);

	//Data�`�����N�̓ǂݍ���
	file.read((char*)&waveFile.chunk, sizeof(waveFile.chunk));
	if (strncmp(waveFile.chunk.id, "data", 4) != 0)assert(0);

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	waveFile.buff = new char[waveFile.chunk.size];
	file.read(waveFile.buff, waveFile.chunk.size);

	//Wave�t�@�C�������
	file.close();

	soundFiles.emplace(keyName, waveFile);

}

void Audio::PlayWave(const std::string& keyName, const float& soundVol, bool loop, int loopCount)
{
	HRESULT result;

	//�T�E���h�Đ�
	WAVEFORMATEX wfex{};

	if (soundVoices[keyName] != nullptr && loop)
		return;

	//�g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&wfex, &soundFiles[keyName].fChunk.fmt, sizeof(soundFiles[keyName].fChunk.fmt));
	wfex.wBitsPerSample = soundFiles[keyName].fChunk.fmt.nBlockAlign * 8 / soundFiles[keyName].fChunk.fmt.nChannels;

	//�g�`�t�H�[�}�b�g������SourceVoice�̍Đ�
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex);
	if FAILED(result)
	{
		delete[] soundFiles[keyName].buff;
		return;
	}
	pSourceVoice->SetVolume(soundVol);
	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundFiles[keyName].buff;
	buf.pContext = soundFiles[keyName].buff;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundFiles[keyName].chunk.size;
	if (loop)
	{
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
		buf.LoopCount = loopCount;
	}

	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
	result = pSourceVoice->Start();
	assert(SUCCEEDED(result));
	soundVoices[keyName] = pSourceVoice;
}

void Audio::StopWave(const std::string& keyName)
{
	if (soundVoices[keyName] == nullptr)
		return;
	soundVoices[keyName]->Stop();
	soundVoices[keyName]->DestroyVoice();
	soundVoices.erase(keyName);

}

void Audio::End()
{
	masterVoice->DestroyVoice();
	if (xAudio2.Get() != nullptr)xAudio2->StopEngine();
	for (auto itr = soundFiles.begin(); itr != soundFiles.end(); itr++)
	{
		delete[](*itr).second.buff;
	}
}