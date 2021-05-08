#pragma once

#include "BaseContainer.hpp"

namespace DE {

/*!
 \brief SequentialContainer.
 \tparam Elements class.
 */
template<class T>
class SequentialContainer : public BaseContainer {

protected:

	/*!
	\param index The index.
	\return Reference to Element at index.
	*/
	virtual T& randomAccessOperator(u32 index) const = 0;

	/*!
	\brief Check boundaries on put method.
	*/
	virtual void checkPut(const SequentialContainer<T> &other, u32 destinyIndex, u32 sourceIndex, u32 length) = 0;

public:

	GENERATE_METADATA(NONE, SequentialContainer<T>)

	/*!
	\brief Default Constructor.
	*/
	SequentialContainer() : BaseContainer() {

	};

	/*!
	\brief Destructor.
	*/
	virtual ~SequentialContainer() override {};

	/*!
	\param index The index.
	\return Element at index.
	*/
	virtual T get(u32 index) const = 0;

	/*!
	\brief Sets element at index.
	\param index The index.
	\param element The element.
	*/
	virtual void set(u32 index, const T element) = 0;

	/*!
	\brief Constructor from raw array.
	\param rawArray The raw array.
	\param length The length of the raw array.
	*/
	virtual void init(const T rawArray[], u32 length) = 0;

	/*!
	\brief Constructor from raw array. Aligned.
	\param rawArray The raw array.
	\param length The length of the raw array.
	\param alignment Bytes alignment.
	*/
	virtual void init(const T rawArray[], u32 length, u32 alignment) = 0;

	/*!
	\brief Constructor.
	\param length Length of the array.
	*/
	virtual void init(u32 length) = 0;

	/*!
	\brief Constructor. Aligned.
	\param length Length of the array.
	\param alignment Bytes alignment.
	*/
	virtual void init(u32 length, u32 alignment) = 0;

	/*!
	\brief Fill the seq. container with the same element.
	\param element The element.
	*/
	virtual void fill(const T element) = 0;

	/*!
	\brief Copy an array into other.
	\param other Other IArray.
	\param destinyIndex Index (of the destiny array) from which to paste the other array.
	\param sourceIndex Index (of the source array) from which to copy.
	*/
	void put(const SequentialContainer<T> &other, u32 destinyIndex, u32 sourceIndex) {
		this->put(other, destinyIndex, sourceIndex, other.getLength());
	}

	/*!
	\brief Copy an array into other.
	\param other Other IArray.
	\param destinyIndex Index (of the destiny array) from which to paste the other array.
	\param sourceIndex Index (of the source array) from which to copy.
	*/
	virtual void put(const SequentialContainer<T> &other, u32 destinyIndex, u32 sourceIndex, u32 length) {
		this->checkPut(other, destinyIndex, sourceIndex, length);

		FOR_RANGE(i,0,length)
		{
			(*this)[destinyIndex + i] = other[sourceIndex + i];
		}
	}

	/*!
	\brief Can be used for assignment.
	\param i Index.
	\return Element reference.
	*/
	T& operator[](const size_t index) {
		return this->randomAccessOperator(index);
	}

	/*!
	\brief Read only.
	\param i Index.
	\return Element reference.
	*/
	T operator[](const size_t index) const {
		return this->randomAccessOperator(index);
	}
};
}

