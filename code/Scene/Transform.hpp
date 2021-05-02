#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Matrix4.hpp"

namespace DE {

class Transform : public Component {

private:

	 Vector3 mWorldPosition;

	mutable Matrix4 mModelMatrix;
	mutable Matrix4 mTranslationMatrix;
	mutable Matrix4 mRotationMatrix;
	mutable Matrix4 mScaleMatrix;

	 bool mModelMatrixGenerated;

	mutable bool mIsDirtyTranslation;
	mutable bool mIsDirtyRotation;
	mutable bool mIsDirtyScale;

	 bool mForceModelMatrixCalculation;

	 Transform* mParent;

	 Vector3 mLocalPosition;
	 Vector3 mRotation;
	 Vector3 mScale;
	 bool mAffectedByProjection;

public:

	static const Vector3 smRight;
	static const Vector3 smUp;
	static const Vector3 smForward;

	GENERATE_METADATA(Transform);

	Transform();
	virtual ~Transform() override;;

	GET_SET(Parent);

	GET(LocalPosition);
	GET(Rotation);
	GET(Scale);
	GET_SET(AffectedByProjection);

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

