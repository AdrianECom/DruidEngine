#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/TimeUtils.hpp"
#include "Core/Assert/Assert.hpp"
#include "Core/Singleton.hpp"

namespace DE {

template<class K, class V> class HashMap;
template<class T> class List;

#define PROFILER_TIMEMARK_START() Profiler::getInstance()->timeMarkStart(__PRETTY_FUNCTION__);
#define PROFILER_TIMEMARK_END() Profiler::getInstance()->timeMarkEnd(__PRETTY_FUNCTION__);

class Profiler : public ObjectBase, public Singleton<Profiler>{
private:

	HashMap<String, f32>* mTimeMap;
	HashMap<String, TimeMark*>* mTimeMarkMap;
	PRIVATE(TotalTime, NONE, f32)

	void printResult(String& name, f32 time);

public:
	GENERATE_METADATA(CONSTRUCTOR, Profiler)

	void init();
	void step(f32 deltaTime);
	void terminate();

	void timeMarkStart(StringRef name);
	void timeMarkEnd(StringRef name);
};
}

