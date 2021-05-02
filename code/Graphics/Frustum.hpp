#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Memory/Memory.hpp"

namespace DE {

class Matrix4;
class Camera;
template<class T> class Array;

class Frustum : public ObjectBase {

private:

	 Matrix4* mVPmatrix;
	 Camera* mCamera;
	 Array<Vector4>* mPlanes;

public:

	GENERATE_METADATA(Frustum);

	Frustum();
	virtual ~Frustum() override;

	void init(Camera *camera);

	/**
	 * Builds the frustum planes.
	 */
	void build(bool forceBuild = false);

	bool testSphere(const Vector3 &center, f32 radius) const;
	bool testPoint(const Vector3 &point) const;
	bool testRectangle(const Vector3 &leftTop, f32 width, f32 height) const;
};
}

