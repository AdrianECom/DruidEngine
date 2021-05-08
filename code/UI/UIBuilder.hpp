#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "UI/UIElement.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIText.hpp"
#include "UI/UITextEditable.hpp"
#include "UI/UIDropdown.hpp"

#include <string>

namespace DE {

class Texture;
class Material;
class Scene;

template<class T> class List;
template<class K, class V> class HashMap;

enum class UIElementType {
	PANEL,
	BUTTON,
	TEXT,
	TEXTEDITABLE,
	DROPDOWN
};

enum class UILayout {
	HORIZONTAL,
	VERTICAL,
	MAX
};

class UIElementData : public ObjectBase {
public:

	GENERATE_METADATA(CONSTRUCTOR, UIElementData)

	PUBLIC(ElementType, NONE, UIElementType)
	PUBLIC(Position, NONE, Vector2)
	PUBLIC(DisplayPosition, NONE, Vector2)
	PUBLIC(Size, NONE, Vector2)
	PUBLIC(TextSize, NONE, Vector2)
	PUBLIC(Text, NONE, String)
	PUBLIC(AdjustSizeToText, NONE, bool)
	PUBLIC(Layer, NONE, u32)
	PUBLIC(IsAffectedByLayout, NONE, bool)
	PUBLIC(Group, NONE, String)
	PUBLIC(SeparatorSize, NONE, f32)
	PUBLIC(BackgroundColor, NONE, Vector4)
	Vector4 mBackgroundColor2;
	Vector4 mBackgroundColor3;
	Vector4 mBackgroundColor4;

	void init(const Vector2 &position, const Vector2 &size, StringRef text, u32 layer);

	UIElementData& operator=(const UIElementData& otherData)
	{
		// check for self-assignment
		if(&otherData == this)
			return *this;

		mElementType = otherData.mElementType;
		mPosition = otherData.mPosition;
		mDisplayPosition = otherData.mDisplayPosition;
		mSize = otherData.mSize;
		mTextSize = otherData.mTextSize;
		if(!otherData.mText.empty()){mText = otherData.mText;} 
		mAdjustSizeToText = otherData.mAdjustSizeToText;
		mLayer = otherData.mLayer;
		mIsAffectedByLayout = otherData.mIsAffectedByLayout;
		if(!otherData.mGroup.empty()){mGroup = otherData.mGroup;} 
		mSeparatorSize = otherData.mSeparatorSize;
		mBackgroundColor = otherData.mBackgroundColor;
		mBackgroundColor2 = otherData.mBackgroundColor2;
		mBackgroundColor3 = otherData.mBackgroundColor3;
		mBackgroundColor4 = otherData.mBackgroundColor4;

		return *this;
	}
};

#define UI_BUILDER_DATA_SETTER(ObjectBase, Name) UIBuilder* const set##Name(ObjectBase _##Name)\
 { mData.m##Name = _##Name; return this; }

class UIBuilder: public ObjectBase, public Singleton<UIBuilder> {
private:

PRIVATE(Scene, NONE, Scene*)
	PRIVATE(CurrentLayout, NONE, UILayout)
PRIVATE(ButtonMaterial, NONE, Material*)
	PRIVATE(Data, NONE, UIElementData)
	List<UIElementData>* mDataStack;
	PRIVATE(LastData, NONE, UIElementData)
	PRIVATE(MakeRelativeToLastData, NONE, bool) // used for layouts
	PRIVATE(LayoutFirstUIElementData, NONE, UIElementData)
	PRIVATE(NewRowOrColumn, NONE, bool)
PRIVATE(CurrentUIElement, NONE, UIElement*)

	void registerUIElement(UIElement* uiElement);
	void registerCurrentUIElement(UIElement* uiElement);
	UILayout getOppositeLayout(UILayout layout);
	Vector2 calculateNextElementOffset(UILayout layout);
	void calculateData();
	UIElement* internalCreatePanel();
	UIButton* internalCreateButton();
	UIText* internalCreateText();
	UITextEditable* internalCreateTextEditable();
	UIDropdown* internalCreateDropdown();

public:

	GENERATE_METADATA(CONSTRUCTOR, UIBuilder)

	UIBuilder* const setLayout(UILayout layout) {
		mCurrentLayout = layout;
		mMakeRelativeToLastData = false; // reset
		mNewRowOrColumn = true;
		return this;
	}

	UIBuilder* const nextRow();
	UIBuilder* const nextColumn();

	UIBuilder* const setScene(Scene *scene) { mScene = scene; return this; }
	UIBuilder* const setData(UIElementData data) { mData = data; return this; }

	UI_BUILDER_DATA_SETTER(bool, IsAffectedByLayout)
	UI_BUILDER_DATA_SETTER(const Vector2&, Position)
	UI_BUILDER_DATA_SETTER(const Vector2&, Size)
	UI_BUILDER_DATA_SETTER(u32, Layer)
	UI_BUILDER_DATA_SETTER(StringRef, Text)
	UI_BUILDER_DATA_SETTER(Vector2, TextSize)
	UI_BUILDER_DATA_SETTER(bool, AdjustSizeToText)
	UI_BUILDER_DATA_SETTER(String, Group)
	UI_BUILDER_DATA_SETTER(const Vector4&, BackgroundColor)
	UI_BUILDER_DATA_SETTER(f32, SeparatorSize)

	UIBuilder* const restoreColors() {
		mData.mBackgroundColor = Vector4(0.5,0.5,0.5,1);
		mData.mBackgroundColor2 = Vector4(0.6,0.6,0.6,1);
		mData.mBackgroundColor3 = Vector4(0.4,0.4,0.4,1);
		mData.mBackgroundColor4 = Vector4(0.5,0.5,0.5,0.7);
		return this;
	}

	UIBuilder* const restoreSeparatorSize() {
		mData.mSeparatorSize = 0.01f;
		return this;
	}

	UIBuilder* const create(UIElementType type);

	UIBuilder* const saveData();
	UIBuilder* const restoreData();

	UIElement* getUIElement();
};
}

