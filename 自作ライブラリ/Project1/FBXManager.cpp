#include "FBXManager.h"
#include"FbxLoader.h"
std::unordered_map<std::string, FBXModel*> FBXManager::models = {};
void FBXManager::LoadModelFile(const std::string& keyName, const std::string& fileName, const bool& smooth)
{
	//���Ƀ��f�����o�^����Ă�����ǉ������ɏI��
	if(models[keyName] != nullptr)
		return;
	
	FBXModel* model = FbxLoader::GetInstance()->LoadModelFromFile(fileName);

	models[keyName] = model;
}

void FBXManager::DeleteModels()
{
	for (auto& itr : models)
	{
		delete itr.second;
		itr.second = nullptr;
	}
}

void FBXManager::AddAnimationList(const std::string& modelName, const std::string& animationName, const int startFrame, const int endFrame)
{
	models[modelName]->AddAnimation(animationName, startFrame, endFrame);
}

void FBXManager::AddAnimationList(FBXModel* model, const std::string& animationName, const int startFrame, const int endFrame)
{
	model->AddAnimation(animationName, startFrame, endFrame);
}

