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

	std::array<ComPtr<ID3D12Resource>, 3> constBuff; // 定数バッファ
	std::array<std::unique_ptr<BoxCollider>, 3> colliders;

	bool goal;
};

