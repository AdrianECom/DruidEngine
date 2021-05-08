#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Matrix4.hpp"
#include "Memory/Memory.hpp"

namespace DE {

class Matrix4;
class Frustum;

class Camera : public Component {

private:

	PRIVATE(ProjectionMatrix, NONE, Matrix4)
	PRIVATE(ViewTranslationMatrix, NONE, Matrix4)
	PRIVATE(InversePVMatrix, NONE, Matrix4) // used in screen to world calculations.

	PRIVATE(Left, NONE, f32)
	PRIVATE(Right, NONE, f32)
	PRIVATE(Bottom, NONE, f32)
	PRIVATE(Top, NONE, f32)
	PRIVATE(Near, NONE, f32)
	PRIVATE(Far, NONE, f32)
	
	PRIVATE(Fov, NONE, f32)
	PRIVATE(IsOrtho, NONE, bool)

PRIVATE(Frustum, NONE, Frustum*)
	PRIVATE(Zoom, NONE, f32)

public:

	GENERATE_METADATA(CONSTRUCTOR, Camera)

	GET(Frustum);
	GET(Zoom);

	void init() override;

	const Matrix4& getProjectionMatrix() const;
	const Matrix4& getViewTranslationMatrix();
	const Matrix4& getViewRotationMatrix();

	void setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void setPerspective(f32 near, f32 far, f32 aspect, f32 fov);

	Vector3 screenToWorld(Vector2 screenPosition);
	void calculateInverseMatrix(bool forceCalculate = false);

	void setZoom(f32 zoom);
	void resetZoom() {
		mZoom = 1;
		setZoom(mZoom);
	};
};
}

