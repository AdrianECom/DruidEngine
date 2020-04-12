#include "ScenesManager.h"
#include "HashMap.h"
#include "List.h"
#include "Scene.h"
#include "Memory.h"
#include "GameObject.h"
#include "Camera.h"
#include "Settings.h"
#include "RenderEngine.h"
#include <string>

namespace DE{

// ---------------------------------------------------------------------------

ScenesManager::ScenesManager() : DE_Class(), Singleton(){
  mCurrentScene = nullptr;
  mSceneHasChanged = true;
}

// ---------------------------------------------------------------------------

ScenesManager::~ScenesManager(){

	FOR_LIST (it, mScenes){
		Memory::free<Scene>(it.get());
	}

	Memory::free<List<Scene*>>(mScenes);
}

// ---------------------------------------------------------------------------

void ScenesManager::internalLoadScene(){
  if(Settings::getInstance()->getU32("scenes.length") > 0){
		std::string sceneName = Settings::getInstance()->getString("scenes["+std::to_string(mCurrentSceneIndex)+"]");

    RenderEngine::getInstance()->setCamera(mCurrentScene->getCameraGameObject()->getComponents<Camera>()->get(0));

		mCurrentScene->loadScene(sceneName);
    mGameObjectController->setScene(mCurrentScene);
	}
}

// ---------------------------------------------------------------------------

void ScenesManager::init(){
  mCurrentSceneIndex = 0;

  mScenes = Memory::allocate<List<Scene*>>();
  mScenes->init();

  if(Settings::getInstance()->getU32("scenes.length") > 0){
    FOR_RANGE(i, 0, Settings::getInstance()->getU32("scenes.length")){
      Scene* scene = Memory::allocate<Scene>();
      scene->init();
      addScene(scene);
    }

    mCurrentScene = mScenes->get(0);
  }
}

// ---------------------------------------------------------------------------

void ScenesManager::step(){

  if(mSceneHasChanged){
    mCurrentScene = mScenes->get(mCurrentSceneIndex);
    internalLoadScene();
    mSceneHasChanged = false;
  } else {
    mCurrentScene->step();
  }
}

// ---------------------------------------------------------------------------

void ScenesManager::addScene(Scene* newScene){
	mScenes->pushBack(newScene);
}

// ---------------------------------------------------------------------------

void ScenesManager::setScene(u32 i){
  if(mCurrentSceneIndex != i){
    mCurrentSceneIndex = i;
    mSceneHasChanged = true;
  }
}

// ---------------------------------------------------------------------------

Scene* ScenesManager::getCurrentScene(){
  return mCurrentScene;
}

// ---------------------------------------------------------------------------

void ScenesManager::setGameObjectController(GameObject* controller){
  mGameObjectController = controller;
};

// ---------------------------------------------------------------------------

}
