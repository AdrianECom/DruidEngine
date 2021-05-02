#pragma once

#include "Core/Basic.hpp"
#include "Core/Singleton.hpp"
#include "ConfigMap.hpp"

namespace DE {

class EngineConfig: public ObjectBase, public Singleton<EngineConfig> {

private:
	 ConfigMap mConfigMap;

public:

	GENERATE_METADATA(EngineConfig);

	EngineConfig();
	virtual ~EngineConfig() override;

	void init();
	void readConfigFile(const String &path);
	String getString(const String &key);
	u32 getU32(const String &key);
	f32 getF32(const String &key);
	bool getBool(const String &key);
};
}

