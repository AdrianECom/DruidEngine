#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include "Element.hpp"

namespace DE {

class UIText;
class Renderer;
class RigidBody;

class EnemyScript : public Script {

private:
PRIVATE(Renderer, NONE, Renderer*)
PRIVATE(RigidBody, NONE, RigidBody*)
	PRIVATE(Element, NONE, Element)
	PRIVATE(Falling, NONE, bool)
	PRIVATE(SlowDown, NONE, bool)
	PRIVATE(Speed, NONE, f32)

public:

	GENERATE_METADATA(CONSTRUCTOR, EnemyScript)

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void onEnterCollision(GameObject *otherGameObject) override;
	void onCollision(GameObject *otherGameObject) override;
	void onExitCollision(GameObject *otherGameObject) override;

	void createProjectile(f32 x, f32 y, f32 clickX, f32 clickY);

	void setElement(Element element) {
		mElement = element;
	}

};
}

