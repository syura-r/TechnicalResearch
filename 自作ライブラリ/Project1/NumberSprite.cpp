#include "NumberSprite.h"

#include "PtrDelete.h"
#include "Texture.h"
#include "Vector.h"

NumberSprite::NumberSprite(float& num):num(num)
{
}

NumberSprite::~NumberSprite()
{
	for(auto it = sprites.begin();it!=sprites.end();++it)
	{
		PtrDelete(*it);
	}
}

void NumberSprite::Draw(const int digits, const std::string& texName, const XMFLOAT2& pos, const XMFLOAT2& scale, const XMFLOAT4& color,const XMFLOAT2& anchorPoint)
{
	if (sprites.size() < digits)
	{
		const int add = digits - sprites.size();
		for (int i = 0; i < add; i++)
		{
			sprites.push_back(new Sprite());
		}
	}
	std::string nuwNum = std::to_string(num);
	D3D12_RESOURCE_DESC resDesc = Texture::GetTexBuff(texName).Get()->GetDesc();
	const float width = (float)resDesc.Width / 10; //�摜�̉���
	const float height = (float)resDesc.Height; //�摜�̉���

	const float offset = (float)(digits - 1) *  anchorPoint.x;
	int k = 0;
	for (auto i = 0; i < digits; i++)
	{
		int drawNumber = nuwNum[k];
		//�����_���������΂�
		if(drawNumber == 46)
		{
			k++;
			drawNumber = nuwNum[k];
		}
		drawNumber -= 48;
		XMFLOAT2 position = pos;
		position.x += i * width * scale.x - offset * width * scale.x;
		sprites[i]->SpriteSetTextureRect(texName, drawNumber * width, 0, width, height);
		sprites[i]->DrawSprite(texName, position, 0, scale, color);
		k++;
	}

}

