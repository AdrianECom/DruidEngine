#pragma once

#include "Core/Basic.hpp"
#include "Core/Singleton.hpp"

namespace DE {
template<class T> class List;
class Scene;
class Script;
class GameObject;

class ScenesManager: public ObjectBase, public Singleton<ScenesManager> {

private:
	List<Scene*>* mScenes;
	PRIVATE(CurrentSceneIndex, NONE, u32)
	
	PRIVATE(CurrentScene, GET, Scene*)
	PRIVATE(SceneHasChanged, GET, bool)
	PRIVATE(GameObjectController, GET_SET, GameObject*)

	void internalLoadScene();
	void addScene(Scene *newScene);

public:

	GENERATE_METADATA(CONSTRUCTOR, ScenesManager)

	void init();
	void step();
	void loadCurrentScene();
	void setScene(u32 i);
};
}

