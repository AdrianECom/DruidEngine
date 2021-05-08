#pragma once

#include "Core/BasicTypes.hpp"
#include "Core/Singleton.hpp"
#include "Core/ObjectBase.hpp"
#include "Log/Log.hpp"
#include <chrono>

namespace DE {

class TimeMark  : public ObjectBase {
private:
	PRIVATE(DeltaTimeMillis, NONE, f32)
	PRIVATE(DeltaTimeSeconds, NONE, f32)
	std::chrono::milliseconds mDeltaTimeChronoDuration;
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> mLastTime;

	PRIVATE(IsStarted, NONE, bool)

public:

	GENERATE_METADATA(CONSTRUCTOR, TimeMark)
	GET(IsStarted);

	void init();
	void start();
	void end();
	f32 getElapsedTimeMillis();
	f32 getDeltaTimeMillis();
	f32 getDeltaTimeSeconds();

	TimeMark& operator= (const TimeMark &other) {
		// self-assignment guard
		if (this == &other)
			return *this;

		// do the copy
		mDeltaTimeMillis = other.mDeltaTimeMillis;
		mDeltaTimeSeconds = other.mDeltaTimeSeconds;
		mDeltaTimeChronoDuration = other.mDeltaTimeChronoDuration;
		mStartTime = other.mStartTime;
		mLastTime = other.mLastTime;

		return *this;
	}
};

class Time : public ObjectBase, public Singleton<Time>{
private:
	PRIVATE(InternalTimeMark, NONE, TimeMark)

public:
	GENERATE_METADATA(CONSTRUCTOR, Time)

	void init() { mInternalTimeMark.init(); }
	void startFrame() { mInternalTimeMark.start(); }
	void endFrame() { mInternalTimeMark.end(); }
	f32 getElapsedTimeMillis() { return mInternalTimeMark.getElapsedTimeMillis(); }
	f32 getDeltaTimeMillis() { return mInternalTimeMark.getDeltaTimeMillis(); }
	f32 getDeltaTimeSeconds() { return mInternalTimeMark.getDeltaTimeSeconds(); }
};
}

