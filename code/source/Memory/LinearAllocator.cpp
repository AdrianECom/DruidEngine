#include "LinearAllocator.h"

namespace DE {

  // ---------------------------------------------------------------------------

LinearAllocator::LinearAllocator() : Allocator(){

}

LinearAllocator::~LinearAllocator(){

}

// ---------------------------------------------------------------------------

void LinearAllocator::init(u32 size){
	TRACE();

  Allocator::init(size); TRACE()
}

// ---------------------------------------------------------------------------

void* LinearAllocator::allocate(u32 size){
  return LinearAllocator::allocate(size, 1);
}

// ---------------------------------------------------------------------------

void* LinearAllocator::allocate(u32 size, u32 alignment){
  return Allocator::allocateAlignedAddress((void*)(reinterpret_cast<byte*>(mStart) + Allocator::getAllocatedSize()), size, alignment);
}

// ---------------------------------------------------------------------------

void LinearAllocator::free(const void* pointer){
  // DE_ASSERT(false, "LinearAllocator can't use free(void* pointer), use reset().");
}

// ---------------------------------------------------------------------------

void LinearAllocator::reset(){
  Allocator::reset();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
