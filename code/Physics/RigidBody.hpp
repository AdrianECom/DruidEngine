#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

class Collider;

class RigidBody : public Component {
private:

	class State {
	public:
		PUBLIC(Position, NONE, Vector3)
		PUBLIC(Linear, NONE, Vector3)
		PUBLIC(ForceAccumulator, NONE, Vector3)
		PUBLIC(Mass, NONE, f32)
		PUBLIC(Simulate, NONE, bool)

		State();
	};

	PRIVATE(State, NONE, State)
	PRIVATE(LastCollisionPosition, NONE, Vector3)
	PRIVATE(AntiPenetrationForce, NONE, Vector3)

	PRIVATE(ForceAccumulator, GET, Vector3)
	PRIVATE(Mass, GET_SET, f32)
	PRIVATE(Linear, GET_SET, Vector3)
	PRIVATE(Simulate, GET_SET, bool)
	PRIVATE(Collider, GET_SET, Collider*)

public:

	GENERATE_METADATA(CONSTRUCTOR, RigidBody)

	void init() override;

	void integrate(f32 deltaTime);
	void stopMovement();

	void saveState();
	void restoreState();

	void addForce(const Vector3 &force) {
		mForceAccumulator.add(force);
	}

	void addLinear(const Vector3 &linear) {
		mLinear.add(linear);
	}

	void addAntiPenetrationForce(Vector3 newAntiPenetrationForce) {
		mAntiPenetrationForce.add(newAntiPenetrationForce);
	};

	void resetAntiPenetrationForce() {
		mAntiPenetrationForce.set(0, 0, 0);
	};

	bool isSleeping();

};
}

