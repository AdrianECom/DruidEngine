#pragma once

#include "UI/UIText.hpp"

namespace DE {

class UITextEditable : public UIText {

public:

	GENERATE_METADATA(CONSTRUCTOR, UITextEditable)

	virtual void init();
	virtual void onFocusLost();

};
}

