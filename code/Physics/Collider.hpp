#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector2.hpp"

namespace DE {

template<class T> class Array;
class RigidBody;
class ContactsManager;
class Contact;

enum class ColliderStatus {
	STATUS_NONE, STATUS_COLLISION, STATUS_PENETRATION
};

enum class ColliderShape {
	RECTANGLE, SPHERE
};

class Collider : public Component {
private:

	PRIVATE(BoxVertices, NONE, Array<Vector2>*)

	PRIVATE(HalfWidth, NONE, f32)
	PRIVATE(HalfHeight, NONE, f32)
	PRIVATE(Status, NONE, ColliderStatus)
	
	static f32 msDepthEpsilon;

	PRIVATE(Radius, GET, f32)
	PRIVATE(Height, GET, f32)
	PRIVATE(Width, GET, f32)
	PRIVATE(PositionOffset, GET_SET, Vector3)
	PRIVATE(RigidBody, GET_SET, RigidBody*)
	PRIVATE(CollisionLayer, GET_SET, f32)
	PRIVATE(Shape, GET_SET, ColliderShape)
	PRIVATE(IsSolid, GET_SET, bool)
	PRIVATE(LastContact, GET, Contact*)
	PRIVATE(IsPenetrated, GET, bool)
	PRIVATE(HasSizeChanged, GET, bool)

public:

	GENERATE_METADATA(CONSTRUCTOR, Collider)

	void init() override;

	Vector3 getCenter() const;
	void setSize(f32 width, f32 height);
	void resetHasSizeChaged() { mHasSizeChanged = false; }
	Array<Vector2>* getBoundingBox(bool forceCalculateBoundingBox = false);
	Vector3 getRelativeVelocity(Collider *otherCollider);

	void markPenetratedBy(Collider* otherCollider);
	void unmarkPenetrated();
	bool isSimulate();

	bool checkCollisionRadius(Collider *otherCollider) const;

	ColliderStatus testCollider(Collider *otherCollider);
	ColliderStatus testRectangleRectangle(Collider *otherCollider);
	ColliderStatus testRectangleSphere(Collider *otherCollider);
	ColliderStatus testSphereSphere(Collider *otherCollider);

	ColliderStatus testPoint(Vector2 point);

	void render();

	virtual void save(ConfigMap* configMap, StringRef objectName) override;
	virtual void load(ConfigMap* configMap, StringRef objectName) override;
};
}

