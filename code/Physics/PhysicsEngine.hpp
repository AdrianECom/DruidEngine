#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"

namespace DE {

class RigidBody;
class Collider;
class QuadTree;
class GameObject;
template<class T> class List;
class Iterator;

class PhysicsEngine: public ObjectBase, public Singleton<PhysicsEngine> {
private:

	PRIVATE(RigidBodies, NONE, List<RigidBody*>*)
	PRIVATE(RigidBodiesToFree, NONE, List<RigidBody*>*)
	PRIVATE(QuadTree, NONE, QuadTree*);

	PRIVATE(DebugColliders, GET_SET, bool)

	void internalRemoveRigidBody(const Iterator *it);

public:

	GENERATE_METADATA(CONSTRUCTOR, PhysicsEngine)

	void addRigidBody(RigidBody *rigidBody);
	void addCollider(RigidBody *rigidBody, Collider *collider);

	void rayCastQuery(const Vector3 &lineStart, const Vector3 &lineSnd, List<GameObject*> *outList);

	void init(f32 sceneSize);
	void step(f32 deltaTime);
	void updateContacts();

	void terminate();

};
}

