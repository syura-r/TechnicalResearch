#pragma once
#include"XAudio2VoiceCallback.h"
#include<cstdint>
#include<wrl.h>
#include<xaudio2.h>
#include<string>
#include<map>
using namespace Microsoft::WRL;
#pragma comment(lib,"xaudio2.lib")
//�`�����N�w�b�_
struct Chunk
{
	char id[4];//�`�����N���Ƃ�ID
	int32_t size;//�`�����N�T�C�Y
};

//RIFF�w�b�_�`�����N
struct RiffHeader
{
	Chunk chunk;//"RIFF"
	char type[4];//"WAVE"
};

//FMT�`�����N
struct FormatChunk
{
	Chunk chunk;//"fmt"
	WAVEFORMAT fmt;//�g�`�t�H�[�}�b�g
};
struct File
{
	Chunk chunk;
	RiffHeader riffheader;
	FormatChunk fChunk;
	char* buff;
};

class Audio
{
public:
	Audio();
	~Audio();
	static void Initialize();
	static void LoadFile(const std::string& keyName, const std::string& fileName);
	static void PlayWave(const std::string& keyName, const float& soundVol = 1.0f, bool loop = false, int loopCount = XAUDIO2_LOOP_INFINITE);
	static void StopWave(const std::string& keyName);
	static void End();
private:

	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
	static std::map < std::string, IXAudio2SourceVoice*> soundVoices;
	static std::map < std::string, File> soundFiles;
};
