#pragma once

#include "Core/ObjectBase.hpp"
#include <functional>

namespace DE {

template <class T>
class Functor : public ObjectBase {

protected:

	PROTECTED(Callback, NONE, T)

public:

	GENERATE_METADATA(NONE, Functor<T>)

	Functor() : ObjectBase(){};
	virtual ~Functor() override {};

	virtual void execute() = 0;

	void setCallback(T newCallback) {
		mCallback = newCallback;
	};

	// NOTE : Override in children!
	Functor& operator= (const Functor &functor) {
		// self-assignment guard
		if (this == &functor)
			return *this;

		// do the copy
		mCallback = functor.mCallback;

		return *this;
	}

	bool operator== (const Functor& functor) const{
	   return this == &functor;
	}
};

class FunctorVoid : public Functor<std::function<void()>> {
public:

	GENERATE_METADATA(NONE, FunctorVoid)

	FunctorVoid() : Functor<std::function<void()>>(){};
	virtual ~FunctorVoid() override {};

	virtual void execute() override {
		if (mCallback)
			mCallback();
	}

};
}

