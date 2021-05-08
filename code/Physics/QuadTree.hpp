#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

class Collider;
enum class ColliderStatus;

template<class T> class List;
class Iterator;
template<class T> class Array;
template<class K, class V> class HashMap;
class GameObject;

class QuadTree : public ObjectBase {
private:

	class Node : public ObjectBase {
	private:
		void internalRemoveColliderFromList(const Iterator *it);
		void internalFreeCollider(Collider *collider);

	public:
		PUBLIC(LeftTop, NONE, Vector2)
		PUBLIC(Width, NONE, f32)
		PUBLIC(Height, NONE, f32)
		PUBLIC(MinWidth, NONE, f32)
		PUBLIC(MinHeight, NONE, f32)

		PUBLIC(Radius, NONE, f32)

PRIVATE(Tree, NONE, QuadTree*)

		PUBLIC(HalfWidth, NONE, f32)
		PUBLIC(HalfHeight, NONE, f32)
		PUBLIC(IsDivisible, NONE, bool)

		PUBLIC(Colliders, NONE, List<Collider*>*)
		PUBLIC(ExitingColliders, NONE, List<Collider*>*)
		PUBLIC(Children, NONE, Array<Node*>*)
		PUBLIC(LeftTopChildrenArray, NONE, Array<Vector2>*)

		PUBLIC(ChildrenCount, NONE, u32)

		PUBLIC(DynamicCollidersCount, NONE, u32)
		PUBLIC(StaticCollidersCount, NONE, u32)

		GENERATE_METADATA(CONSTRUCTOR, Node)

		void init(const Vector2 &leftTop, f32 width, f32 height, f32 minWidth, f32 minHeight, QuadTree *tree);
		Node* createChildNode(u32 index);
		void addCollider(Collider *collider);
		void update();
		void updateChildren();
		void checkExit(Collider *collider) const;
		void manageExits(List<Collider*> *exitingColliders);

		bool isLeaf() const;
		u32 getCollidersCount() const;
		bool childNodeTestPartialCollider(u32 index, Collider *collider) const;

		void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);

	};

	PRIVATE(Root, NONE, Node*);

	PRIVATE(Width, NONE, f32)
	PRIVATE(Height, NONE, f32)

	PRIVATE(Status, GET_SET, ColliderStatus)

public:

	GENERATE_METADATA(CONSTRUCTOR, QuadTree)

	void init(f32 size);

	void addCollider(Collider *collider);
	void update();

	void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);

};
}

