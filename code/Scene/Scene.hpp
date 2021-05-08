#pragma once

#include "Core/ObjectBase.hpp"
#include "File/File.hpp"

namespace DE {

template<class T> class List;
class GameObject;
class ConfigMap;

class Scene : public ObjectBase {

private:

	PRIVATE(GameObjects, GET, List<GameObject*>*);
	PRIVATE(NewGameObjects, GET, List<GameObject*>*);
	PRIVATE(CameraGameObject, GET_SET, GameObject*)

	PRIVATE(Size, GET, f32)
	PRIVATE(Path, GET, String)

	void destroyGameObjects();

	PRIVATE(LoadSceneConfigMap, NONE, ConfigMap*)
	PRIVATE(MaxGameObjectsToLoadPerFrame, NONE, u32)
	PRIVATE(GameObjectsToLoadTotal, NONE, u32)
	PRIVATE(GameObjectsToLoadIndex, NONE, u32)

public:

	GENERATE_METADATA(CONSTRUCTOR, Scene)

	void init();

	void loadScene(const String &path);
	void saveScene(const String &path);
	void unloadScene();
	bool isLoadFinished() const;

	void addGameObject(GameObject *gameObject);
	void updateComponents(GameObject *gameObject);
	void removeGameObject(GameObject *gameObject);

	void step();

	bool thereAreNewGameObjects() const;
	void flushNewGameObjects();
};
}

