#pragma once

#include "Memory/Allocator.hpp"
#include "Core/Basic.hpp"

namespace DE {

/*!
 \brief Allocates memory in a linear way. The whole memory is freed in one shot.
 */
class LinearAllocator : public Allocator {

public:

	GENERATE_METADATA(CONSTRUCTOR, LinearAllocator)

	void init(u32 size) override;
	byte* allocate(u32 size) override;
	byte* allocate(u32 size, u32 alignment) override;
	void free(const byte *pointer) override;
	void reset() override;

};
}

