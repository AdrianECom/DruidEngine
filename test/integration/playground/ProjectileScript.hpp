#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include "Element.hpp"

namespace DE {

class Renderer;

class ProjectileScript : public Script {

private:
PRIVATE(Renderer, NONE, Renderer*)
	PRIVATE(Element, NONE, Element)

	PRIVATE(ExplosionTime, NONE, f32)
	PRIVATE(ExplosionTimeCounter, NONE, f32)
	PRIVATE(IsExploding, NONE, bool)

	static u32 smProjectileIndex;

public:

	GENERATE_METADATA(CONSTRUCTOR, ProjectileScript)

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void onEnterCollision(GameObject *otherGameObject) override;
	void onCollision(GameObject *otherGameObject) override;
	void onExitCollision(GameObject *otherGameObject) override;

	void setElement(Element element) {
		mElement = element;
	}
	Element getElement() const {
		return mElement;
	}

	void explode();
	bool isExploded() const {
		return mIsExploding;
	}

};
}

