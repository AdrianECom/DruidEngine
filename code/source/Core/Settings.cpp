#include "Settings.h"

namespace DE{

// ---------------------------------------------------------------------------

Settings::Settings() : DE_Class(), Singleton(){
  mSceneSize = 0;
  mQuadTreeMinSize = 0;
}

// ---------------------------------------------------------------------------

Settings::~Settings(){

}

// ---------------------------------------------------------------------------

void Settings::init(){
  mSceneSize = 2000;
  mQuadTreeMinSize = 500;
}

// ---------------------------------------------------------------------------

}
