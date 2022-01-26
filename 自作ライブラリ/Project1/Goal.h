#pragma once
#include "BoxCollider.h"
#include "Object.h"
class Goal :
    public Object
{
public:
    Goal();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void DrawReady() override;
	bool GetGoal() { return goal; }
private:
	struct ConstBuffDeta
	{
		float offsetTime;
		XMFLOAT3 pad;
	};
	float offsetTime;
	ComPtr<ID3D12Resource> constBuff[3]; // 定数バッファ
	BoxCollider* colliders[3];
	bool goal;
};

