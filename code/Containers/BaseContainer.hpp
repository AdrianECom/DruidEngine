#pragma once

#include "Core/Basic.hpp"
#include "Memory/Allocator.hpp"
#include <string>

namespace DE {

/*!
 \brief Generic container.
 */
class BaseContainer : public ObjectBase {

protected:

	PROTECTED(Length, GET, u32);
	PROTECTED(ElementSize, GET, u32);
	PROTECTED(Alignment, GET, u32);
	PROTECTED(Allocator, GET_SET, Allocator*);

	void init(u32 length, u32 elementSize, u32 alignment);

public:

	GENERATE_METADATA(CONSTRUCTOR, BaseContainer)

	/*!
	\brief Clear the container.
	*/
	virtual void clear();
};
}

