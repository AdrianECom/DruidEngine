#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include <string>

namespace DE {

class Transform;
class Renderer;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class UIText;
class Texture;
class Material;
template<class K, class V> class HashMap;
template<class T> class Array;
template<class T> class List;

class Playground : public Script {
private:

	PRIVATE(Transform, NONE, Transform*)
	PRIVATE(Camera, NONE, Camera*)
	PRIVATE(CameraTransform, NONE, Transform*)

	PRIVATE(CameraControl, NONE, bool)

public:

	PUBLIC(Player, NONE, GameObject*)
	PUBLIC(Material, NONE, Material*)

	GENERATE_METADATA(CONSTRUCTOR, Playground)

	GameObject* createProjectile(f32 x, f32 y);
	void createPlayer();
	void processMovement();

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;
};
}

