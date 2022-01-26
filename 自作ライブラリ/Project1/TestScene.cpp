#include "TestScene.h"
#include"PtrDelete.h"
#include"Object3D.h"
#include"imgui.h"
#include "Input.h"
using namespace Tree;

TestScene::TestScene()
{
	next = Ending;
	//���C�g����
	lightGroup = LightGroup::Create();
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup);
	//���C�g�F��ݒ�
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, XMFLOAT3(color0));

	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetSpotLightActive(0, false);
	lightGroup->SetCircleShadowActive(0, true);

	for (int i = 0; i < 5; i++)
	{
		wall[i] = new Object();
		wall[i]->Create(OBJLoader::GetModel("plain"));
	}
	wall[0]->SetPosition({ 640,320,320 });
	wall[0]->SetScale({ 640,640,1 });
	wall[0]->SetRotation({ 0,90,0 });
	wall[0]->SetColor({ 1, 1, 0, 0.2f });

	wall[1]->SetPosition({ 0,320,320 });
	wall[1]->SetScale({ 640,640,1 });
	wall[1]->SetRotation({ 0,-90,0 });
	wall[1]->SetColor({ 1, 0, 0, 0.2f });

	wall[2]->SetPosition({ 320,320,640 });
	wall[2]->SetScale({ 640,640,1 });
	wall[2]->SetColor({ 0, 1, 1, 0.2f });

	wall[3]->SetPosition({ 320,320,0 });
	wall[3]->SetScale({ 640,640,1 });
	wall[3]->SetColor({ 0, 1, 0, 0.2f });

	wall[4]->SetPosition({ 320,0,320 });
	wall[4]->SetScale({ 640,640,1 });
	wall[4]->SetRotation({ 90,0,0 });
	wall[4]->SetColor({ 0, 0, 1, 0.2f });

	for (int i = 0; i < 5; i++)
	{
		wall[i]->Update();
	}

}


TestScene::~TestScene()
{
	PtrDelete(lightGroup);
	for (int i = 0; i < 5; i++)
	{
		PtrDelete(wall[i]);
	}
}

void TestScene::Initialize()
{
	isEnd = false;
	if (!L8Tree.Initialize(
		g_PartitionLevel,
		Vector3(-60, -60, -60),
		Vector3(720, 1200, 720)))
		assert(0);

	for (cn = 0; cn < g_SphereNum; cn++)	// g_SphereNum�������𐶐�
	{
		SAry[cn].ID = cn;
		float r = 5 + 4 * (float)rand() / RAND_MAX;
		SAry[cn].r = r;// ���̔��a
		SAry[cn].SetScale({ r,r,r });
		SAry[cn].p.x = 42 + 30.5f * (float)rand() / RAND_MAX;		// �ג��������ʒu
		SAry[cn].p.y = 10 + 800 * (float)cn / g_SphereNum;	// ���\�����Ƃ��납�痎�Ƃ��܂�
		if (cn % 2)
			SAry[cn].p.z = 11 + 30.5f * (float)rand() / RAND_MAX;
		else
			SAry[cn].p.z = 550 + 30.5f * (float)rand() / RAND_MAX;
		SAry[cn].v.x = 0.0;		// �����i�K���j
		SAry[cn].v.y = 0.0;//-cn/80.0f;
		if (cn % 2)
			SAry[cn].v.z = -0.0f;
		else
			SAry[cn].v.z = 0.0f;
		SAry[cn].scale = SAry[cn].r;	
		SAry[cn].w = SAry[cn].r * SAry[cn].r * SAry[cn].r;	// ���ʂ͔��a��3��ɔ��Ƃ��܂�
		// OFT�ɓo�^
		Tree::TreeObject<SPHERE>* p = new Tree::TreeObject<SPHERE>;
		p->pObject = &SAry[cn];	// �o�^
		spSOFTAry[cn].SetPtr(p);
	}

}

void TestScene::Update()
{
	lightGroup->SetAmbientColor(XMFLOAT3(coloramb));
	lightGroup->SetDirLightDir(0, { lightDir[0],lightDir[1],lightDir[2],1 });
	lightGroup->Update();


	if(Input::TriggerKey(DIK_SPACE))
	{
		for (cn = 0; cn < g_SphereNum; cn++)
		{
			SAry[cn].v.y = rand() % 300 * 0.1f;
		}
	}
	if (Input::TriggerKey(DIK_RETURN))
		drawWall = drawWall == false ? true : false;
	// ���̈ʒu���X�V���ăc���[�ɍēo�^
	for (cn = 0; cn < g_SphereNum; cn++)
	{
		SPHERE* pTmp = spSOFTAry[cn]->pObject;
		GetNextSpherePos(*pTmp);	// ���̈ړ��ʒu��������
		spSOFTAry[cn]->Remove();		// ��x���X�g����O���
		// �ēo�^
		Vector3 tmpMin(pTmp->p.x - pTmp->r, pTmp->p.y - pTmp->r, pTmp->p.z - pTmp->r);
		Vector3 tmpMax(pTmp->p.x + pTmp->r, pTmp->p.y + pTmp->r, pTmp->p.z + pTmp->r);
		L8Tree.Regist(tmpMin, tmpMax, spSOFTAry[cn]);
	}
	
	collisionCount = L8Tree.GetAllCollisionList(ColVect);
#ifdef _DEBUG
	static int count = 0;
	count++;
	if (count == 60)
	{
		printf("����� : %d  �y���� : %f��\n", collisionCount, (1.0f - (double)collisionCount / 999000) * 100);
		count = 0;
	}
#endif
	// �Փ˔���
	DWORD c;
	auto colCount = collisionCount / 2;	// 2�Ŋ���̂̓y�A�ɂȂ��Ă���̂�
	for (c = 0; c < colCount; c++) {
		float r2 = (ColVect[c * 2]->r + ColVect[c * 2 + 1]->r) * (ColVect[c * 2]->r + ColVect[c * 2 + 1]->r);
		float x = (ColVect[c * 2]->p.x - ColVect[c * 2 + 1]->p.x);
		float y = (ColVect[c * 2]->p.y - ColVect[c * 2 + 1]->p.y);
		float z = (ColVect[c * 2]->p.z - ColVect[c * 2 + 1]->p.z);
		if (r2 >= x * x + y * y + z * z)
		{
			// 2���Փˏ���������
			SphereColProc(ColVect[c * 2], ColVect[c * 2 + 1]);
		}
	}

	
	for (cn = 0; cn < g_SphereNum; cn++) {
		SAry[cn].SetPosition(SAry[cn].p);
		SAry[cn].Update();
	}

}

void TestScene::PreDraw()
{
	
	ImGui::Begin("Explanation");
	static int count = 0;
	count++;
	static DWORD a = 0;
	if (count == 60)
	{
		a = collisionCount;
		count = 0;
	}
	//char str[50];
	//sprintf_s(str, 50, "collisionCount : %d  percent : %f��", a, (1.0f - (double)a / 999000) * 100);
	//ImGui::Text(str);
	ImGui::Text("SPACE : Shake");
	ImGui::Text("MouseRightButton + Drag : CameraRotation");
	ImGui::Text("MouseMiddleButton + Drag : CameraMove");
	ImGui::Text("MouseWhile : CameraZoom");

	ImGui::End();

	for (cn = 0; cn < g_SphereNum; cn++) {
		SAry[cn].Draw();
	}
	if(drawWall)
	for (int i = 0; i < 5; i++)
	{
		wall[i]->CustomDraw();
	}

}

void TestScene::PostDraw()
{
}

void TestScene::SphereColProc(SPHERE* s1, SPHERE* s2)
{
	float t = 0;
	Vector3 C1ColPos, C2ColPos, C1Velo, C2Velo;

	// �Փ˂��Ă���2�~�̏Փˈʒu�����o
	if (!CheckParticleCollision(
		s1->r, s2->r,
		&s1->Pre_p, &s1->p,
		&s2->Pre_p, &s2->p,
		&t,
		&C1ColPos,
		&C2ColPos))
		return;	// �Փ˂��Ă��Ȃ��悤�ł�

	// �Փˈʒu��O�ʒu�Ƃ��ĕۑ�
	s1->p = C1ColPos;
	s2->p = C2ColPos;
	s1->Pre_p = C1ColPos;
	s2->Pre_p = C2ColPos;

	// �Փˌ�̑��x���Z�o
	if (!CalcParticleColliAfterPos(
		&C1ColPos, &s1->v,
		&C2ColPos, &s2->v,
		s1->w, s2->w,
		g_Circle_Ref, g_Circle_Ref,		// ���̔����W��
		t,
		&C1ColPos, &C1Velo,
		&C2ColPos, &C2Velo))
		return; // �������s�����悤�ł�

	// �Փˌ�ʒu�Ɉړ�
	s1->v = C1Velo;
	s2->v = C2Velo;
	s1->p += s1->v;
	s2->p += s2->v;

}
