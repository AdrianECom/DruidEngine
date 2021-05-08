#pragma once

#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "Core/Singleton.hpp"

namespace DE {

class UIButton;
class UIDropdown;
class UIText;
class UITextEditable;

// Vector2 Text Editable
class UITextEditableVector2 {
public:
	UITextEditable* TextEditableX = nullptr;
	UITextEditable* TextEditableY = nullptr;
};

class EditorBuilder : public ObjectBase, public Singleton<EditorBuilder>{

public:

	GENERATE_METADATA(CONSTRUCTOR, EditorBuilder)

	u32 mUILayer = 3;

	UIElement* createPanel(const Vector2& position, const Vector2& size);
	UIText* createLabel(StringRef text);
	UITextEditable* createTextBoxSimple(StringRef text, UIElementCallback onTextChangedCallback);
	UITextEditable* createTextBoxLabeled(StringRef textLabel, StringRef text, UIElementCallback onTextChangedCallback);
	UIButton* createButton(StringRef textLabel, UIElementCallback onPressedCallback);
	UIButton* createRadialButton(StringRef textLabel, UIElementCallback onPressedCallback);
	UITextEditableVector2 createTextBoxVector2(StringRef textLabel, UIElementCallback onTextChangedCallbackX, UIElementCallback onTextChangedCallbackY);
	UIDropdown* createDropdown(StringRef textLabel);
};
}

