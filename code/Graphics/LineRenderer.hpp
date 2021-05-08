#pragma once

#include "Maths/Vector3.hpp"

namespace DE {

class Shader;
template<class T> class Array;

class LineRenderer : public ObjectBase {
private:
PRIVATE(ShaderLine, NONE, Shader*)
		
		PRIVATE(MaxShapes, NONE, u32)
		PRIVATE(ShapesCounter, NONE, u32)

		PRIVATE(VAO, NONE, u32)
		PRIVATE(VBOPosition, NONE, u32)
		PRIVATE(EBO, NONE, u32)
		PRIVATE(PositionBufferIndex, NONE, u32)
		Array<f32>* mPositionBuffer;
		Array<u32>* mIndicesBuffer;

		PRIVATE(Size, NONE, f32)
public:
		GENERATE_METADATA(CONSTRUCTOR, LineRenderer)

	
		PUBLIC(IsAffectedByProjection, NONE, bool)
		
		void init();
		void add(const Vector3 &start, const Vector3 &end);
		void bind();
		void render();
	};
	
}
