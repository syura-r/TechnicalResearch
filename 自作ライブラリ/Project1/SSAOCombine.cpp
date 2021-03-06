#include "SSAOCombine.h"
#include "DirectXLib.h"
#include "PipelineState.h"
#include "Texture.h"
#include "Camera.h"
SSAOCombine::SSAOCombine()
{
	HRESULT result;

	vertices[0].uv = { 0.0f,1.0f };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[3].uv = { 1.0f,0.0f };
	vertices[0].pos = { -1.0f,-1.0f,0.0f };
	vertices[1].pos = { -1.0f,+1.0f,0.0f };
	vertices[2].pos = { +1.0f,-1.0f,0.0f };
	vertices[3].pos = { +1.0f,+1.0f,0.0f };

	auto dev = DirectXLib::GetInstance()->GetDevice();

	//頂点バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//頂点バッファを転送
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices.data(), sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの作成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffer) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));


	ConstBuffer* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	constMap->mat = XMMatrixIdentity();
	constMap->color = { 1,1,1,1 };
	constBuff->Unmap(0, nullptr);

}

void SSAOCombine::Draw()
{
	auto cmdList = DirectXLib::GetInstance()->GetCommandList();
	//パイプラインのセット
	PipelineState::SetPipeline("SSAOCombine");

	//デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetBasicDescHeap().Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//インデックスバッファのセットコマンド
	cmdList->SetGraphicsRootDescriptorTable(1, Texture::GetGpuDescHandleSRV("mainTex" + std::to_string(DirectXLib::GetInstance()->GetBbIndex())));  //ヒープの先頭が定数バッファ
	cmdList->SetGraphicsRootDescriptorTable(2, Texture::GetGpuDescHandleSRV("SSAO" + std::to_string(DirectXLib::GetInstance()->GetBbIndex())));  //ヒープの先頭が定数バッファ
	cmdList->SetGraphicsRootDescriptorTable(3, Texture::GetGpuDescHandleSRV("normalTex" + std::to_string(DirectXLib::GetInstance()->GetBbIndex())));  //ヒープの先頭が定数バッファ
	cmdList->SetGraphicsRootDescriptorTable(4, Texture::GetGpuDescHandleSRV("depthTex" + std::to_string(DirectXLib::GetInstance()->GetBbIndex())));  //ヒープの先頭が定数バッファ
	cmdList->SetGraphicsRootDescriptorTable(5, Texture::GetGpuDescHandleSRV("halfNormalTex" + std::to_string(DirectXLib::GetInstance()->GetBbIndex())));  //ヒープの先頭が定数バッファ
	cmdList->SetGraphicsRootDescriptorTable(6, Texture::GetGpuDescHandleSRV("halfDepthTex" + std::to_string(DirectXLib::GetInstance()->GetBbIndex())));  //ヒープの先頭が定数バッファ
	cmdList->DrawInstanced(4, 1, 0, 0);


}
