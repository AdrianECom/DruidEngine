#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector4.hpp"

namespace DE {

class Texture;
class Shader;

class Material : public ObjectBase {

private:
	PRIVATE(Color, GET_SET, Vector4)
	PRIVATE(Shader, GET_SET, Shader*);
	PRIVATE(Texture, GET_SET, Texture*);
	PRIVATE(AlphaEnabled, GET_SET, bool)
	PRIVATE(HasBorder, GET_SET, bool)

public:

	GENERATE_METADATA(CONSTRUCTOR, Material)

	void init();
};
}

