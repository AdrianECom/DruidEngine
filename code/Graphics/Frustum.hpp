#ifndef DE_FRUSTUM_H
#define DE_FRUSTUM_H

#include "DE_Class.hpp"
#include "Vector4.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Memory.hpp"

namespace DE {

class Matrix4;
class Camera;
template<class T> class Array;

class Frustum: public DE_Class {

private:

	DE_M(VPmatrix, Matrix4*)
	DE_M(Camera, Camera*)
	DE_M(Planes, Array<Vector4>*)

public:

	DE_CLASS(Frustum, DE_Class)

	void init(Camera *camera);

	/**
	 * Builds the frustum planes.
	 */
	void build(bool forceBuild = false);

	bool testSphere(const Vector3 &center, f32 radius) const;
	bool testPoint(const Vector3 &point) const;
	bool testRectangle(const Vector3 &leftTop, f32 width, f32 height) const;
};

} /* namespace DE */

#endif /* DE_FRUSTUM_H */