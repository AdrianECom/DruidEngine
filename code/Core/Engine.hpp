#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"

namespace DE {

class RenderEngine;
class PhysicsEngine;
class ScriptEngine;

class Engine: public ObjectBase, public Singleton<Engine> {

private:

	PRIVATE(FPS, NONE, f32)
PRIVATE(RenderEngine, NONE, RenderEngine*)
PRIVATE(PhysicsEngine, NONE, PhysicsEngine*)
PRIVATE(ScriptEngine, NONE, ScriptEngine*)

public:

	GENERATE_METADATA(CONSTRUCTOR, Engine)

	void init();
	void initSubsystems();
	void terminateSubSystems();
	void run();
	void terminate();
};
}

