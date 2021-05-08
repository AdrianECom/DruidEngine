#pragma once

#include "Core/ObjectBase.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <regex>

namespace DE {

class File : public ObjectBase {

public:

	GENERATE_METADATA(CONSTRUCTOR, File)

	static void init();
	static void readFile(const String &path, std::function<void(std::ifstream &file)> callback);
	static void writeFile(const String &path, std::function<void(std::ofstream &file)> callback);
};
}

