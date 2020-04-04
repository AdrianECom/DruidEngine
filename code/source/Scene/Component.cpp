#include "Component.h"

#include "GameObject.h"

namespace DE {

// ---------------------------------------------------------------------------

Component::Component() : DE_Class(){
	mGameObject = nullptr;
	mIsActive = true;
	mDestroy = false;
}

Component::~Component() = default;

// ---------------------------------------------------------------------------

bool Component::isStatic() {
	return mGameObject->isStatic();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
