#include "DirectXLib.h"
#include"window.h"
#include"PtrDelete.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#include "ComputeWrapper.h"
#include "PostEffect.h"

#define InterlockedGetValue(object) InterlockedCompareExchange(object, 0, 0)

DirectXLib* DirectXLib::GetInstance()
{
	static DirectXLib instance;
	return &instance;
}

void DirectXLib::Initialize(Window* window)
{
	this->window = window;
	if (CreateDXGI() != S_OK) assert(0);
	if (CreateCommands() != S_OK) assert(0);
	if (CreateSwapChain() != S_OK) assert(0);
	if (CreateDesc() != S_OK) assert(0);
	if (CreateDSV() != S_OK) assert(0);
	//if(CreatePeraResurce() != S_OK)assert(0);
	if (CreateFence() != S_OK) assert(0);
	if (ImguiInitialize() != S_OK)assert(0);
}

HRESULT DirectXLib::CreateDXGI()
{
	HRESULT result;

	//DXGI �t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (result != S_OK) return result;

	// �A�_�v�^�[�̗񋓗p
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr < IDXGIAdapter> tmpAdapter;
	for (int i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);//���I�z��ɒǉ�����
	}
	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);  //�A�_�v�^�[�̏����擾
		std::wstring strDesc = adesc.Description;  //�A�_�v�^�[��
		//Microsoft Basic Render Diver �����
		if (strDesc.find(L"Microsoft") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];  //�̗p
			break;
		}
	}


	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;


	for (int i = 0; i < _countof(levels); i++)
	{
		// �f�o�C�X�𐶐�
		result = D3D12CreateDevice(nullptr, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
	return result;
}

HRESULT DirectXLib::CreateCommands()
{
	HRESULT result;

	for (int i = 0; i < backBuffNum; i++)
	{
		//�R�}���h�A���P�[�^���쐬
		result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&cmdAllocator[i]));
		if (result != S_OK) return result;
	}
	
	//�R�}���h���X�g���쐬
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator[bbIndex].Get(), nullptr, IID_PPV_ARGS(&cmdList));
	if (result != S_OK) return result;

	//�W���ݒ�ŃR�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	return result;
}

HRESULT DirectXLib::CreateSwapChain()
{
	HRESULT result;
	//�e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = window->GetWindowWidth();
	swapchainDesc.Height = window->GetWindowHeight();
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //�F���̏���
	swapchainDesc.SampleDesc.Count = 1; //�}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; //�o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = backBuffNum; //�o�b�t�@����3�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //�t���b�v��͔j��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	ComPtr<IDXGISwapChain1> swapchain1;
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(), window->GetHwnd(), &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)&swapchain1);
	assert(SUCCEEDED(result));

	swapchain1.As(&swapchain);
	bbIndex = swapchain->GetCurrentBackBufferIndex();

	return result;
}

HRESULT DirectXLib::CreateDesc()
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	if (FAILED(result)) {
		assert(0);
		return result;
	}


	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	//�e��ݒ�����ăf�B�X�N���v�^�q�[�v�𐶐�
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //�����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = swcDesc.BufferCount;   //3��
	result = dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	if (result != S_OK) return result;

	//3�̃o�b�t�@���ɂ���
	backBuffers.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers.size(); i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		if (result != S_OK) return result;
		//�f�B�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			i,
			dev->GetDescriptorHandleIncrementSize(heapDesc.Type));

		//�����_�[�^�[�Q�b�g�r���[�̍쐬
		dev->CreateRenderTargetView(backBuffers[i].Get(), nullptr, handle);
	}
	return result;
}

HRESULT DirectXLib::CreateFence()
{
	HRESULT result;
	//�t�F���X�̐���
	fence = nullptr;
	for(int i = 0;i<backBuffNum;i++)
	{
		fenceVal[i] = 0;
	}
	result = dev->CreateFence(fenceVal[bbIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	return result;
}

HRESULT DirectXLib::CreateDSV()
{

	HRESULT result;

	//�[�x�l�p�q�[�v�v���p�e�B
	//D3D12_HEAP_PROPERTIES depthHeapProp{};
	//depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		window->GetWindowWidth(),
		window->GetWindowHeight(),
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
	if (result != S_OK) return result;

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	if (result != S_OK) return result;

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	return result;
}

HRESULT DirectXLib::ImguiInitialize()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	result = dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&imguiHeap));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// �X���b�v�`�F�[���̏����擾
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	if (ImGui::CreateContext() == nullptr) {
		assert(0);
		return result;
	}
	if (!ImGui_ImplWin32_Init(window->GetHwnd())) {
		assert(0);
		return result;
	}
	if (!ImGui_ImplDX12_Init(
		GetDevice(),
		swcDesc.BufferCount,
		swcDesc.BufferDesc.Format,
		imguiHeap.Get(),
		imguiHeap->GetCPUDescriptorHandleForHeapStart(),
		imguiHeap->GetGPUDescriptorHandleForHeapStart()
	)) {
		assert(0);
		return result;
	}

	return result;
}

void DirectXLib::ImguiDraw()
{
	// imgui�J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DirectXLib::BeginDraw()
{

		//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, (float)window->GetWindowWidth(), (float)window->GetWindowHeight()));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, window->GetWindowWidth(), window->GetWindowHeight()));

	
	// 1.���\�[�X�o���A��ύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),  D3D12_RESOURCE_STATE_PRESENT,D3D12_RESOURCE_STATE_RENDER_TARGET));

	//2.��ʃN���A�R�}���h��������
	//auto rtvHeapPointer = CD3DX12_CPU_DESCRIPTOR_HANDLE(peraRTVHeap->GetCPUDescriptorHandleForHeapStart(), bbIndex, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	//cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);


	
	//�S��ʃN���A          R    G     B    A
	float clearColor[] = { 0.0f,0.0f,0.0f,1.0f };
	//cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void DirectXLib::DepthClear()
{
	//auto rtvHeapPointer = peraRTVHeap->GetCPUDescriptorHandleForHeapStart();
	//dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//cmdList->OMSetRenderTargets(1, &rtvHeapPointer, false, &dsvH);
	dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void DirectXLib::EndDraw()
{
	//cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(peraResource.Get(),
	//	D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	//dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//postEffect->Draw(peraSRVHeap.Get(), peraSRVHeap->GetGPUDescriptorHandleForHeapStart());
	
	// imgui�`��
	ImGui::Render();
	ID3D12DescriptorHeap* ppHeaps[] = { imguiHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList.Get());

	//4.���\�[�X�o���A��߂�
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	//���߂̃N���[�Y
	cmdList->Close();

	//�R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };//�R�}���h���X�g�̔z��
	cmdQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	//�o�b�t�@���t���b�v(���\�̓���ւ�)
	swapchain->Present(1, 0);

	const UINT64 currentFenceValue = fenceVal[bbIndex];
	//�R�}���h���X�g�̎��s������҂�
	cmdQueue->Signal(fence.Get(), currentFenceValue);

	//�o�b�N�o�b�t�@�̔ԍ����擾(3�Ȃ̂�0�`2)
	bbIndex = swapchain->GetCurrentBackBufferIndex();

	if (fence->GetCompletedValue() < fenceVal[bbIndex])
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal[bbIndex], event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	cmdAllocator[bbIndex]->Reset(); //�L���[���N���A
	cmdList->Reset(cmdAllocator[bbIndex].Get(), nullptr);		//�ĂуR�}���h���X�g�𒙂߂鏀��

	fenceVal[bbIndex] = currentFenceValue + 1;

	// DirectX ���t���[������ �����܂�

}

void DirectXLib::End()
{
	for (int i = 0; i < backBuffNum; i++)
	{
		cmdQueue->Signal(fence.Get(), fenceVal[i]);

		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal[i], event);
		WaitForSingleObjectEx(event, INFINITE, FALSE);

		CloseHandle(event);
	}
	//PtrDelete(postEffect);
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DirectXLib::ReRenderTarget()
{
	rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, (float)window->GetWindowWidth(), (float)window->GetWindowHeight()));
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, window->GetWindowWidth(), window->GetWindowHeight()));

	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
}

void DirectXLib::ComputeBegin()
{
	auto ComputeWrapperPtr = ComputeWrapper::GetInstance();
	ComputeWrapperPtr->AsyncComputeThreadProc();
	//InterlockedExchange(&ComputeWrapperPtr->m_threadFenceValue,fenceVal[bbIndex]);

	UINT64 threadFenceValue = InterlockedGetValue(&ComputeWrapperPtr->m_threadFenceValue);
	if (ComputeWrapperPtr->fence->GetCompletedValue() < threadFenceValue)
	{
		// Instruct the rendering command queue to wait for the current 
		// compute work to complete.
		auto result = cmdQueue->Wait(ComputeWrapperPtr->fence.Get(), threadFenceValue);
		assert(SUCCEEDED(result));
	}
}

