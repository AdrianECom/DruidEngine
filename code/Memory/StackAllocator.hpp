#pragma once

#include "Memory/LinearAllocator.hpp"
#include "Core/Basic.hpp"

namespace DE {

/*!
 \brief Allocates memory in FIFO strategy.
 */
class StackAllocator : public LinearAllocator {

private:

PRIVATE(Top, NONE, byte*)

	static const u32 smHeaderSize;
	void storeHeader(const byte *address, u32 size);

public:

	GENERATE_METADATA(CONSTRUCTOR, StackAllocator)

	/*!
	\return Pointer to the top of the stack.
	*/
	byte* getTop();

	void init(u32 size) override;
	virtual void initFromMemory(u32 size, byte *mem) override;
	virtual byte* allocate(u32 size) override;
	virtual byte* allocate(u32 size, u32 alignment) override;
	virtual void free(const byte *pointer) override;

	/*!
	\brief Free the top of the stack.
	*/
	void free();

	void reset() override;

};
}

