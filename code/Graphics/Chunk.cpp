#include "Graphics/Chunk.hpp"

#include "Graphics/Batch.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/BatchesMap.hpp"
#include "Containers/HashMap.hpp"
#include "Maths/MathUtils.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Log/Log.hpp"

namespace DE {

Chunk::Chunk() : ObjectBase() {
	mLeftTop = Vector2(0, 0);
	mRadius = 0;
	mSize = 0;
	mIsLoaded = false;

	mRenderers = nullptr;
}

Chunk::~Chunk() {

	FOR_LIST(it, mRenderers) {
		//if(!it.get()->isDestroyed()){
			it.get()->finallyDestroy();
			Memory::free(it.get());
		//}
	}

	Memory::free(mRenderers);
}

void Chunk::init() {
	//TRACE();

	mRenderers = Memory::allocate<List<Renderer*>>();
	mRenderers->init();

	mLeftTop.set(0, 0, 0);
}

void Chunk::set(const Vector3 &leftTop, f32 size) {
	mLeftTop = leftTop;
	mSize = size;
	f32 halfSize = mSize / 2.0f;
	mCenter.set(mLeftTop.x + halfSize, mLeftTop.y - halfSize, 0);

	mRadius = mCenter.dst(mLeftTop);
}

void Chunk::update(BatchesMap *batchesMap) {

	RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y - mSize,0));
	RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y - mSize,0), Vector3(mLeftTop.x + mSize, mLeftTop.y - mSize,0));
	RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mSize, mLeftTop.y - mSize,0), Vector3(mLeftTop.x + mSize, mLeftTop.y,0));
	RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mSize, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y,0));

	FOR_LIST(it, mRenderers) {
		Renderer* renderer = it.get();

		if(renderer->isActive()){
			if (!renderer->getIsAlreadyInBatch()) {
				batchesMap->addRenderer(renderer);
			}

			if (!renderer->isStatic() && renderer->getIsAffectedByProjection() && ! containsRenderer(renderer)) {
				Chunk* newChunk = RenderEngine::getInstance()->assignChunk(renderer);

				// Only remove the renderer from this chunk if another chunk is found.
				// If not, keep the renderer here until a new chunk is found.
				if (newChunk && newChunk != this) {
					mRenderers->remove(it);
					newChunk->addRenderer(renderer);
				}
			}
		}

		if(renderer->getIsPendingToBeDestroyed()) {
			renderer->finallyDestroy();
			RenderEngine::getInstance()->freeRenderer(renderer);
		}

		if(renderer->getIsDestroyed()) {
			mRenderers->remove(it);
		}
	}
}

void Chunk::load() {
	if (!mIsLoaded) {
		mIsLoaded = true; /*ECHO("load")*/
	}
}
void Chunk::unload() {
	if (mIsLoaded) {
		mIsLoaded = false; /*ECHO("unload")*/
	}
}

void Chunk::addRenderer(Renderer *renderer) {
	mRenderers->pushBack(renderer);
}

bool Chunk::containsRenderer(const Renderer *renderer, f32 epsilon /*= 0.0f*/) const {
	Vector3 rendererPosition = renderer->getGameObject()->getTransform()->getWorldPosition();
	bool contains = MathUtils::testRectanglePoint(mLeftTop, mSize, mSize, rendererPosition, epsilon);
	return contains; // TODO : move to settings ?
}

bool Chunk::containsRendererSphere(const Renderer *renderer) const {
	Vector3 rendererPosition = renderer->getGameObject()->getTransform()->getWorldPosition();
	return MathUtils::testSphereSphere(mCenter, rendererPosition, mRadius,
			renderer->getGameObject()->getTransform()->getScale().y * 2.0f, 0);
}

}
