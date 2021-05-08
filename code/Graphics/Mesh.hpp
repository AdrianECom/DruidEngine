#pragma once

#include "Containers/Array.hpp"
#include "Containers/DynamicArray.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Core/ObjectBase.hpp"

namespace DE {

class Mesh : public ObjectBase {

private:

	Array<f32>* mVertices;
	Array<f32>* mNormals;
	Array<f32>* mTextureCoordinates;
	Array<u32>* mFaces;

	PRIVATE(VertexCount, NONE, u32)

	PRIVATE(VerticesIndex, NONE, u32)
	PRIVATE(NormalsIndex, NONE, u32)
	PRIVATE(FacesIndex, NONE, u32)
	PRIVATE(TextureCoordinatesIndex, NONE, u32)

	static Mesh* smRectangle;

public:

	GENERATE_METADATA(CONSTRUCTOR, Mesh)

	GET(Vertices);
	GET(Normals);
	GET(TextureCoordinates);
	GET(Faces);

	void init(u32 vertexCount, u32 facesCount);
	Mesh* addVertex(const Vector3 &vec);
	Mesh* addNormal(const Vector3 &vec);
	Mesh* addTexCoord(u32 u, u32 v);
	Mesh* addFace(u32 v1, u32 v2, u32 v3);

	static Mesh* getRectangle();
	static void freeRectangle();
};
}

