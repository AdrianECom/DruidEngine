#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "Element.hpp"

namespace DE {

class UIText;
class Renderer;
class RigidBody;

class PlayerScript : public Script {

private:
PRIVATE(Renderer, NONE, Renderer*)
PRIVATE(RigidBody, NONE, RigidBody*)
	PRIVATE(AttackAnimTime, NONE, f32)
	PRIVATE(AttackAnimTimeCount, NONE, f32)
	PRIVATE(IsAttackPlaying, NONE, bool)

PRIVATE(Book, NONE, GameObject*)
	PRIVATE(BookColor, NONE, Vector4)
	PRIVATE(Element, NONE, Element)
	PRIVATE(Mana, NONE, u32)
	PRIVATE(ManaChanged, NONE, bool)

public:

	GENERATE_METADATA(CONSTRUCTOR, PlayerScript)

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void onEnterCollision(GameObject *otherGameObject) override;
	void onCollision(GameObject *otherGameObject) override;
	void onExitCollision(GameObject *otherGameObject) override;

	void createProjectile(f32 x, f32 y, f32 clickX, f32 clickY);
	void createWall(f32 x, f32 y);
	void createBook(f32 x, f32 y, const Vector4 &color);

	void setBookColor(const Vector4 &color) {
		mBookColor = color;
	}
	void setElement(Element element) {
		mElement = element;
	}

	u32 getMana() const {
		return mMana;
	}
	bool isManaChanged() const {
		return mManaChanged;
	}
	void setMana(u32 mana) {
		mMana = mana;
	}

};
}

