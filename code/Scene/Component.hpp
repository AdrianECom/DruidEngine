#pragma once

#include "Core/ObjectBase.hpp"

namespace DE {

class GameObject;
class ConfigMap;

class Component : public ObjectBase {

private:

	PRIVATE(IsActive, NONE, bool)
	PRIVATE(IsStatic, NONE, bool)

	PRIVATE(GameObject, GET_SET, GameObject*)
	PRIVATE(AlreadyAddedToEngine, GET_SET, bool)
	PRIVATE(IsPendingToBeDestroyed, GET, bool)
	PRIVATE(IsDestroyed, GET, bool)

public:

	GENERATE_METADATA(CONSTRUCTOR, Component)

	virtual void init() = 0;

	bool isStatic();

	bool isActive() const {
		return (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : mIsActive;
	};

	void setIsActive(bool isActive) {
		mIsActive = (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : isActive;
	};

	void finallyDestroy() {
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	};

	void destroy() {
		mIsPendingToBeDestroyed = true;
		mIsActive = false;
		onDestroy();
	};

	virtual void onDestroy() { };

	virtual void save(ConfigMap* configMap, StringRef objectName);
	virtual void load(ConfigMap* configMap, StringRef objectName);
};
}

