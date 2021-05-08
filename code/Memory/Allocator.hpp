#pragma once

#include "Core/Basic.hpp"

namespace DE {

/*!
 \brief Manages memory allocation.
 */
class Allocator : public ObjectBase {

protected:

PROTECTED(Start, NONE, byte*)

protected:

	void checkAllocate(u32 size) const;
	void checkAlignment(u32 alignment) const;
	void checkFree() const;

	byte* calculateAlignedAddress(const byte *unalignedAddress, u32 alignment) const;
	byte* calculateUnalignedAddress(const byte *alignedAddress) const;
	byte* allocateAlignedAddress(byte *unalignedAddress, u32 size, u32 alignment);

	void setAllocatedSize(u32 size);
	void clean(byte *mem, u32 size);
	void setMemoryChunk(byte *mem);

public:

	GENERATE_METADATA(CONSTRUCTOR, Allocator)

	// Debug

	PUBLIC(TotalSize, NONE, u32)
	PUBLIC(AllocatedSize, NONE, u32)
	PUBLIC(DebugMaxAllocatedSize, NONE, u32)

	/*!
	\return True if space is enough.
	\param size Size you want to check.
	*/
	bool hasSpace(u32 size) const;

	/*!
	\brief Constructor.
	\param size Amount of memory you want to allocate.
	*/
	virtual void init(u32 size);

	/*!
	\brief Constructor.
	\param size Amount of memory you want to allocate.
	\param mem Pointer to pre-allocated memory chunk.
	*/
	virtual void initFromMemory(u32 size, byte *mem);

	/*!
	\brief Allocates memory.
	\param size Amount of memory you want to allocate.
	\return Pointer to memory chunk.
	*/
	virtual byte* allocate(u32 size) = 0;

	/*!
	\brief Allocates memory.
	\param size Amount of memory you want to allocate.
	\param alignment Bytes alignment.
	\return Pointer to memory chunk.
	*/
	virtual byte* allocate(u32 size, u32 alignment) = 0;

	/*!
	\brief Frees memory.
	\param pointer Pointer to memory.
	*/
	virtual void free(const byte *pointer) = 0;

	/*!
	\brief Cleaning step. Used by FreeListAllocator to remove invalid blocks.
	*/
	virtual void flush();

	virtual void reset();

	/*!
	\brief Frees aligned memory.
	*/
	virtual void terminate();

	/*!
	\brief Constructs objects. It is used like "new" keyword.
	Allocate an object of T class, using the allocator.

	\tparam T ObjectBase.
	\param allocator Allocator used to allocate memory.
	\param alignment Bytes alignment.
	*/
	template<class T>
	static T* internalAllocate(Allocator *allocator, u32 alignment) {
		T* object = new (allocator->allocate(sizeof(T), alignment)) T;
		return object;
	};

	/*!
	\brief Constructs objects. It is used like "new" keyword.
	Allocate an object of T class, using the allocator.

	\tparam T ObjectBase
	\param allocator Allocator used to allocate memory.
	*/
	template<class T>
	static T* internalAllocate(Allocator *allocator) {
		T* object = new (allocator->allocate(sizeof(T))) T;
		return object;
	};

	/*!
	\brief Destroys objects. It is used like "delete" keyword.
	Deallocate data, using the allocator.
	\param allocator Allocator used to deallocate memory.
	*/
	static void internalFree(ObjectBase *object, Allocator *allocator) {
		if (object != nullptr) {
			object->dynamicDestructor(); // ~T(); 
			allocator->free(reinterpret_cast<byte*>(object));
		}
	};
};

}

