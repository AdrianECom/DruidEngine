#pragma once

#include "Core/ObjectBase.hpp"

#include "Maths/Vector2.hpp"

namespace DE {

class AnimationFrame : public ObjectBase {
private:

	PRIVATE(Position, NONE, Vector2)
	PRIVATE(Width, NONE, f32)
	PRIVATE(Height, NONE, f32)

public:

	GENERATE_METADATA(CONSTRUCTOR, AnimationFrame)

	GET(Position);
	GET(Width);
	GET(Height);

	void init(const Vector2 &position, f32 width, f32 height);
};
}

