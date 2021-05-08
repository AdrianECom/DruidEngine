#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Containers/List.hpp"

namespace DE {

class Script;
class Iterator;

class ScriptEngine: public ObjectBase, public Singleton<ScriptEngine> {
private:

	List<Script*>* mScripts;
PRIVATE(Controller, NONE, Script*)
	void internalRemoveScript(const Iterator *it);

public:GENERATE_METADATA(CONSTRUCTOR, ScriptEngine)

	void init();
	void addScript(Script *newScript);
	void step();
	void terminate();
};
}

