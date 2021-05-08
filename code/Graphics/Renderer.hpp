#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Matrix4.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Containers/HashMap.hpp"
#include "Containers/Array.hpp"
#include "Graphics/Chunk.hpp"

namespace DE {

class Renderer : public Component {
private:

	PRIVATE(Animations, NONE, HashMap<String, Animation*>*)
	PRIVATE(RenderereModelMatrix, NONE, Matrix4)
	PRIVATE(PositionOffsetDirty, NONE, bool)
	PRIVATE(ForceRecalculateVertices, NONE, bool)
	PRIVATE(Vertices, NONE, Array<Vector2>*)
	PRIVATE(IsAffectedByProjection, NONE, bool)

	PRIVATE(CurrentAnimation, GET, Animation*)
	PRIVATE(Color, GET, Array<f32>*)
	PRIVATE(PositionOffset, GET, Vector3)
	PRIVATE(RegionPosition, GET, Vector2)
	PRIVATE(RegionSize, GET, Vector2)
	PRIVATE(Mesh, GET_SET, Mesh*)
	PRIVATE(Material, GET_SET, Material*)
	PRIVATE(IsInvertAxis, GET_SET, bool)
	PRIVATE(IsLineMode, GET_SET, bool)
	PRIVATE(Layer, GET_SET, u32)
	PRIVATE(RenderDistance, GET_SET, f32)
	PRIVATE(IsOutOfCamera, GET_SET, bool)
	PRIVATE(IsAlreadyInBatch, GET_SET, bool)
	PRIVATE(Chunk, GET_SET, Chunk*)

public:

	GENERATE_METADATA(CONSTRUCTOR, Renderer)

	void init() override;
	void setAnimation(StringRef name);
	void addAnimation(StringRef name, Animation* animation);
	bool hasAnimations() const;
	void updateAnimation();
	void setColor(const Vector4& color);
	void setRegion(f32 u, f32 v, f32 width, f32 height);
	void setPositionOffset(Vector3 newPositionOffset);
	bool getIsAffectedByProjection();
	const Array<Vector2>* getVertices(bool force = false);
	void forceRecalculateVertices();

	virtual void save(ConfigMap* configMap, StringRef objectName) override;
	virtual void load(ConfigMap* configMap, StringRef objectName) override;
};
}

