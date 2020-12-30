#ifndef DE_ENGINE_H
#define DE_ENGINE_H

#include "DE_Class.hpp"
#include "Singleton.hpp"

namespace DE {

class RenderEngine;
class PhysicsEngine;
class ScriptEngine;

// ---------------------------------------------------------------------------

class Engine: public DE_Class, public Singleton<Engine> {

private:

	DE_M(FPS, f32)
	DE_M(RenderEngine, RenderEngine*)
	DE_M(PhysicsEngine, PhysicsEngine*)
	DE_M(ScriptEngine, ScriptEngine*)

public:

	DE_CLASS(Engine, DE_Class)

	void init();
	void initSubsystems();
	void terminateSubSystems();
	void run();
	void terminate();
};

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif /* DE_ENGINE_H */