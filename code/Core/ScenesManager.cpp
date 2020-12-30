#include "ScenesManager.hpp"
#include "HashMap.hpp"
#include "List.hpp"
#include "Scene.hpp"
#include "Memory.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Settings.hpp"
#include "RenderEngine.hpp"
#include "Script.hpp"
#include <string>

namespace DE {

// ---------------------------------------------------------------------------

ScenesManager::ScenesManager() : DE_Class(), Singleton() {
	mCurrentScene = nullptr;
	mSceneHasChanged = true;
}

// ---------------------------------------------------------------------------

ScenesManager::~ScenesManager() {

	FOR_LIST (it, mScenes) {
		Memory::free<Scene>(it.get());
	}

	Memory::free<List<Scene*>>(mScenes);

	//mGameObjectController->getComponents<Script>()->get(0)->terminate();
	//Memory::free<Script>(mGameObjectController->getComponents<Script>()->get(0));
	mGameObjectController->destroy();
	Memory::free<GameObject>(mGameObjectController);
}

// ---------------------------------------------------------------------------

void ScenesManager::internalLoadScene() {
	if (Settings::getInstance()->getU32("scenes.length") > 0) {
		std::string sceneName = Settings::getInstance()->getString(
				"scenes[" + std::to_string(mCurrentSceneIndex) + "]");
		mCurrentScene->loadScene(sceneName);
	}

	mGameObjectController->setScene(mCurrentScene);
	RenderEngine::getInstance()->setCamera(mCurrentScene->getCameraGameObject()->getComponents<Camera>()->get(0));
}

// ---------------------------------------------------------------------------

void ScenesManager::init() {
	mCurrentSceneIndex = 0;

	mScenes = Memory::allocate<List<Scene*>>();
	mScenes->init();

	u32 scenesCount = 1;

	if (Settings::getInstance()->getU32("scenes.length") > 0) {
		scenesCount = Settings::getInstance()->getU32("scenes.length");
	}

	FOR_RANGE(i, 0, scenesCount) {
		Scene* scene = Memory::allocate<Scene>();
		scene->init();
		addScene(scene);
	}

	mCurrentScene = mScenes->get(0);
}

// ---------------------------------------------------------------------------

void ScenesManager::step() {

	/*if (mSceneHasChanged) {

	} else {

	}*/

	mCurrentScene->step();
}

void ScenesManager::loadCurrentScene() {
	if (mSceneHasChanged) {
		mCurrentScene = mScenes->get(mCurrentSceneIndex);
		internalLoadScene();
		mSceneHasChanged = false;
	}
}

// ---------------------------------------------------------------------------

void ScenesManager::addScene(Scene *newScene) {
	mScenes->pushBack(newScene);
}

// ---------------------------------------------------------------------------

void ScenesManager::setScene(u32 i) {
	if (mCurrentSceneIndex != i) {
		mCurrentSceneIndex = i;
		mSceneHasChanged = true;
	}
}

// ---------------------------------------------------------------------------

}