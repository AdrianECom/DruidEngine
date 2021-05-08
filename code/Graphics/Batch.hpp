#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Matrix4.hpp"
#include "Containers/List.hpp"
#include "Containers/Array.hpp"
#include "Containers/HashMap.hpp"

namespace DE {

class Batch: public ObjectBase {

private:

	class RenderEngine* mRenderEngine;
	HashMap<u32, List<class Renderer*>*>* mRenderers;
	class Material* mMaterial;
	const class Mesh* mMesh;

	PRIVATE(VBOPosition, NONE, u32) // TODO: change u32 for GLuint
	PRIVATE(EBO, NONE, u32)
	PRIVATE(VBOTexture, NONE, u32)
	PRIVATE(VBOColor, NONE, u32)
	PRIVATE(VBONormal, NONE, u32)
	PRIVATE(VAO, NONE, u32)

	PRIVATE(MaxVertexBufferSize, NONE, u32)
	PRIVATE(MaxMeshes, NONE, u32)
	PRIVATE(MeshesIndex, NONE, u32)
	PRIVATE(VerticesPerMesh, NONE, u32)
	PRIVATE(VertexPositionSize, NONE, u32)
	PRIVATE(VertexTextureSize, NONE, u32)
	PRIVATE(VertexColorSize, NONE, u32)
	PRIVATE(FacesSize, NONE, u32)
	PRIVATE(PositionBufferIndex, NONE, u32)
	PRIVATE(TextureBufferIndex, NONE, u32)
	PRIVATE(ColorBufferIndex, NONE, u32)
	Array<f32>* mPositionBuffer;
	Array<f32>* mTextureBuffer;
	Array<f32>* mColorBuffer;
	Array<u32>* mIndicesBuffer;

	PRIVATE(Binded, NONE, bool)
	PRIVATE(TextureId, NONE, u32)

	void addToVertexBuffer(class Renderer* renderer);
	void clearVertexBuffer();

	bool checkInFrustum(class Camera *cam, class Renderer *renderer);
	bool checkDistance(class Camera *cam, class Renderer *renderer);
	bool checkIsOutOfCamera(class Camera *cam, class Renderer *renderer);

	void internalRemoveRendererFromList(const class Iterator *it, List<class Renderer*> *list);

	static u8 rendererYCoordinateComparator(class Renderer *a, class Renderer *b);

	void sort(u32 layer);
	void insertSorted(class Renderer *renderer, List<class Renderer*> *renderers);

	static bool smIsScreenOrthoReady;
	static Matrix4 smScreenOrtho;

public:

	GENERATE_METADATA(CONSTRUCTOR, Batch)

	SET(Material);
	SET(Mesh);

	void init(const class Mesh *mesh, class Material *material);
	void bind();
	void update();

	// returns the count of draw calls
	u32 render(u32 layer);

	void addRenderer(class Renderer *renderer);
};
}

