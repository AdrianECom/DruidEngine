#pragma once

#include "UI/UIElement.hpp"
#include "Maths/Vector2.hpp"
#include <string>

namespace DE {

class UIText : public UIElement {

	PRIVATE(Layer, NONE, u32)
	PRIVATE(Size, NONE, Vector2)
	PRIVATE(String, NONE, String)

public:

	GENERATE_METADATA(CONSTRUCTOR, UIText)

	virtual void init() override;
	virtual void onDestroy() override;

	virtual void setText(const String &text) override;

	void setLayer(u32 layer) {
		mLayer = layer;
	};

	void setSize(const Vector2 &size) {
		mSize = size;
	};

	StringRef getText() const {
		return mString;
	};
};

}

