#include "File.h"

namespace DE {

// ---------------------------------------------------------------------------

File::File() : DE_Class() {

}

File::~File() {

}

// ---------------------------------------------------------------------------

void File::init() {

}

// ---------------------------------------------------------------------------

void File::readFile(const std::string& path) {

  // Test method.

  std::ifstream file(path);
  std::string str;

  while(std::getline(file,str)){
    std::cout << str << std::endl;
  }
}

// ---------------------------------------------------------------------------

} /* namespace DE */