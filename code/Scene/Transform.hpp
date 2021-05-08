#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Matrix4.hpp"

namespace DE {

class Transform : public Component {

private:

	PRIVATE(WorldPosition, NONE, Vector3)

	PRIVATE(ModelMatrix, NONE, mutable Matrix4)
	PRIVATE(TranslationMatrix, NONE, mutable Matrix4)
	PRIVATE(RotationMatrix, NONE, mutable Matrix4)
	PRIVATE(ScaleMatrix, NONE, mutable Matrix4)

	PRIVATE(ModelMatrixGenerated, NONE, bool)

	PRIVATE(IsDirtyTranslation, NONE, mutable bool)
	PRIVATE(IsDirtyRotation, NONE, mutable bool)
	PRIVATE(IsDirtyScale, NONE, mutable bool)

	PRIVATE(ForceModelMatrixCalculation, NONE, bool)

	//Transform* mParent;
	PRIVATE(Parent, GET_SET, Transform*);

	PRIVATE(LocalPosition, GET, Vector3)
	PRIVATE(Rotation, GET, Vector3)
	PRIVATE(Scale, GET, Vector3)
	PRIVATE(AffectedByProjection, GET_SET, bool)

public:

	static const Vector3 smRight;
	static const Vector3 smUp;
	static const Vector3 smForward;

	GENERATE_METADATA(CONSTRUCTOR, Transform)

	void init() override;

	/*!
	\brief Translate the object by an increment vector.
	\param Vector3 Delta position.
	*/
	void translate(const Vector3 &vector);

	/*!
	\brief Rotate the object by an increment vector.
	\param Vector3 Delta rotation.
	*/
	void rotate(const Vector3 &vector);

	/*!
	\brief Orient the object to a 3D point.
	\param Vector3 Target position.
	*/
	void lookAt(const Vector3 &targetPosition);

	// void setWorldPosition(const Vector3& vector);
	void setLocalPosition(const Vector3 &vector);
	void setRotation(const Vector3 &vector);
	void setScale(const Vector3 &vector);

	const Vector3& getWorldPosition();

	const Matrix4& getTranslationMatrix();
	const Matrix4& getRotationMatrix();
	const Matrix4& getScaleMatrix();

	const Matrix4& getModelMatrix(bool force = false);

	bool isDirtyTranslation() const;
	void setDirtyTranslation(bool dirty);

	void forceModelMatrixCalculation() {
		mForceModelMatrixCalculation = true;
	}

	
};
}

