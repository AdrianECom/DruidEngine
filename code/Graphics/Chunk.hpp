#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

class Renderer;
class BatchesMap;
template<class T> class List;

class Chunk : public ObjectBase {
private:

	List<Renderer*>* mRenderers;
	PRIVATE(LeftTop, NONE, Vector3)
	PRIVATE(Size, NONE, f32)
	PRIVATE(IsLoaded, NONE, bool)
	PRIVATE(Center, NONE, Vector3)
	PRIVATE(Radius, NONE, f32)

public:

	GENERATE_METADATA(CONSTRUCTOR, Chunk)

	GET(IsLoaded);
	GET(Center);
	GET(Radius);

	void init();
	void set(const Vector3 &leftTop, f32 size);
	void load();
	void update(BatchesMap *batchesMap);
	void unload();
	bool containsRenderer(const Renderer *renderer, f32 epsilon = 0.0f) const;
	bool containsRendererSphere(const Renderer *renderer) const;
	void addRenderer(Renderer *renderer);
};
}