#ifndef DE_MESH_H
#define DE_MESH_H

#include "Array.hpp"
#include "DynamicArray.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "DE_Class.hpp"

namespace DE {

class Mesh: public DE_Class {

private:

	DE_M_GET(Vertices, Array<f32>*)
	DE_M_GET(Normals, Array<f32>*)
	DE_M_GET(TextureCoordinates, Array<f32>*)
	DE_M_GET(Faces, Array<u32>*)

	DE_M(VertexCount, u32)

	DE_M(VerticesIndex, u32)
	DE_M(NormalsIndex, u32)
	DE_M(FacesIndex, u32)
	DE_M(TextureCoordinatesIndex, u32)

	static Mesh* smRectangle;

public:

	DE_CLASS(Mesh, DE_Class)

	void init(u32 vertexCount, u32 facesCount);
	Mesh* addVertex(const Vector3 &vec);
	Mesh* addNormal(const Vector3 &vec);
	Mesh* addTexCoord(u32 u, u32 v);
	Mesh* addFace(u32 v1, u32 v2, u32 v3);

	static Mesh* getRectangle();
	static void freeRectangle();
};

} /* namespace DE */

#endif /* MESH_H */