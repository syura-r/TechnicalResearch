#include "TextureResource.h"

#include "DirectXLib.h"
#include "Texture.h"
std::vector<TextureResource*> TextureResource::nowRenderTargets = {};
int TextureResource::bbindex = 0;
TextureResource::TextureResource(const std::string& name, const bool noDepth)
	: resourceWidth(1920), resourceHeight(1080), format(DXGI_FORMAT_R8G8B8A8_UNORM), clearColor{ 1,1,1,1 }, threeResource(true), noDepth(noDepth)
{
	Initialize(name);
}

TextureResource::TextureResource(const std::string& name, const Vector2& size, const DXGI_FORMAT resourceFormat, const DirectX::XMFLOAT4& arg_clearColor, const bool arg_threeResource, const bool noDepth)
	: resourceWidth(size.x), resourceHeight(size.y), format(resourceFormat), clearColor{ arg_clearColor.x,arg_clearColor.y, arg_clearColor.z, arg_clearColor.w }, threeResource(arg_threeResource), noDepth(noDepth)
{
	Initialize(name);
}
void TextureResource::Initialize(const std::string& name)
{
	dev = DirectXLib::GetInstance()->GetDevice();
	cmdList = DirectXLib::GetInstance()->GetCommandList();
	auto heapDesc = DirectXLib::GetInstance()->GetRtvHeaps()->GetDesc();

	//�g���Ă�o�b�N�o�b�t�@�[�̏��𗘗p����
	auto& bbuff = DirectXLib::GetInstance()->GetBackBuffers()[0];
	auto resDesc = bbuff->GetDesc();
	resDesc.Format = format;
	D3D12_HEAP_PROPERTIES heapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	resDesc.Height = resourceHeight;
	resDesc.Width = resourceWidth;

	//�����_�����O���̃N���A�l�Ɠ����l
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(format, clearColor.data());

	HRESULT result;

	if (threeResource)
	{
		//���\�[�X�쐬
		for (int i = 0; i < 3; i++)
		{
			result = dev->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				IID_PPV_ARGS(resource[i].ReleaseAndGetAddressOf()));
			if (result != S_OK)
				assert(0);
		}
		//RTV�p�q�[�v�����
		heapDesc.NumDescriptors = 3;
	}
	else
	{
		result = dev->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(resource[0].ReleaseAndGetAddressOf()));
		if (result != S_OK)
			assert(0);
		//RTV�p�q�[�v�����
		heapDesc.NumDescriptors = 1;
	}
	result = dev->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(peraRTVHeap.ReleaseAndGetAddressOf()));
	if (result != S_OK)
		assert(0);

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = format;

	if (threeResource)
	{
		//�����_�[�^�[�Q�b�g�r���[�����
		for (int i = 0; i < 3; i++)
		{
			dev->CreateRenderTargetView(
				resource[i].Get(),
				&rtvDesc,
				CD3DX12_CPU_DESCRIPTOR_HANDLE(
					peraRTVHeap->GetCPUDescriptorHandleForHeapStart(), i,
					dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
		}
		//SRV�p�q�[�v�����
		heapDesc.NumDescriptors = 3;
	}
	else
	{
		dev->CreateRenderTargetView(
			resource[0].Get(),
			&rtvDesc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				peraRTVHeap->GetCPUDescriptorHandleForHeapStart(), 0,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));

		//SRV�p�q�[�v�����
		heapDesc.NumDescriptors = 1;
	}
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;


	result = dev->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(peraSRVHeap.ReleaseAndGetAddressOf()));
	if (result != S_OK)
		assert(0);


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	if (threeResource)
	{
		//�V�F�[�_�[���\�[�X�r���[�����
		for (int i = 0; i < 3; i++)
		{
			dev->CreateShaderResourceView(
				resource[i].Get(),
				&srvDesc,
				CD3DX12_CPU_DESCRIPTOR_HANDLE(
					peraSRVHeap->GetCPUDescriptorHandleForHeapStart(), i,
					dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
			);
		}
	}
	else
	{
		dev->CreateShaderResourceView(
			resource[0].Get(),
			&srvDesc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				peraSRVHeap->GetCPUDescriptorHandleForHeapStart(), 0,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
		);
	}
	//�[�x�l�p�q�[�v�v���p�e�B
	//D3D12_HEAP_PROPERTIES depthHeapProp{};
	//depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	if (!noDepth)
	{
		//���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32_TYPELESS,
			resourceWidth,
			resourceHeight,
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

		//���\�[�X�ݒ�
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),//�A�b�v���[�h�s��
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
			IID_PPV_ARGS(&depthBuffer));
		assert(SUCCEEDED(result));

		//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
		dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
		result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
		assert(SUCCEEDED(result));

		//�[�x�r���[�쐬
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dev->CreateDepthStencilView(
			depthBuffer.Get(),
			&dsvDesc,
			dsvHeap->GetCPUDescriptorHandleForHeapStart());
	}
	if (threeResource)
	{
		for (int i = 0; i < 3; i++)
		{
			Texture::AddTexture(name + std::to_string(i), resource[i].Get());
		}
	}
	else
	{
		Texture::AddTexture(name, resource[0].Get());
	}

}

void TextureResource::PreDraw(const UINT arg_numRTD, const float topLeftX, const float topLeftY, const float width, const float height,
	const LONG& left, const LONG& top, const LONG& right, const LONG& bottom)
{
	numRTD = arg_numRTD;
	int bbIndex = 0;
	if (threeResource)
		bbIndex = TextureResource::bbindex;
	nowRenderTargets.push_back(this);

	//���\�[�X�o���A�ύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET));
	if (numRTD == 1)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapPointer;
		rtvHeapPointer = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			peraRTVHeap->GetCPUDescriptorHandleForHeapStart(), bbIndex,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
		D3D12_CPU_DESCRIPTOR_HANDLE dsvH;
		CD3DX12_VIEWPORT viewport;
		CD3DX12_RECT scissorRect;

		viewport = CD3DX12_VIEWPORT(topLeftX, topLeftY, width, height);
		scissorRect = CD3DX12_RECT(left, top, right, bottom);
		dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		cmdList->OMSetRenderTargets(1, &rtvHeapPointer, true, &dsvH);
		cmdList->RSSetViewports(1, &viewport);
		cmdList->RSSetScissorRects(1, &scissorRect);

		cmdList->ClearRenderTargetView(rtvHeapPointer, clearColor.data(), 0, nullptr);

		cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
	else
	{
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHeapPointer;
		rtvHeapPointer.resize(arg_numRTD);

		auto it = nowRenderTargets.end();
		it--;

		for (int i = arg_numRTD - 1; i >= 0; i--)
		{
			rtvHeapPointer[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				(*it)->peraRTVHeap->GetCPUDescriptorHandleForHeapStart(), bbIndex,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
			if (it == nowRenderTargets.begin())
				break;
			it--;
		}

		//rtvHeapPointer[0] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		//	mainResource->peraRTVHeap->GetCPUDescriptorHandleForHeapStart(), bbIndex,
		//	dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		//rtvHeapPointer[1] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		//	peraRTVHeap->GetCPUDescriptorHandleForHeapStart(), bbIndex,
		//	dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

		std::vector <CD3DX12_VIEWPORT> viewports;
		std::vector <CD3DX12_RECT> scissorRects;
		viewports.resize(arg_numRTD);
		scissorRects.resize(arg_numRTD);

		for (int i = 0; i < arg_numRTD; i++)
		{
			viewports[i] = CD3DX12_VIEWPORT(topLeftX, topLeftY, width, height);
			scissorRects[i] = CD3DX12_RECT(left, top, right, bottom);
		}

		auto dsvH = (*it)->dsvHeap->GetCPUDescriptorHandleForHeapStart();
		cmdList->OMSetRenderTargets(arg_numRTD, rtvHeapPointer.data(), false, &dsvH);
		cmdList->RSSetViewports(arg_numRTD, viewports.data());
		cmdList->RSSetScissorRects(arg_numRTD, scissorRects.data());

		cmdList->ClearRenderTargetView(rtvHeapPointer[arg_numRTD - 1], clearColor.data(), 0, nullptr);

		//cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
}

void TextureResource::DepthClear()
{
	auto dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}


void TextureResource::ResetRenderTarget()
{
	auto rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(peraRTVHeap->GetCPUDescriptorHandleForHeapStart(), 0, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	auto dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, resourceWidth, resourceHeight));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, resourceWidth, resourceHeight));

	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

}

void TextureResource::PostDraw(const bool renderTargetReset)
{
	int bbIndex = 0;
	if (threeResource)
		bbIndex = TextureResource::bbindex;

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	nowRenderTargets.pop_back();
	if (!renderTargetReset)
		return;
	if (nowRenderTargets.size() == 0)
	{
		DirectXLib::GetInstance()->ReRenderTarget();
	}
	else
	{
		auto it = nowRenderTargets.end();
		it--;
		(*it)->ResetRenderTarget();
	}
	//if (mainResource != this && numRTD == 1)
	//	mainResource->ResetRenderTarget();

}

