#pragma once

#include "Scene/Component.hpp"

namespace DE {

class GameObject;

class Script : public Component {
private:

	 bool mFirstStepDone;

public:

	GENERATE_METADATA(Script);

	Script();
	virtual ~Script() override;;

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

