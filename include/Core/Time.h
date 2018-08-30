#ifndef DE_TIME_H
#define DE_TIME_H

#include "BasicTypes.h"
#include <chrono>

namespace DE {

using time_point = std::chrono::time_point<std::chrono::system_clock>;

class Time {
private:
  static f32 mDeltaTime;
  static time_point mNow, mLastTime;

public:
  Time();
  ~Time();

  static void init();
  static void tick();
  static f32 getNow();
  static f32 getDeltaTime();
  static f32 getElapsedTime();
};

} /* namespace DE */

#endif /* DE_TIME_H */
