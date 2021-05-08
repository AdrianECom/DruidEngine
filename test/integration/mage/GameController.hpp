#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "PlayerScript.hpp"
#include "Element.hpp"

namespace DE {

class Renderer;
class Transform;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class UIText;
class Material;
template<class T> class Array;
template<class T> class List;
template<class K, class V> class HashMap;

class GameController : public Script {
private:

PRIVATE(Player, NONE, GameObject*)
PRIVATE(PlayerScript, NONE, PlayerScript*)
PRIVATE(CrossHair, NONE, GameObject*)
	// Array<GameObject*>* mRenderFloor;
	List<GameObject*>* mRenderFloor;
	PRIVATE(RenderTilesAmount, NONE, u32)
	List<GameObject*>* mRecycledTiles;
	Array<GameObject*>* mPhysicFloor;
PRIVATE(Background, NONE, GameObject*)
	PRIVATE(RenderTileSize, NONE, f32)

	Array<UIButton*>* mBookSelector;
PRIVATE(SelectedBook, NONE, UIButton*)

PRIVATE(TextMana, NONE, UIText*)

	PRIVATE(EnemySpawnTimeCount, NONE, f32)
	PRIVATE(EnemySpawnTime, NONE, f32)

public:
	static f32 smGlobalSpeed;

	GENERATE_METADATA(CONSTRUCTOR, GameController)

	void createPlayer(f32 x, f32 y);
	void createCrossHair();
	void createEnemy(f32 x, f32 y);
	void createProjectile(f32 x, f32 y, f32 clickX, f32 clickY);
	GameObject* createRenderFloorTile(f32 x, f32 y);
	GameObject* createPhysicFloorTile(f32 x, f32 y);
	void createBackground(f32 x, f32 y);
	UIButton* createBook(f32 x, f32 y, const Vector4 &color, Element element, f32 size);

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

};
}

