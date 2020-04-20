#ifndef DE_GameController_H
#define DE_GameController_H

#include "Script.h"
#include "Vector2.h"
#include "Vector4.h"
#include "PlayerScript.h"

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
template <class T> class Array;
template <class T> class List;
template <class K, class V> class HashMap;

class GameController : public Script{
private:

  GameObject* mPlayer;
  // Array<GameObject*>* mRenderFloor;
  List<GameObject*>* mRenderFloor;
  u32 mRenderTilesAmount;
  List<GameObject*>* mRecycledTiles;
  Array<GameObject*>* mPhysicFloor;
  GameObject* mBackground;
  f32 mRenderTileSize;

  Array<UIButton*>* mBookSelector;
  UIButton* mSelectedBook;

  // UIText* mText;
  // f32 mTextTimeCount;
  // f32 mTextTime;

public:

DE_CLASS(GameController, Script);

void createPlayer(f32 x, f32 y);
void createEnemy(f32 x, f32 y);
void createProjectile(f32 x, f32 y, f32 clickX, f32 clickY);
GameObject* createRenderFloorTile(f32 x, f32 y);
GameObject* createPhysicFloorTile(f32 x, f32 y);
void createBackground(f32 x, f32 y);
UIButton* createBook(f32 x, f32 y, const Vector4& color, f32 size);

void init() override;
void firstStep() override;
void step() override;
void terminate() override;

};

} /* namespace DE */

#endif /* DE_GameController_H */
