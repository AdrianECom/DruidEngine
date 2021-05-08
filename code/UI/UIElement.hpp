#pragma once

#include "Scene/GameObject.hpp"
#include "Core/Functor.hpp"
#include "Graphics/RenderContext.hpp"
#include <string>

namespace DE {

class Collider;
class Renderer;
class UIGroup;

class UIElement;

using UIElementCallback = std::function<void(UIElement* uiElement)>;

class FunctorUIElement : public Functor<UIElementCallback> {
public:
	GENERATE_METADATA(NONE, FunctorUIElement)

	PUBLIC(UIElement, NONE, UIElement*)

	FunctorUIElement():Functor<UIElementCallback>() {
		mUIElement = nullptr;
	};
	~FunctorUIElement() override {};

	virtual void execute() override {
		if (mCallback)
			mCallback(mUIElement);
	}

	FunctorUIElement& operator= (const FunctorUIElement &functor) {
		// self-assignment guard
		if (this == &functor)
			return *this;

		// do the copy
		mUIElement = functor.mUIElement;
		Functor<UIElementCallback>::operator =(functor);

		return *this;
	}

};

class UIElement : public GameObject {

protected:

	PROTECTED(OnPressedFunctor, NONE, FunctorUIElement)
	PROTECTED(OnReleasedFunctor, NONE, FunctorUIElement)

	PROTECTED(OnTextChangedFunctor, NONE, FunctorUIElement)
	PROTECTED(OnFocusLostFunctor, NONE, FunctorUIElement)

	PRIVATE(Renderer, GET, Renderer*)
	PRIVATE(Collider, GET, Collider*)
	PROTECTED(InputString, GET, String)
	PROTECTED(ConsumeInput, GET_SET, bool)
	PRIVATE(Group, GET_SET, UIGroup*)
	PROTECTED(Pressed, GET, bool)

public:

	GENERATE_METADATA(CONSTRUCTOR, UIElement)

	virtual void init() override;
	virtual void onDestroy();

	void subscribeToKeyEvents();
	void subscribeToCharEvents();
	void subscribeToMouseButtonEvents();
	void subscribeToEnterEvent();
	void subscribeToEscEvent();

	bool hasFocus() const;

	void onChar(c8 character);

	virtual void setText(const String &text) = 0;

	void onPressed();
	void onReleased();

	virtual void onFocusLost();
	void onFocus();

	void setOnPressedCallback(UIElementCallback callback) {
		mOnPressedFunctor.mUIElement = this;
		mOnPressedFunctor.setCallback(callback);
	}

	void setOnReleasedCallback(UIElementCallback callback) {
		mOnReleasedFunctor.mUIElement = this;
		mOnReleasedFunctor.setCallback(callback);
	}

	void setOnTextChangedCallback(UIElementCallback callback) {
		mOnTextChangedFunctor.mUIElement = this;
		mOnTextChangedFunctor.setCallback(callback);
	}

	void setOnFocusLostCallback(UIElementCallback callback) {
		mOnFocusLostFunctor.mUIElement = this;
		mOnFocusLostFunctor.setCallback(callback);
	}

	void setComponentsCache();

	virtual void setVisibility(bool visibility);
	bool isVisible();

	virtual void toggle() {};
};

}

