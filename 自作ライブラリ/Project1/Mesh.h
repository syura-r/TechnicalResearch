#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Material.h"
#include <vector>
#include <unordered_map>

class Mesh
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�\����
	//���_�f�[�^���̍\����
	struct VERTEXSTATUS
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

public://�����o�֐�
	inline const std::string& GetName() { return name; }
	void SetName(const std::string& name) { this->name = name; }
	Material* GetMaterial() { return material; }
	void SetMaterial(Material* material) { this->material = material; }
	void AddVertex(const VERTEXSTATUS& vertex) { vertices.emplace_back(vertex); }
	void AddIndex(unsigned short index) { indices.emplace_back(index); }
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView; }
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView; }
	inline size_t GetVertexCount() { return vertices.size(); }
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
	{
		smoothData[indexPosition].emplace_back(indexVertex);
	}

	void CreateBuffers();
	void Draw();
	void CalculateSmoothedVertexNormals();

	//���_�z����擾
	inline const std::vector< VERTEXSTATUS>& GetVertices() { return vertices; }
	//�C���f�b�N�X�z����擾
	inline const std::vector<unsigned short>&GetIndices() { return indices; }

private://�����o�ϐ�
	std::string name;
	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	std::vector<VERTEXSTATUS> vertices;
	std::vector<unsigned short> indices;
	Material* material = nullptr;
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;

};

