#pragma once

#include "Containers/Array.hpp"
#include "Containers/List.hpp"
#include "Core/Basic.hpp"
#include "Memory/Allocator.hpp"
#include "Memory/Memory.hpp"

namespace DE {

/*!
 \brief DynamicArray of elements. Variable size.
 \tparam Elements class.
 */
template<class T>
class DynamicArray: public SequentialContainer<T> {

	template<class K, class V>
	friend class HashMap; // Friend ObjectBase

private:

	static const u32 smMinSize = 100;
	mutable Array<T>* mCache = nullptr;
	mutable u32 mCacheIndex = 0;
	List<Array<T>*>* mArrays;

	void _updateCache(u32 arrayIndex) const { // const method, but mutable variables.
		if (arrayIndex != mCacheIndex || mCache == nullptr) {
			mCache = mArrays->get(arrayIndex);
			mCacheIndex = arrayIndex;
		}
	}

	T& randomAccessOperator(u32 index) const {
		ASSERT(index >= 0, "Index out of bounds.");

		u32 realIndex = index % smMinSize;
		u32 arrayIndex = ceil(index / smMinSize);

		DynamicArray::_updateCache(arrayIndex);
		return (*mCache)[realIndex];
	}

	void checkPut(const SequentialContainer<T> &other, u32 destinyIndex, u32 sourceIndex, u32 length) override {
		ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
		ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");
		ASSERT(length <= other.getLength() - sourceIndex, "Not enough space to copy.");
	}

	
public:

	GENERATE_METADATA(NONE, DynamicArray<T>)

	/*!
	\brief Default Constructor.
	*/
	DynamicArray() : 	SequentialContainer<T>() {
		mArrays = nullptr;
		mCache = nullptr;
	}

	/*!
	\brief Destructor.
	*/
	virtual ~DynamicArray() override {
		if (mArrays != nullptr) {

			FOR_LIST (it, mArrays) {
				Memory::free(it.get());
			}

			Memory::free(mArrays);
		}
	}

	/*!
	\brief Copy Constructor.
	\param other Other DynamicArray.
	*/
	void init(const DynamicArray<T> &other) {
		BaseContainer::init(other.BaseContainer::mLength, other.mElementSize, other.BaseContainer::mAlignment);
		mArrays = Memory::allocate<List<Array<T>*> >();
		mArrays->init();

		FOR_LIST (it, other.mArrays)
		{
			Array<T>* otherArray = it.get();
			Array<T>* array = Memory::allocate<Array<T>>();
			array->init(*otherArray);
			mArrays->pushBack(array);
		}
	}

	/*!
	\brief Copy Constructor.
	\param other Other Array.
	*/
	void init(const Array<T> &other) {
		u32 otherLength = other.getLength();
		u32 otherOffset = 0;

		DynamicArray::init(otherLength, other.getAlignment());

		FOR_LIST_COND(it, mArrays, otherOffset < otherLength)
		{
			Array<T>* array = it.get();

			if (otherLength < smMinSize)
				array->put(other, 0, 0);
			else
				array->put(other, 0, otherOffset, smMinSize - 1);

			otherOffset += smMinSize;
		}
	}

	/*!
	\brief Constructor from raw array.
	\param rawArray The raw array.
	\param length The length of the raw array.
	*/
	void init(const T rawArray[], u32 length) override {
		DynamicArray::init(rawArray, length, 1);
	}

	/*!
	\brief Constructor from raw array. Aligned.
	\param rawArray The raw array.
	\param length The length of the raw array.
	\param alignment Bytes alignment.
	*/
	void init(const T rawArray[], u32 length, u32 alignment) override {
		BaseContainer::setAllocator(&Memory::getGlobal());
		Array<T>* array = Memory::allocate<Array<T>>(alignment);
		array->init(rawArray, length);
		DynamicArray::init(*array);
	}

	/*!
	\brief Constructor.
	*/
	void init() {
		DynamicArray::init(0);
	}

	/*!
	\brief Constructor.
	\param length Length of the array.
	*/
	void init(u32 length) {
		DynamicArray::init(length, 1);
	}

	/*!
	\brief Constructor. Aligned.
	\param length Length of the array.
	\param alignment Bytes alignment.
	*/
	void init(u32 length, u32 alignment) {
		BaseContainer::init(length, sizeof(T), alignment);

		// list of arrays
		mArrays = Memory::allocate<List<Array<T>*> >();
		mArrays->init();

		// how many arrays are needed.
		u32 arrayCount = ceil(length / smMinSize) + 1;

		FOR_RANGE(i, 0, arrayCount)
		{
			Array<T>* newArray = Memory::allocate<Array<T>>(SequentialContainer<T>::mAlignment);
			newArray->init(smMinSize);

			mArrays->pushBack(newArray);
		}
	}

	/*!
	\brief Fill the array with the same element.
	\param element The element.
	*/
	void fill(const T element) {

		FOR_LIST(it, mArrays)
		{
			Array<T>* array = it.get();
			array->fill(element);
		}
	}

	/*!
	\param index The index.
	\return Element at index.
	*/
	T get(u32 index) const {
		return DynamicArray::randomAccessOperator(index);
	}

	/*!
	\brief Sets element at index.
	\param index The index.
	\param element The element.
	*/
	void set(u32 index, const T element) {
		// resize
		if (index >= mArrays->getLength() * smMinSize) {
			Array<T>* newArray = Memory::allocate<Array<T>>(SequentialContainer<T>::mAlignment);
			newArray->init(smMinSize);

			mArrays->pushBack(newArray);
		}

		SequentialContainer<T>::mLength = std::max(index + 1, SequentialContainer<T>::mLength); // save the max index accessed

		(*this)[index] = element;
	}

	void clear() override {
		BaseContainer::clear();

		FOR_LIST(it, mArrays)
		{
			it.get()->clear();
		}

		mArrays->clear();

		mCache = nullptr;
		mCacheIndex = 0;
	}
};
}

