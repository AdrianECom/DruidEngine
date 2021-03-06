#include "Scripting/ScriptEngine.hpp"

#include "Memory/Memory.hpp"
#include "Scripting/Script.hpp"
#include "Scene/ScenesManager.hpp"
#include "Scene/GameObject.hpp"

#include "Log/Log.hpp"

namespace DE {

ScriptEngine::ScriptEngine() : ObjectBase(), Singleton() {
	mScripts = nullptr;
	mController = nullptr;
}

ScriptEngine::~ScriptEngine() = default;

void ScriptEngine::init() {
	TRACE()

	mScripts = Memory::allocate<List<Script*>>();
	mScripts->init();

	mController = ScenesManager::getInstance()->getGameObjectController()->getFirstComponent<Script>();
}

void ScriptEngine::addScript(Script *newScript) {
	mScripts->pushBack(newScript);
}

void ScriptEngine::step() {

	if (mController) {
		if (!mController->getFirstStepDone()) {
			mController->firstStep();
			mController->firstStepDone();
		}

		mController->step();
	}

	FOR_LIST (it, mScripts) {
		Script* script = it.get();

		if (script->isActive()) {
			if (!script->getFirstStepDone()) {
				script->firstStep();
				script->firstStepDone();
			}

			script->step();
		} else if (it.get()->getIsPendingToBeDestroyed()) {
			internalRemoveScript(&it);
		}

	}
}

void ScriptEngine::internalRemoveScript(const Iterator *it) {
	auto castedIt = it->cast<Script*>();
	mScripts->remove(*castedIt);

	Script* script = (*castedIt).get();
	script->terminate();
	script->finallyDestroy();
	Memory::free(script);
}

void ScriptEngine::terminate() {
	TRACE()

	if(mController){
		mController->terminate();
		Memory::free(mController);
	}

	if (mScripts) {
		FOR_LIST (it, mScripts) {
			Script* script = it.get();
			script->terminate();
			Memory::free(script);
		}

		Memory::free(mScripts);
	}
}

}
