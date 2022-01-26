#pragma once
#include"Texture.h"
#include"PipelineState.h"
#include"Sprite.h"
#include<string>
#include<vector>
#include"window.h"
static const int maxCharCount = 256;//�ő啶����
struct Status
{
	XMFLOAT2 position;
	XMFLOAT2 scale;
};
class DebugText
{
public:
	static void Initialize();
	static void Print(const std::string& text,const float& posX,const float& posY,const float& scale = 1.0f);
	static void Draw(const XMFLOAT4& color = {1,1,1,1});
	static void End();
private:
	static const int fontWidth = 9;		//�t�H���g�摜��1�������ډ���
	static const int fontHeight = 18;	//�t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14;//�t�H���g�摜��1�s���̕�����
	static Sprite* spriteDatas[maxCharCount];
	static std::vector<Status> statues;
	static int spriteIndex;
};

