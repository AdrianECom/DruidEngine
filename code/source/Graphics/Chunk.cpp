#include "Chunk.h"

#include "Batch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Material.h"
#include "Mesh.h"
#include "HashMap.h"
#include "MathUtils.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderEngine.h"
#include "Log.h"

namespace DE {

// ---------------------------------------------------------------------------

BatchesMap::BatchesMap() : DE_Class(){
  mBatches = nullptr;
}

BatchesMap::~BatchesMap(){
  FOR_LIST(it, mBatches->getValues()){
    Memory::free<Batch>(it.get());
  }

  Memory::free<HashMap<Texture*, Batch*>>(mBatches);
}

// ---------------------------------------------------------------------------

void BatchesMap::init(){
  TRACE();
  mBatches = Memory::allocate<HashMap<Texture*, Batch*>>();
  mBatches->init();
}

// ---------------------------------------------------------------------------

void BatchesMap::addRenderer(Renderer* renderer){

  Texture* texture = renderer->getMaterial()->getTexture();

  if( ! mBatches->contains(texture)){

    Batch* batch = Memory::allocate<Batch>();
    batch->init(renderer->getMesh(), renderer->getMaterial());
    // batch->setChunk(chunk);

    mBatches->set(texture, batch);
  }

  mBatches->get(texture)->addRenderer(renderer);
}

// ---------------------------------------------------------------------------

u32 BatchesMap::render(u32 layer){
  u32 drawCallCounter = 0;

  FOR_LIST(it, mBatches->getValues()){
		drawCallCounter += it.get()->render(layer);
	}

  return drawCallCounter;
}

// ---------------------------------------------------------------------------

Chunk::Chunk() : DE_Class(){
  mLeftTop = Vector2(0,0);
  mRadius = 0;
  mSize = 0;
  mIsLoaded = false;

  mRenderers = nullptr;
  mLastRenderersSize = 0;
  mNewRenderersSize = 0;
}

Chunk::~Chunk(){
  Memory::free<List<Renderer*>>(mRenderers);
}

// ---------------------------------------------------------------------------

void Chunk::init(){
  TRACE();

  mRenderers = Memory::allocate<List<Renderer*>>();
  mRenderers->init();

  mLeftTop.set(0,0,0);
}

// ---------------------------------------------------------------------------

void Chunk::set(const Vector3& leftTop, f32 size){
  mLeftTop = leftTop;
  mSize = size;
  f32 halfSize = mSize / 2.0f;
  mCenter.set(mLeftTop.x + halfSize, mLeftTop.y - halfSize, 0);

  mRadius = mCenter.dst(mLeftTop);
}

// ---------------------------------------------------------------------------

void Chunk::update(BatchesMap* batchesMap){

  // RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y - mSize,0));
  // RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y - mSize,0), Vector3(mLeftTop.x + mSize, mLeftTop.y - mSize,0));
  // RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mSize, mLeftTop.y - mSize,0), Vector3(mLeftTop.x + mSize, mLeftTop.y,0));
  // RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mSize, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y,0));

  FOR_LIST(it, mRenderers){
    Renderer* renderer = it.get();

    if(!renderer->isAlreadyInBatch()){
      batchesMap->addRenderer(it.get());
    }

    if( ! it.get()->isStatic() && ! this->containsRenderer(renderer)){
      RenderEngine::getInstance()->assignChunk(renderer)->addRenderer(renderer);
      mRenderers->remove(it);
    }
  }
}


// ---------------------------------------------------------------------------

void Chunk::load(){ if(!mIsLoaded){ mIsLoaded = true; /*ECHO("load")*/} }
void Chunk::unload(){ if(mIsLoaded){ mIsLoaded = false; /*ECHO("unload")*/ } }
bool Chunk::isLoaded(){ return mIsLoaded; }

// ---------------------------------------------------------------------------

void Chunk::addRenderer(Renderer* renderer){
  mRenderers->pushBack(renderer);

  mNewRenderersSize = mRenderers->getLength();
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

u32 Chunk::render(u32 layer){
  RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y - mSize,0));
  RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y - mSize,0), Vector3(mLeftTop.x + mSize, mLeftTop.y - mSize,0));
  RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mSize, mLeftTop.y - mSize,0), Vector3(mLeftTop.x + mSize, mLeftTop.y,0));
  RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mSize, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y,0));

  return 0 /* mBatchesMap->render(layer)*/;
}

// ---------------------------------------------------------------------------

bool Chunk::containsRenderer(const Renderer* renderer, f32 epsilon /*= 0.0f*/){
  Vector3 rendererPosition = renderer->getGameObject()->getTransform()->getWorldPosition();
  bool contains = MathUtils::testRectanglePoint(mLeftTop, mSize, mSize, rendererPosition, epsilon);
  return contains; // TODO : move to settings ?
}

// ---------------------------------------------------------------------------

bool Chunk::containsRendererSphere(const Renderer* renderer){
  Vector3 rendererPosition = renderer->getGameObject()->getTransform()->getWorldPosition();
  return MathUtils::testSphereSphere(mCenter, rendererPosition, mRadius, renderer->getGameObject()->getTransform()->getScale().y*2.0f);
}

// ---------------------------------------------------------------------------

} /* namespace DE */