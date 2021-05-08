#pragma once

#include "Scene/Component.hpp"

namespace DE {

class GameObject;

class Script : public Component {
private:

	PRIVATE(FirstStepDone, NONE, bool)

public:

	GENERATE_METADATA(CONSTRUCTOR, Script)

	GET(FirstStepDone);

	virtual void init();
	virtual void firstStep();
	virtual void step();
	virtual void terminate();

	virtual void onEnterCollision(GameObject *otherGameObject);
	virtual void onCollision(GameObject *otherGameObject);
	virtual void onExitCollision(GameObject *otherGameObject);

	void firstStepDone() {
		mFirstStepDone = true;
	}
};
}

