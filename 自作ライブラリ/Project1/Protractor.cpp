#include "Protractor.h"

#include <fstream>

#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "DrawMode.h"
#include "OBJLoader.h"
#include "PtrDelete.h"
#include "Texture.h"
//分度器のクラスだけど使ってない
Protractor::Protractor()
{
	Create(OBJLoader::GetModel("protractor"));
	position = { 0,-5,-3.7f };
	scale = { 1 };
	Object::Update();
	for (int i = 0; i < 8; i++)
	{
		auto adjustScale = scale;//scaleが5の時を基準に調節した値だからscaleを5で割ってる
		const auto colOffset = XMVECTOR{ colliderOffset[i][0] * adjustScale.x,colliderOffset[i][1] * adjustScale.y,colliderOffset[i][2] * adjustScale.z,0 };
		const Vector3 colScale = Vector3{ colliderScale[i][0], colliderScale[i][1], colliderScale[i][2] } *adjustScale;
		BoxCollider* col = new BoxCollider(colOffset, colScale);
		col->SetObject(this);
		col->SetMove(true);
		col->SetRotation({ colliderRotation[i][0],colliderRotation[i][1],colliderRotation[i][2] });
		col->SetAttribute(COLLISION_ATTR_LANDSHAPE);
		col->Update();
		CollisionManager::GetInstance()->AddCollider(col);
		colliders.push_back(col);
	}
}

Protractor::~Protractor()
{
	std::string filepath = "Resources/Map/ProtractorCollider.txt";
	//ファイル入力ストリームのインスタンス
	std::ofstream file;
	//.wavファイルをバイナリモードで開く
	file.open(filepath.c_str(), std::ios_base::binary);
	assert(file.is_open());
	//std::string size = "size : " + std::to_string(objects.max_size()) + "\n";
	//file.write(size.c_str(), size.length());

	for (int i = 0; i < 8; i++)
	{
		Vector3 position = colliderOffset[i];
		std::string write = "offset ";
		write += std::to_string(position.x) + "f, " + std::to_string(position.y) + "f, " + std::to_string(position.z) + "f,\n";
		file.write(write.c_str(), write.length());
	}
	for (int i = 0; i < 8; i++)
	{
		Vector3 scale = colliderScale[i];
		std::string write = "scale ";
		write += std::to_string(scale.x) + "f, " + std::to_string(scale.y) + "f, " + std::to_string(scale.z) + "f,\n";
		file.write(write.c_str(), write.length());
	}
	for (int i = 0; i < 8; i++)
	{
		Vector3 rot = colliderRotation[i];
		std::string write = "rotation ";
		write += std::to_string(rot.x) + "f, " + std::to_string(rot.y) + "f, " + std::to_string(rot.z) + "f,\n";
		file.write(write.c_str(), write.length());
	}

	file.close();

	for (auto it = colliders.begin(); it != colliders.end(); ++it)
	{
		CollisionManager::GetInstance()->RemoveCollider(*it);
		PtrDelete(*it);
	}
}

void Protractor::Initialize()
{
}

void Protractor::Update()
{
	if (hit)
	{
		color = { 1,1,1,1 };
		hit = false;
	}
	Object::Update();
	for (auto it = colliders.begin(); it != colliders.end(); ++it)
	{
		(*it)->Update();
	}
}

void Protractor::Draw()
{
#ifdef _DEBUG
	if (!Object3D::GetDrawShadow() && DrawMode::GetDrawImGui())
	{
	ImGui::Begin("ProtractorState");
	for (int i = 0; i < 8; i++)
	{
		std::string name = "colliderOffset" + std::to_string(i);
		std::string name2 = "colliderScale" + std::to_string(i);
		std::string name3 = "colliderRotation" + std::to_string(i);

		ImGui::SliderFloat3(name.c_str(), colliderOffset[i], -5, 5);
		colliders[i]->SetOffset({ colliderOffset[i][0],colliderOffset[i][1],colliderOffset[i][2] });
		ImGui::SliderFloat3(name2.c_str(), colliderScale[i], 0, 5);
		colliders[i]->SetScale({ colliderScale[i][0],colliderScale[i][1],colliderScale[i][2] });
		ImGui::SliderFloat3(name3.c_str(), colliderRotation[i], 0, 360);
		colliders[i]->SetRotation({ colliderRotation[i][0],colliderRotation[i][1],colliderRotation[i][2] });
	}
	ImGui::End();
	}
#endif

	//Object::Draw();
	auto cmdList = DirectXLib::GetInstance()->GetCommandList();
	cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	if (Object3D::GetDrawShadow())
	{
		// パイプラインステートの設定
		PipelineState::SetPipeline("PBShadowMap");
		cmdList->SetGraphicsRootConstantBufferView(1, constBuff[bbIndex]->GetGPUVirtualAddress());
	}
	else
	{
		// パイプラインステートの設定
		PipelineState::SetPipeline("PolygonBreak");
		cmdList->SetGraphicsRootConstantBufferView(4, constBuff[bbIndex]->GetGPUVirtualAddress());
		cmdList->SetGraphicsRootConstantBufferView(5, constCameraBuff[bbIndex]->GetGPUVirtualAddress());
		cmdList->SetGraphicsRootDescriptorTable(6, Texture::GetGpuDescHandleSRV("shadowMap" + std::to_string(bbIndex)));  //ヒープの先頭が定数バッファ
	}
	CustomDraw(false, true, ALPHA, true);

}
void Protractor::OnCollision(const CollisionInfo& info)
{
}
