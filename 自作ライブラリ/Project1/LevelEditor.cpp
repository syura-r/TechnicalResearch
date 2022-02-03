#include "LevelEditor.h"
#include<fstream>
#include <memory>
#include <sstream>

#include "Book.h"
#include "Chair.h"
#include "Desk.h"
#include "ObjectManager.h"
#include "SetSquare30.h"
#include "SetSquare45.h"
#include "TeacherDesk.h"

LevelEditor* LevelEditor::GetInstance()
{
	static LevelEditor instance;
	return &instance;
}

void LevelEditor::Draw()
{
	ImGui::Begin("LevelEditor", nullptr, ImGuiWindowFlags_MenuBar);
	//���j���[�o�[���̏���
	if (ImGui::BeginMenuBar()) {
		//�t�@�C���^�u
		if (ImGui::BeginMenu("File"))
		{
			//�Z�[�u
			if (ImGui::BeginMenu("Save"))
			{
				for (int i = 0; i < 10; i++)
				{
					std::string name = "Slot" + std::to_string(i + 1);
					if (ImGui::MenuItem(name.c_str()))
					{
						Save(i);
					}
				}
				ImGui::EndMenu();
			}
			//���[�h
			if (ImGui::BeginMenu("Load"))
			{
				for (int i = 0; i < 10; i++)
				{
					std::string name = "Slot" + std::to_string(i + 1);
					if (ImGui::MenuItem(name.c_str()))
					{
						Load(i);
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		//�R���C�_�[�ǉ��^�u
		if (ImGui::BeginMenu("AddObject"))
		{
			//�֎q
			if (ImGui::Button("Chair"))
			{
				std::shared_ptr<Object> ptr(new Chair());
				objects.push_front(ptr);
				ObjectManager::GetInstance()->Add(ptr.get());
			}
			//��
			if (ImGui::Button("Desk"))
			{
				std::shared_ptr<Object> ptr(new Desk());
				objects.push_front(ptr);
				ObjectManager::GetInstance()->Add(ptr.get());
			}
			//����
			if (ImGui::Button("TeacherDesk"))
			{
				std::shared_ptr<Object> ptr(new TeacherDesk());
				objects.push_front(ptr);
				ObjectManager::GetInstance()->Add(ptr.get());
			}
			//�O�p��K30�x
			if (ImGui::Button("SetSquare30"))
			{
				std::shared_ptr<Object> ptr(new SetSquare30());
				objects.push_front(ptr);
				ObjectManager::GetInstance()->Add(ptr.get());
			}
			//�O�p��K45�x
			if (ImGui::Button("SetSquare45"))
			{
				std::shared_ptr<Object> ptr(new SetSquare45());
				objects.push_front(ptr);
				ObjectManager::GetInstance()->Add(ptr.get());
			}
			//�{
			if (ImGui::Button("Book"))
			{
				std::shared_ptr<Object> ptr(new Book());
				objects.push_front(ptr);
				ObjectManager::GetInstance()->Add(ptr.get());
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	int i = 0;
	//�I�u�W�F�N�g�ꗗ�\��
	for (auto& it : objects)
	{
		std::string name = typeid(*it).name() + std::to_string(i);
		if (ImGui::Button(name.c_str()))
		{
			if (nowSelectObject != nullptr)
				nowSelectObject->SetColor({ 1,1,1,1 });
			nowSelectObject = it;
			nowSelectObject->SetColor({ 1,0.3f,0.3f,1 });
		}
		i++;
	}
	ImGui::End();
	if (nowSelectObject == nullptr)
		return;

	//�I�𒆂̃I�u�W�F�N�g�ɂ���
	ImGui::Begin("SelectObjectState");
	float pos[3], scale[3], rot[3];
	//�|�W�V�����A�X�P�[���A���[�e�[�V�������擾
	memcpy(pos, &nowSelectObject->GetPosition(), sizeof(float) * 3);
	memcpy(scale, &nowSelectObject->GetScale(), sizeof(float) * 3);
	memcpy(rot, &nowSelectObject->GetRotation(), sizeof(float) * 3);
	ImGui::SliderFloat("SliderLength", &length, 0, 20);
	//�|�W�V����
	ImGui::Text("position");
	ImGui::SliderFloat("position_x", &pos[0], -10 * length + pos[0], 10 * length + pos[0]);
	ImGui::SliderFloat("position_y", &pos[1], -10 * length + pos[1], 10 * length + pos[1]);
	ImGui::SliderFloat("position_z", &pos[2], -10 * length + pos[2], 10 * length + pos[2]);
	//�X�P�[��
	ImGui::Text("scale");
	ImGui::SliderFloat("scale_x", &scale[0], -5 * length + scale[0], 5 * length + scale[0]);
	ImGui::SliderFloat("scale_y", &scale[1], -5 * length + scale[1], 5 * length + scale[1]);
	ImGui::SliderFloat("scale_z", &scale[2], -5 * length + scale[2], 5 * length + scale[2]);
	//���[�e�[�V����
	ImGui::Text("rotation");
	ImGui::SliderFloat("rotation_x", &rot[0], -10 * length + rot[0], 10 * length + rot[0]);
	ImGui::SliderFloat("rotation_y", &rot[1], -10 * length + rot[1], 10 * length + rot[1]);
	ImGui::SliderFloat("rotation_z", &rot[2], -10 * length + rot[2], 10 * length + rot[2]);
	//�R�s�[����
	if (ImGui::Button("Copy")) {
		//�I�𒆂̃I�u�W�F�N�g�̃N���X�����擾
		std::string name = typeid(*nowSelectObject).name();
		std::shared_ptr<Object> ptr = nullptr;
		//�N���X���ɉ������I�u�W�F�N�g���쐬
		if (name == "class Chair")
		{
			ptr = std::shared_ptr<Object>(new Chair());
		}
		else if (name == "class Desk")
		{
			ptr = std::shared_ptr<Object>(new Desk());
		}
		else if (name == "class TeacherDesk")
		{
			ptr = std::shared_ptr<Object>(new TeacherDesk());
		}
		else if (name == "class SetSquare30")
		{
			ptr = std::shared_ptr<Object>(new SetSquare30());
		}
		else if (name == "class SetSquare45")
		{
			ptr = std::shared_ptr<Object>(new SetSquare45());
		}
		else if (name == "class Book")
		{
			ptr = std::shared_ptr<Object>(new Book());
		}
		//�I�u�W�F�N�g���쐬����Ă�����
		if (ptr != nullptr)
		{
			//�|�W�V�����A�X�P�[���A���[�e�[�V������ݒ�
			ptr->SetPosition(nowSelectObject->GetPosition());
			ptr->SetScale(nowSelectObject->GetScale());
			ptr->SetRotation(nowSelectObject->GetRotation());
			ptr->Update();
			objects.push_front(ptr);
			//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
			ObjectManager::GetInstance()->Add(ptr.get());

		}
	}

	//�폜�{�^���������ꂽ��
	if (ImGui::Button("Delete")) {
		//�폜�t���O��ON��
		onDelete = true;
		//�I�u�W�F�N�g�}�l�[�W���[���폜
		ObjectManager::GetInstance()->Remove(nowSelectObject.get());
		//�G�f�B�^���̃I�u�W�F�N�g���X�g������폜
		objects.remove(nowSelectObject);
	}
	else
	{
		//�|�W�V�����A�X�P�[���A���[�e�[�V�����̍X�V
		nowSelectObject->SetPosition(pos);
		nowSelectObject->SetScale(scale);
		nowSelectObject->SetRotation(rot);
	}
	ImGui::End();
	
}

void LevelEditor::Clear()
{
	nowSelectObject.reset();
	for (auto& it : objects)
	{
		ObjectManager::GetInstance()->Remove(it.get());
		it.reset();
	}
	objects.clear();
}

void LevelEditor::Delete()
{
	//�폜�t���O�������Ă��Ȃ������瑁�����^�[��
	if(!onDelete)
		return;
	//2�t���[���ҋ@��폜
	if(deleteCounter>=2)
	{
		ObjectManager::GetInstance()->Remove(nowSelectObject.get());
		onDelete = false;
		nowSelectObject.reset();
		deleteCounter = 0;
	}
	deleteCounter++;
	
}

void LevelEditor::Save(const int slotNum)
{
	//�t�@�C���p�X�̎w��
	std::string filepath = "Resources/Map/Report"+std::to_string(slotNum)+".txt";
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ofstream file;
	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filepath.c_str(), std::ios_base::binary);
	assert(file.is_open());
	for (auto& it : objects)
	{
		//�N���X���̎擾
		std::string name = typeid(*it).name();
		name += "\n";
		file.write(name.c_str(), name.length());//�t�@�C���ɏ�������
		//�|�W�V�����̎擾
		Vector3 position = it->GetPosition();
		std::string write = "position ";
		write += std::to_string(position.x) + " " + std::to_string(position.y) + " " + std::to_string(position.z)+"\n";
		file.write(write.c_str(), write.length());//�t�@�C���ɏ�������
		//�X�P�[���̎擾
		Vector3 scale = it->GetScale();
		write = "scale ";
		write += std::to_string(scale.x) + " " + std::to_string(scale.y) + " " + std::to_string(scale.z) + "\n";
		file.write(write.c_str(), write.length());//�t�@�C���ɏ�������
		//���[�e�[�V�����̎擾
		Vector3 rot = it->GetRotation();
		write = "rotation ";
		write += std::to_string(rot.x) + " " + std::to_string(rot.y) + " " + std::to_string(rot.z) + "\n";
		file.write(write.c_str(), write.length());//�t�@�C���ɏ�������
	}
	file.close();

}

void LevelEditor::Load(const int slotNum)
{
	Clear();
	std::string filepath = "Resources/Map/Report" + std::to_string(slotNum) + ".txt";
	std::ifstream file;
	file.open(filepath.c_str());
	if(!file.is_open())
		return;
	std::string line;
	std::vector<Vector3>positions;
	std::vector<Vector3>scales;
	std::vector<Vector3>rotations;
	std::vector<std::shared_ptr<Object>>loadObjects;

	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪�������擾
		std::string key;
		getline(line_stream, key, ' ');

		//�N���X��
		if (key == "class")
		{
			std::string name;
			line_stream >> name;
			if(name == "Chair")
			{
				std::shared_ptr<Object> ptr(new Chair());
				loadObjects.push_back(ptr);
			}
			else if(name == "Desk")
			{
				std::shared_ptr<Object> ptr(new Desk());
				loadObjects.push_back(ptr);
			}
			else if (name == "TeacherDesk")
			{
				std::shared_ptr<Object> ptr(new TeacherDesk());
				loadObjects.push_back(ptr);
			}
			else if (name == "SetSquare30")
			{
				std::shared_ptr<Object> ptr(new SetSquare30());
				loadObjects.push_back(ptr);
			}
			else if (name == "SetSquare45")
			{
				std::shared_ptr<Object> ptr(new SetSquare45());
				loadObjects.push_back(ptr);
			}
			else if (name == "Book")
			{
				std::shared_ptr<Object> ptr(new Book());
				loadObjects.push_back(ptr);
			}

		}
		//�|�W�V����
		if (key == "position")
		{
			Vector3 position;
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			positions.push_back(position);
		}
		//�X�P�[��
		if (key == "scale")
		{
			Vector3 scale;
			line_stream >> scale.x;
			line_stream >> scale.y;
			line_stream >> scale.z;

			scales.push_back(scale);
		}
		//��]
		if (key == "rotation")
		{
			Vector3 rotation;
			line_stream >> rotation.x;
			line_stream >> rotation.y;
			line_stream >> rotation.z;

			rotations.push_back(rotation);
		}
	}
	file.close();

	int i = 0;
	for (auto& it : loadObjects)
	{
		it->SetPosition(positions[i]);
		it->SetScale(scales[i]);
		it->SetRotation(rotations[i]);
		objects.push_front(it);
		ObjectManager::GetInstance()->Add(it.get());
		i++;
	}
	
}
