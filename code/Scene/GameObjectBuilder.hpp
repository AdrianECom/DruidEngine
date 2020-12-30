#ifndef DE_GAMEOBJECTBUILDER_H
#define DE_GAMEOBJECTBUILDER_H

#include "DE_Class.hpp"
#include "Singleton.hpp"
#include "GameObject.hpp"
#include "Vector3.hpp"

namespace DE {

class Material;

// ---------------------------------------------------------------------------

class GameObjectBuilder: public DE_Class, public Singleton<GameObjectBuilder> {

private:

	DE_M_GET(GameObject, GameObject*)

public:

	DE_CLASS(GameObjectBuilder, DE_Class)

	GameObjectBuilder* createSprite(Material* material, u32 layer, bool isStatic);
};

// ---------------------------------------------------------------------------

} /* namespace DE */

#endif /* GAMEOBJECT_H */