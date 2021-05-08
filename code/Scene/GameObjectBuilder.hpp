#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Scene/GameObject.hpp"
#include "Maths/Vector3.hpp"

namespace DE {

class Material;

class GameObjectBuilder: public ObjectBase, public Singleton<GameObjectBuilder> {

private:

PRIVATE(GameObject, NONE, GameObject*)

public:

	GENERATE_METADATA(CONSTRUCTOR, GameObjectBuilder)

	GET(GameObject);

	GameObjectBuilder* createSprite(Material* material, u32 layer, bool isStatic);
};
}

