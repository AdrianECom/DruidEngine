#pragma once

#include "SequentialContainer.hpp"
#include <cstring>

namespace DE {

/*!
 \brief Array of elements. Fixed size.
 */
template<class T>
class Array: public SequentialContainer<T> {

private:

	PRIVATE(TStart, NONE, T*)
	PRIVATE(Start, NONE, byte*)
		
	T& randomAccessOperator(u32 index) const {
		ASSERT(index >= 0 && index < this->getLength(), "Index out of bounds.");
		return mTStart[index];
	}
	
	void checkPut(const SequentialContainer<T> &other, u32 destinyIndex, u32 sourceIndex, u32 length) override {
		ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
		ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");
		ASSERT(length <= other.getLength() - sourceIndex, "Not enough space to copy.");
		ASSERT(length <= this->getLength() - destinyIndex, "Not enough space to put array.");
	}
	
	void raw_copy(const Array &other) {
		raw_init(other.mTStart, other.mLength, other.mElementSize, other.mAlignment);
	}
	
	void raw_init(const T rawArray[], u32 length, u32 elementSize) {
		raw_init(length, elementSize, 1);
		raw_set(reinterpret_cast<const byte*>(rawArray));
	}

	void raw_init(const T rawArray[], u32 length, u32 elementSize, u32 alignment) {
		raw_init(length, elementSize, alignment);
		raw_set(reinterpret_cast<const byte*>(rawArray));
	}

	void raw_init(u32 length, u32 elementSize) {
		raw_allocate(length, elementSize, 1);
	}

	void raw_init(u32 length, u32 elementSize, u32 alignment) {
		raw_allocate(length, elementSize, alignment);
	}

	void raw_set(const byte *rawArray) {
		std::memcpy(mStart, rawArray, this->mLength * (this->mElementSize));
	}

	void raw_put(const byte *rawArray, u32 destinyIndex, u32 sourceIndex, u32 length) {
		std::memcpy(mStart + destinyIndex * (this->mElementSize), rawArray + sourceIndex * (this->mElementSize),
				length * (this->mElementSize));
	}
	
	void raw_allocate(u32 length, u32 elementSize, u32 alignment) {
		BaseContainer::init(length, elementSize, alignment);
		mStart = this->mAllocator->allocate(this->mLength * (this->mElementSize), this->mAlignment);
		// TODO allocate with Memory::allocate, create new function for raw allocation.
	}

	void raw_clear() {
		f32 length = this->mLength;
		BaseContainer::clear();
		this->mLength = length;
		std::memset(mStart, 0, this->mLength * (this->mElementSize));
	}
	
public:

	using SequentialContainer<T>::put; // because "put" method is ambiguous.

	GENERATE_METADATA(NONE, Array<T>)

	/*!
	\brief Default Constructor.
	*/
	Array() : 	SequentialContainer<T>(){
		mStart = nullptr;
		mTStart = nullptr;
	}

	/*!
	\brief Destructor.
	*/
	virtual ~Array() override {
		if (mStart != nullptr) {
			this->mAllocator->free(mStart);
			mStart = nullptr;
			mTStart = nullptr;
		}
	}

	/*!
	\brief Copy Constructor.
	\param other Other Array.
	*/
	void init(const Array<T> &other) {
		raw_copy(other);
		mTStart = reinterpret_cast<T*>(mStart);
	}

	void fill(const T element) override {
		FOR_RANGE(i, 0, SequentialContainer<T>::mLength) {
			mTStart[i] = element;
		}
	}

	void init(const T rawArray[], u32 length) override {
		raw_init(rawArray, length, sizeof(T));
		mTStart = reinterpret_cast<T*>(mStart);
	}

	void init(const T rawArray[], u32 length, u32 alignment) override {
		raw_init(rawArray, length, sizeof(T), alignment);
		mTStart = reinterpret_cast<T*>(mStart);
	}

	void init(u32 length) override {
		raw_init(length, sizeof(T));
		mTStart = reinterpret_cast<T*>(mStart);
	}

	void init(u32 length, u32 alignment) override {
		raw_init(length, sizeof(T), alignment);
		mTStart = reinterpret_cast<T*>(mStart);
	}

	void put(const Array<T> &other, u32 destinyIndex, u32 sourceIndex) {
		Array::put(other, destinyIndex, sourceIndex, other.getLength());
	}

	void put(const Array<T> &other, u32 destinyIndex, u32 sourceIndex, u32 length) {
		this->checkPut(other, destinyIndex, sourceIndex, length);
		raw_put(other.mStart, destinyIndex, sourceIndex, length);
	}

	T get(u32 index) const override {
		ASSERT(index >= 0 && index < SequentialContainer<T>::getLength(), "Index out of bounds.");
		return mTStart[index];
	}

	void set(u32 index, const T element) override {
		ASSERT(index >= 0 && index < SequentialContainer<T>::getLength(), "Index out of bounds.");
		mTStart[index] = element;
	}

	void clear() override {
		raw_clear();
	}

	const T* getRawData() const {
		return mTStart;
	}
};
}

