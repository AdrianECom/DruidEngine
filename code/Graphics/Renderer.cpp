#include "Graphics/Renderer.hpp"

#include "Graphics/Animation/AnimationFrame.hpp"
#include "Graphics/Shader.hpp"
#include "Containers/List.hpp"
#include "Log/Log.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Physics/Collider.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Config/ConfigMap.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/MaterialManager.hpp"
namespace DE {

Renderer::Renderer() : Component() {
	mMesh = nullptr;
	mMaterial = nullptr;
	mAnimations = nullptr;
	mCurrentAnimation = nullptr;
	mColor = nullptr;

	mVertices = nullptr;

	mPositionOffset = Vector3(0.0, 0.0, 0.0);
	mPositionOffsetDirty = true;

	// texture region
	mRegionPosition = Vector2(0.0, 0.0);
	mRegionSize = Vector2(1.0, 1.0);

	mIsInvertAxis = false;

	mIsLineMode = false;

	mLayer = 0;

	mRenderDistance = 1500; // TODO : move to settings?
	mIsOutOfCamera = false;

	mIsAffectedByProjection = true;

	mChunk = nullptr;
	mIsAlreadyInBatch = false;

	mForceRecalculateVertices = false;
}

Renderer::~Renderer() {
	if (mAnimations) {
		FOR_LIST(it, mAnimations->getValues()) {
			Memory::free(it.get());
		}

		Memory::free(mAnimations);
	}

	Memory::free(mColor);

	if (mVertices) {
		Memory::free(mVertices);
	}

}

void Renderer::init() {
	// TRACE();

	mColor = Memory::allocate<Array<f32>>();
	mColor->init(4);

	setColor(Vector4(0, 0, 0, 1));

	mVertices = Memory::allocate<Array<Vector2>>();
	mVertices->init(4);

	mVertices->set(0, Vector2(0, 0)); // LEFT TOP VERTEX
	mVertices->set(1, Vector2(0, 0)); // LEFT BOTTOM
	mVertices->set(2, Vector2(0, 0)); // RIGHT BOTTOM
	mVertices->set(3, Vector2(0, 0)); // RIGHT TOP

	mForceRecalculateVertices = false;

}

bool Renderer::hasAnimations() const { return mAnimations && mAnimations->getLength() > 0; } ;

void Renderer::setRegion(f32 u, f32 v, f32 width, f32 height) {
	mRegionPosition.x = u;
	mRegionPosition.y = v;

	mRegionSize.x = width;
	mRegionSize.y = height;
}

/**
 * Set the animation, by name.
 * \param string name The name.
 */
void Renderer::setAnimation(const String &name) {
	if (mAnimations) {
		if (mAnimations->contains(name)) {
			mCurrentAnimation = mAnimations->get(name);
		}
	}
};

//----------------------------------------------------------------------

/**
 * Add an animation, by name.
 * \param string name The name.
 * \param Animation animation The animation.
 */
void Renderer::addAnimation(const String &name, Animation *animation) {
	if (!mAnimations) {
		mAnimations = Memory::allocate<HashMap<String, Animation*>>();
		mAnimations->init();
	}

	mAnimations->set(name, animation);
};

void Renderer::updateAnimation() {
	if (mMaterial) {
		if (hasAnimations()) {
			const AnimationFrame* frame = mCurrentAnimation->getNextFrame();
			mRegionPosition = frame->getPosition();
			mRegionSize = Vector2(frame->getWidth(), frame->getHeight());
		}
	}
};

void Renderer::setPositionOffset(Vector3 newPositionOffset) {
	mPositionOffset = newPositionOffset;
	mPositionOffsetDirty = true;
};

void Renderer::setColor(const Vector4 &color) {
	mColor->set(0, color.x);
	mColor->set(1, color.y);
	mColor->set(2, color.z);
	mColor->set(3, color.w);
};

bool Renderer::getIsAffectedByProjection() {

	if(getGameObject()){
		mIsAffectedByProjection = getGameObject()->getTransform()->getAffectedByProjection();
	}

	return mIsAffectedByProjection;
}

const Array<Vector2>* Renderer::getVertices(bool force /*= false*/) {

	if(mPositionOffsetDirty || !isStatic() || force || mForceRecalculateVertices ){

		mRenderereModelMatrix.translation(mPositionOffset);

		mRenderereModelMatrix.mul(getGameObject()->getTransform()->getModelMatrix());
		mPositionOffsetDirty = false;

		FOR_ARRAY(i, mVertices) {
			Vector3 vertexPosition(
			mMesh->getVertices()->get(i*3 + 0),
			mMesh->getVertices()->get(i*3 + 1),
			mMesh->getVertices()->get(i*3 + 2));

			vertexPosition = mRenderereModelMatrix.mulVector(Vector4(vertexPosition,1));

			mVertices->set(i, vertexPosition);
		}

		mForceRecalculateVertices = false;
	}

	return mVertices;
}

void Renderer::forceRecalculateVertices() {
	getGameObject()->getTransform()->forceModelMatrixCalculation();
	mForceRecalculateVertices = true;
}

void Renderer::save(ConfigMap* configMap, StringRef objectName) {
	Texture *texture = getMaterial()->getTexture();
	configMap->setString(objectName + ".texture.path", texture->getPath());
	configMap->setF32(objectName + ".texture.region.u", getRegionPosition().x);
	configMap->setF32(objectName + ".texture.region.v", getRegionPosition().y);
	configMap->setF32(objectName + ".texture.region.width", getRegionSize().x);
	configMap->setF32(objectName + ".texture.region.height", getRegionSize().y);
	configMap->setU32(objectName + ".layer", getLayer());
}

void Renderer::load(ConfigMap* configMap, StringRef objectName) {
	Material* material = MaterialManager::getInstance()->loadMaterial(configMap->getString(objectName + ".texture.path"));
	Vector2 textureRegionPosition(configMap->getF32(objectName + ".texture.region.u"),
			configMap->getF32(objectName + ".texture.region.v"));
	Vector2 textureRegionSize(configMap->getF32(objectName + ".texture.region.width"),
			configMap->getF32(objectName + ".texture.region.height"));

	setLayer(configMap->getU32(objectName + ".layer"));
	setMesh(Mesh::getRectangle());
	setMaterial(material);
	setRegion(textureRegionPosition.x, textureRegionPosition.y, textureRegionSize.x, textureRegionSize.y);

}

}
