#pragma once

#include "Core/ObjectBase.hpp"

namespace DE {

class Texture;
class Renderer;
class Batch;
template<class K, class V> class HashMap;

class BatchesMap: public ObjectBase {

private:

	HashMap<Texture*, Batch*>* mBatches;

public:
	
	GENERATE_METADATA(CONSTRUCTOR, BatchesMap)
	void init();
	u32 render(u32 layer);
	void addRenderer(Renderer *renderer);
};
}

