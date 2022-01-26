#include "ObjectManager.h"

#include "Collision.h"
#include "CollisionManager.h"
#include "Object.h"
#include "PtrDelete.h"

ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager instance;
    return &instance;
}

void ObjectManager::Add(Object* object, bool preDraw)
{
	objects[preDraw].push_back(object);
}

void ObjectManager::Initialize()
{
	auto end_it = objects.end();
	for (auto it = objects.begin(); it != end_it; ++it)
	{
		auto end_itr = it->second.end();
		for (auto itr = it->second.begin(); itr != end_itr; ++itr)
		{
			(*itr)->Initialize();
		}
	}
}

void ObjectManager::Update()
{
	auto end_it = objects.end();
	for(auto it = objects.begin();it != end_it;++it)
	{
		auto end_itr = it->second.end();
		for(auto itr = it->second.begin();itr != end_itr;++itr)
		{
			(*itr)->Update();
		}
	}
}

void ObjectManager::Remove(Object* object)
{
	for (auto itr = objects.begin(); itr != objects.end(); ++itr)
	{
		for (auto it = itr->second.begin(); it != itr->second.end();)
		{
			if ((*it) == object)
			{
				it = itr->second.erase(it);
			}
			else
				++it;
		}
	}
}

void ObjectManager::DrawReady()
{
	//登録されているオブジェクトの使用パイプライン毎に配列に格納
	drawObjects.clear();
	for(auto it = objects.begin();it != objects.end();++it)
	{
		for(auto itr = it->second.begin();itr!=it->second.end();++itr)
		{
			//視錐台カリングの判定
			auto colliders = CollisionManager::GetInstance()->GetColliders(*itr);
			if (colliders == nullptr || Object3D::GetDrawShadow())
			{
				(*itr)->DrawReady();
				drawObjects[it->first][(*itr)->GetPipelineName()].push_back(*itr);
				continue;
			}
			bool draw = false;
			auto endIt = colliders->end();
			for (auto it = colliders->begin(); it != endIt; ++it)
			{
				Box box;
				auto max = (*it)->GetMax();
				auto min = (*it)->GetMin();
				if(min.x < max.x)
					box.minPosition.x = min.x;
				else
					box.minPosition.x = max.x;
				if (min.y < max.y)
					box.minPosition.y = min.y;
				else
					box.minPosition.y = max.y;
				if (min.z < max.z)
					box.minPosition.z = min.z;
				else
					box.minPosition.z = max.z;
				box.scale = { fabs(max.x - min.x),fabs(max.y - min.y),fabs(max.z - min.z) };
				//box.minPosition = min;
				Frustum frustum;
				frustum.CalculateFrustumPlanes(*Object3D::GetCamera());
				if (Collision::Detect(box, frustum))
				{
					draw = true;
					break;
				}
			}
			if (draw)
			{
				(*itr)->DrawReady();
				drawObjects[it->first][(*itr)->GetPipelineName()].push_back(*itr);
			}

		}
	}
}

void ObjectManager::PreDraw()
{
	//int num = 0;
	auto end_itr = drawObjects[true].end();
	for (auto itr = drawObjects[true].begin(); itr != end_itr; ++itr)
	{
		//パイプラインごとの配列になっているため配列の先頭時のみパイプラインを設定
		PipelineState::SetPipeline(itr->first);
		auto endObject_itr = itr->second.end();
		for (auto objectItr = itr->second.begin(); objectItr != endObject_itr; ++objectItr)
		{
			(*objectItr)->Draw();
		}
	}

}

void ObjectManager::PostDraw()
{
	auto end_itr = drawObjects[false].end();
	for (auto itr = drawObjects[false].begin(); itr != end_itr; ++itr)
	{
		PipelineState::SetPipeline(itr->first);
		auto endObject_itr = itr->second.end();
		for (auto objectItr = itr->second.begin(); objectItr != endObject_itr; ++objectItr)
		{
			(*objectItr)->Draw();
		}
	}
}

void ObjectManager::End()
{
	auto end_it = objects.end();
	for (auto it = objects.begin(); it != end_it; ++it)
	{
		auto end_itr = it->second.end();
		for (auto itr = it->second.begin(); itr != end_itr; ++itr)
		{
			PtrDelete(*itr);
		}
	}
	objects.clear();
}
