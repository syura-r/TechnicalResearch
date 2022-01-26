//#pragma once
//#include <forward_list>
//
//#include "Object.h"
//
//class ColliderEditor
//{
//public:
//	static ColliderEditor* GetInstance();
//	void Update();
//	void Draw();
//	void ObjectDraw();
//	void Clear();
//	void Delete();
//private:
//	void Save(const int slotNum);
//	void Load(const int slotNum);
//	ColliderEditor() = default;
//	ColliderEditor(const ColliderEditor&) = delete;
//	~ColliderEditor() = default;
//	ColliderEditor& operator=(const ColliderEditor&) = delete;
//	std::shared_ptr<Object> nowSelectObject = nullptr;
//	std::forward_list<std::shared_ptr<Object>> objects;
//	//std::forward_list<std::shared_ptr<Object>> deleteLists;
//	bool onDelete = false;
//	int deleteCounter = 0;
//	float length = 1.0f;
//
//};
//
