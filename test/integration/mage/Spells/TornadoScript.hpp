#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include "Element.hpp"

namespace DE {

class Renderer;

class TornadoScript : public Script {

private:
PRIVATE(Renderer, NONE, Renderer*)
	PRIVATE(Element, NONE, Element)

	PRIVATE(DurationTime, NONE, f32)
	PRIVATE(DurationTimeCounter, NONE, f32)

public:

	GENERATE_METADATA(CONSTRUCTOR, TornadoScript)

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void onEnterCollision(GameObject *otherGameObject) override;
	void onCollision(GameObject *otherGameObject) override;
	void onExitCollision(GameObject *otherGameObject) override;

};
}

