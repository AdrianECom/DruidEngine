#pragma once

#include "Core/Basic.hpp"
#include "Core/Singleton.hpp"
#include "ConfigMap.hpp"

namespace DE {

class EngineConfig: public ObjectBase, public Singleton<EngineConfig> {

private:
	PRIVATE(ConfigMap, NONE, ConfigMap)

public:

	GENERATE_METADATA(CONSTRUCTOR, EngineConfig)

	void init();
	void readConfigFile(const String &path);
	String getString(const String &key);
	u32 getU32(const String &key);
	f32 getF32(const String &key);
	bool getBool(const String &key);
};
}

