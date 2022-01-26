#include "Goal.h"

#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "OBJLoader.h"
#include "SphereCollider.h"

Goal::Goal()
{
	Create(OBJLoader::GetModel("Area"));
	position = { 14.4f,12.3f,41.063f };
	HRESULT result;
	for (int i = 0; i < 3; i++)
	{
		result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffDeta) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff[i]));
		assert(SUCCEEDED(result));
	}
	offsetTime = 0;
	scale = { 0.5f,1.5f,0.5f };
	color = { 1,1,0,1 };
	object->Update();
	for (int i = 0; i < 3; i++)
	{
		colliders[i] = new BoxCollider({ 0, 0.5f, 0,0 }, { 0.25f,0.5f,0.25f });
		colliders[i]->SetRotation({ 0,30.0f * i,0 });
		colliders[i]->SetObject(this);
		colliders[i]->SetAttribute(COLLISION_ATTR_GOAL);
		colliders[i]->Update();
		CollisionManager::GetInstance()->AddCollider(colliders[i]);

	}
	Initialize();
}
void Goal::Initialize()
{
	goal = false;
}

void Goal::Update()
{
	offsetTime += 0.01f;
	if (offsetTime > 1.0f)
		offsetTime = 0;
	Object::Update();
	for (int i = 0; i < 3; i++)
	{
		colliders[i]->Update();
		if(CollisionManager::GetInstance()->CheckHitBox(*colliders[i], COLLISION_ATTR_ALLIES))
		{
			goal = true;
		}
	}
}

void Goal::Draw()
{
	if (Object3D::GetDrawShadow())
		return;	
	ConstBuffDeta* constMap = nullptr;
	auto result = constBuff[bbIndex]->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->offsetTime = offsetTime;
	constBuff[bbIndex]->Unmap(0, nullptr);
	PipelineState::SetPipeline("AreaEffect", ADD);
	
	DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, constBuff[bbIndex]->GetGPUVirtualAddress());
	Object::CustomDraw(false, false,ADD,true);
}

void Goal::DrawReady()
{
	pipelineName = "AreaEffect";
}
