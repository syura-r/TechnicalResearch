#include "OBJModel.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include"Texture.h"
#include"DirectXLib.h"
using namespace std;

const std::string OBJModel::basePath = "Resources/OBJ/";

OBJModel * OBJModel::CreateOBJ(const std::string & name, bool smooth)
{
	OBJModel* instance = new OBJModel;
	instance->Initialize(name, smooth);
	return instance;
}

OBJModel::~OBJModel()
{
	for (auto m : meshes)
	{
		delete m;
	}
	meshes.clear();

	for (auto m : materials)
	{
		delete m.second;
	}
	materials.clear();
}

void OBJModel::Initialize(const std::string & name, bool smooth)
{
	std::ifstream file;
	file.open(basePath + name);
	if (file.fail())
	{
		assert(0);
	}

	this->name = name;

	Mesh* mesh = new Mesh;
	int indexCountTex = 0;
	int indexCountNoTex = 0;

	vector<XMFLOAT3>positions;//���_���W
	vector<XMFLOAT3>normals;//�@���x�N�g��
	vector<XMFLOAT2>texcoords;//�e�N�X�`��UV

	//1�s���ǂݍ���
	string line;
	while (getline(file,line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪�������擾
		string key;
		getline(line_stream, key, ' ');

		//�}�e���A��
		if (key == "mtllib")
		{
			string filename;
			line_stream >> filename;
			LoadMaterial(filename);
		}
		//�擪������g�Ȃ�O���[�v�̊J�n
		if (key == "g")
		{
			if (mesh->GetName().size() > 0)
			{
				//������
				if (smooth)
				{
					mesh->CalculateSmoothedVertexNormals();
				}
				meshes.emplace_back(mesh);
				mesh = new Mesh;
				indexCountTex = 0;
			}
			
			string groupName;
			line_stream >> groupName;

			//���b�V���ɖ��O���Z�b�g
			mesh->SetName(groupName);
		}

		//�擪������v�Ȃ璸�_���W
		if (key == "v")
		{
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);
		}
		//�擪������vt�Ȃ�e�N�X�`��
		if (key == "vt")
		{
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		//�擪������vn�Ȃ�@���x�N�g��
		if (key == "vn")
		{
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			normals.emplace_back(normal);
		}
		//�擪������usemtl�Ȃ�}�e���A�������蓖�Ă�
		if (key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr)
			{
				//�}�e���A����
				string materialName;
				line_stream >> materialName;

				auto itr = materials.find(materialName);
				if (itr != materials.end())
				{
					mesh->SetMaterial(itr->second);
				}
			}
		}
		//�擪������f�Ȃ�|���S��(�O�p�`)
		if (key == "f")
		{
			int faceIndexCount = 0;
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream,index_string,' '))
			{
				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				//���_�ԍ�
				index_stream >> indexPosition;

				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, ios_base::cur);
				
				if (material && material->textureFileName.size() > 0)
				{
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur);
					index_stream >> indexNormal;
					//���_�f�[�^�̒ǉ�
					Mesh::VERTEXSTATUS vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					mesh->AddVertex(vertex);
					//�G�b�W�������p�̃f�[�^��ǉ�
					if (smooth)
					{
						//v�L�[(���W�f�[�^)�̔ԍ��ƁA�S�č����������_�̃C���f�b�N�X���Z�b�g�œo�^
						mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
					}
				}
				else
				{
					char c;
					index_stream >> c;
					//�X���b�V��2�A���̏ꍇ�A���_�ԍ��̂�
					if (c == '/')
					{
						//���_�f�[�^�̒ǉ�
						Mesh::VERTEXSTATUS vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0,0,1 };
						vertex.uv = { 0,0 };
						mesh->AddVertex(vertex);
						//�G�b�W�������p�̃f�[�^��ǉ�
						if (smooth)
						{
							mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
						}
					}
					else
					{
						index_stream.seekg(-1, ios_base::cur);
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur);
						index_stream >> indexNormal;
						//���_�f�[�^�̒ǉ�
						Mesh::VERTEXSTATUS vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0,0 };
						mesh->AddVertex(vertex);
						if (smooth)
						{
							mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
						}
					}
				}
				if (faceIndexCount >= 3)
				{
					mesh->AddIndex(indexCountTex - 1);
					mesh->AddIndex(indexCountTex);
					mesh->AddIndex(indexCountTex - 3);
				}
				else
				{
					mesh->AddIndex(indexCountTex);
				}
				indexCountTex++;
				faceIndexCount++;
			}

		}
	}
	file.close();

	if (smooth)
	{
		mesh->CalculateSmoothedVertexNormals();
	}
	//�R���e�i�ɓo�^
	meshes.emplace_back(mesh);	
	//���b�V���̃}�e���A���`�F�b�N
	for (auto& m : meshes)
	{
		if (m->GetMaterial() == nullptr)
		{
			if (defaultMaterial == nullptr)
			{
				//�f�t�H���g�}�e���A���𐶐�
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			//�f�t�H���g�}�e���A�����Z�b�g
			m->SetMaterial(defaultMaterial);
		}
	}

	//���b�V���̃o�b�t�@����
	for (auto& m : meshes)
	{
		m->CreateBuffers();
	}

	//�}�e���A���̐��l��萔�o�b�t�@�ɔ��f
	for (auto& m : materials)
	{
		m.second->Update();
	}
	// �f�X�N���v�^�q�[�v����
	CreateDescriptorHeap();

	//�e�N�X�`���̓ǂݍ���
	Loadtextures();
}

void OBJModel::Draw()
{
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	//DirectXLib::GetInstance()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	for (auto& mesh : meshes)
	{
		mesh->Draw();
	}
	
}

void OBJModel::LoadMaterial(const std::string & name)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(basePath + name);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	Material* material = nullptr;

	//1�s���ǂݍ���
	string line;
	while (getline(file,line))
	{
		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		//�擪�̃^�u�����͖�������
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		if (key == "newmtl")
		{
			if (material)
			{
				AddMaterial(material);
			}
			// �V�����}�e���A���𐶐�
			material = Material::Create();
			// �}�e���A�����ǂݍ���
			line_stream >> material->name;
		}
		// �擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka") {
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}
		// �擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd") {
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}
		// �擪������Ks�Ȃ�X�y�L�����[�F
		if (key == "Ks") {
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}
		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material->textureFileName;

			// �t���p�X����t�@�C���������o��
			size_t pos1;
			pos1 = material->textureFileName.rfind('\\');
			if (pos1 != string::npos) {
				material->textureFileName = material->textureFileName.substr(pos1 + 1, material->textureFileName.size() - pos1 - 1);
			}

			pos1 = material->textureFileName.rfind('/');
			if (pos1 != string::npos) {
				material->textureFileName = material->textureFileName.substr(pos1 + 1, material->textureFileName.size() - pos1 - 1);
			}
		}
	}
	// �t�@�C�������
	file.close();

	if (material) {
		// �}�e���A����o�^
		AddMaterial(material);
	}
}

void OBJModel::AddMaterial(Material * material)
{
	materials.emplace(material->name, material);
}

void OBJModel::CreateDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// �}�e���A���̐�
	size_t count = materials.size();
	// �f�X�N���v�^�q�[�v�𐶐�	
	if (count > 0) {
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
		descHeapDesc.NumDescriptors = (UINT)count + 1; // �V�F�[�_�[���\�[�X�r���[�̐�
		result = DirectXLib::GetInstance()->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
		if (FAILED(result)) {
			assert(0);
		}
	}
}

void OBJModel::Loadtextures()
{
	for (auto& m : materials)
	{
		Material* material = m.second;
		material->LoadTexture();
	}
}
