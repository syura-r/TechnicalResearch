#include "SSAO.h"

#include "DirectXLib.h"
#include "PipelineState.h"
#include "Texture.h"
#include "Camera.h"
#include "ComputeShade.h"
#include "TextureResource.h"
Camera* SSAO::camera = nullptr;
SSAO::SSAO()
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
		memcpy(vertMap, vertices, sizeof(vertices));
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
	XMFLOAT4 normals[trycnt];
	for (int i = 0; i < trycnt; i++)
	{
		Vector3 randNormal = Vector3{ (float)rand() - rand(),(float)rand() - rand(),(float)rand() - rand() }.Normalize();
		normals[i] = { randNormal.x,randNormal.y,randNormal.z,0 };

	}
	for (int i = 0; i < 3; i++)
	{
		//定数バッファの作成
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuffer2) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuff2[i]));
		assert(SUCCEEDED(result));



		ConstBuffer2* constMap2 = nullptr;
		result = constBuff2[i]->Map(0, nullptr, (void**)&constMap2);
		assert(SUCCEEDED(result));
		memcpy(constMap2->randNormal, normals,sizeof(XMFLOAT4)*trycnt);
		constBuff2[i]->Unmap(0, nullptr);
	}

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&depthSRVHeap));
	assert(SUCCEEDED(result));

	D3D12_SHADER_RESOURCE_VIEW_DESC resDesc = {};
	resDesc.Format = DXGI_FORMAT_R32_FLOAT;
	resDesc.Texture2D.MipLevels = 1;
	resDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	resDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	auto handle = depthSRVHeap->GetCPUDescriptorHandleForHeapStart();
	//通常デプス→テクスチャ用
	dev->CreateShaderResourceView(TextureResource::mainResource->depthBuffer.Get(),
		&resDesc,
		handle);

}

void SSAO::Draw()
{
	auto cmdList = DirectXLib::GetInstance()->GetCommandList();

	SetCameraBuff();
	//パイプラインのセット
	PipelineState::SetPipeline("SSAO");

	//デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetBasicDescHeap().Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff2[bbindex]->GetGPUVirtualAddress());

	//インデックスバッファのセットコマンド
	cmdList->SetGraphicsRootDescriptorTable(2, Texture::GetGpuDescHandleSRV("normalTex" + std::to_string(DirectXLib::GetInstance()->GetBbIndex())));  //ヒープの先頭が定数バッファ
	cmdList->SetDescriptorHeaps(1, depthSRVHeap.GetAddressOf());
	auto handle = depthSRVHeap->GetGPUDescriptorHandleForHeapStart();
	cmdList->SetGraphicsRootDescriptorTable(3, handle);

	cmdList->DrawInstanced(4, 1, 0, 0);
}

void SSAO::SetCameraBuff()
{
	bbindex = DirectXLib::GetInstance()->GetBbIndex();
	ConstBuffer2* constMap = nullptr;
	auto result = constBuff2[bbindex]->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	constMap->view = camera->GetMatView();
	constMap->proj = camera->GetMatProjection();
	constMap->invproj = XMMatrixInverse(nullptr,camera->GetMatViewProjection());

	constBuff2[bbindex]->Unmap(0, nullptr);

}
