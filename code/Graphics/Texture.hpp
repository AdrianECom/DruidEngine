#pragma once

#include "Core/ObjectBase.hpp"
#include <string>

namespace DE {

class Texture : public ObjectBase {

	PRIVATE(TextureId, NONE, u32)
PRIVATE(Data, NONE, byte*)
	PRIVATE(Width, NONE, int)
	PRIVATE(Height, NONE, int)
	PRIVATE(Path, NONE, String)

public:

	GENERATE_METADATA(CONSTRUCTOR, Texture)

	GET(TextureId);
	GET(Data);
	GET(Width);
	GET(Height);
	GET(Path);

	void init(const String &path);
};
}

