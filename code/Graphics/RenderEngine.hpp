#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"

namespace DE {

class Batch;
class Texture;
class RenderContext;
class Camera;
class Renderer;
class Shader;
class BatchesMap;
class Chunk;
template<class T> class List;
template<class T> class Array;
template<class K, class V> class HashMap;
class Vector3;
class LineRenderer;

class RenderEngine: public ObjectBase, public Singleton<RenderEngine> {

private:

	class LayerData : public ObjectBase {
	public:
		GENERATE_METADATA(CONSTRUCTOR, LayerData)

		PUBLIC(Sorted, NONE, bool)
		PUBLIC(DynamicObjectsCount, NONE, u32) // Non static objects count
		PUBLIC(SortCounter, NONE, u32)
		PUBLIC(Visible, NONE, bool)
	};

	PRIVATE(BatchesMap, NONE,BatchesMap*)
	PRIVATE(BatchesMapScreenSpace, NONE,BatchesMap*)

	PRIVATE(LineRenderer, NONE,LineRenderer*)
	PRIVATE(LineRendererScreenSpace, NONE,LineRenderer*)

	PRIVATE(Camera, GET_SET,Camera*)
	PRIVATE(CameraDirtyTranslation, GET, bool)
	
	//HashMap<u32, LayerData*>* mLayersData;
	//using hp = HashMap<u32, LayerData*>;
	PRIVATE(LayersData, GET_SET, HashMap<u32, LayerData*>*);
	PRIVATE(MaxLayers, GET, u32)
	PRIVATE(MaxLayersUsed, NONE, u32)

	PRIVATE(MinChunkDrawDistance, GET, f32)
	PRIVATE(Chunks, NONE, Array<Chunk*>*)

	PRIVATE(RenderersToFree, NONE, List<Renderer*>*)

	void checkChunks();
	void freeRenderersPendingtoFree();
	void renderBatches();
	void swap();

public:

	GENERATE_METADATA(CONSTRUCTOR, RenderEngine)

	void init(f32 sceneSize);
	void step(); // render
	void stepDebug(); // debug render
	void terminate();

	void addRenderer(Renderer *renderer);
	Chunk* assignChunk(Renderer *renderer);
	void freeRenderer(Renderer *renderer);
	void drawLine(const Vector3 &start, const Vector3 &end, f32 size = 1, bool isAffectedByProjection = true, Vector4 color = Vector4(1,1,1,1));
	bool frustumTestSphere(const Vector3 &center, f32 radius);
};
}