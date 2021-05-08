#pragma once

#include "SequentialContainer.hpp"
#include "Memory/Allocator.hpp"
#include "Core/Basic.hpp"

namespace DE {

/*!
 \brief List of elements. Variable size.
 */
template<class T>
class List;

class Iterator : public ObjectBase {

public:

	GENERATE_METADATA(NONE, Iterator)

	Iterator() : ObjectBase() { };

	virtual ~Iterator() override { };

	virtual bool isNull() const = 0;

	template<class T>
	const typename List<T>::ListIterator* cast() const {
		return static_cast<const typename List<T>::ListIterator*>(this);
	}
};

/*
 NOTE : Here we cant use Memory.h, because List.h is used by FreeListAllocator.h.
 */

template<class T>
class List: public SequentialContainer<T> {

	friend class ListIterator;

private:

	class Node : public ObjectBase {

	public:
		List<T>::Node* mNext;
		List<T>::Node* mPrev;
		T mElement;

		GENERATE_METADATA(NONE, Node)

		Node() : ObjectBase() {
		}

		virtual ~Node() override {
		}

		void init() {
			mNext = nullptr;
			mPrev = nullptr;
		}

		void init(T element) {
			Node::init();

			// TODO : wrap in if(T is same String) ??
			// The bug hapens only with empty strings ("").
#if defined(_MSC_VER)
			mElement = T(element);
#else
			mElement = element;
#endif

		}
	};

	static const u32 smNodeSize = sizeof(Node);

	List<T>::Node* mFirst;
	List<T>::Node* mLast;

	List<T>::Node* newNode() {
		List<T>::Node* node = reinterpret_cast<List<T>::Node*>(BaseContainer::mAllocator->allocate(sizeof(Node)));
		return node;
	}

	void freeNode(Node *node) {
		BaseContainer::mAllocator->free(reinterpret_cast<byte*>(node));
	}

	static u8 defaultComparator(T a, T b) {
		return (a < b) ? 1 : (b < a) ? 2 : 0;
	}

	i32 partition(u8 (*comparator)(T a, T b), i32 low, i32 high) {

		T pivot = this->get(high);

		i32 i = low - 1;

		for (i32 j = low; j <= high - 1; j++) {
			if (comparator(this->get(j), pivot) == 1/*this->get(j) < pivot*/) {
				++i;    // increment index of smaller element
				this->swap(i, j);
			}
		}

		this->swap(i + 1, high);

		return i + 1;
	}

	void qsort(u8 (*comparator)(T a, T b), i32 low, i32 high) {

		if (low < high) {
			i32 pivot = partition(comparator, low, high);

			qsort(comparator, low, pivot - 1);
			qsort(comparator, pivot + 1, high);
		}
	}

	
public:

	class ListIterator : public Iterator {

		friend class List;

	private:

		List<T>::Node* mNode;
		bool mReverse;
		
		void init(Node *start) {
			mNode = start;
			mReverse = false;
		}
		
	public:

		GENERATE_METADATA(NONE, ListIterator)

		ListIterator() : 		Iterator() {
			mNode = nullptr;
			mReverse = false;
		}

		ListIterator(const ListIterator &other) : 		Iterator() {
			mNode = other.mNode;
			mReverse = other.mReverse;
		}

		~ListIterator() {
			mNode = nullptr;
			mReverse = false;
		}

		bool isNull() const override {
			return mNode == nullptr;
		}

		bool hasNext() const {
			ASSERT(mNode != nullptr, "Node is null.");

			if (mReverse)
				return (mNode->mPrev != nullptr);
			else
				return (mNode->mNext != nullptr);
		}

		void next() {
			ASSERT(mNode != nullptr, "Node is null.");

			if (mReverse)
				mNode = mNode->mPrev;
			else
				mNode = mNode->mNext;
		}

		ListIterator getNext() const {
			ASSERT(mNode != nullptr, "Node is null.");

			ListIterator it;

			if (mReverse)
				it.init(mNode->mPrev);
			else
				it.init(mNode->mNext);

			return it;
		}

		bool hasPrev() const {
			ASSERT(mNode != nullptr, "Node is null.");

			if (mReverse)
				return (mNode->mNext != nullptr);
			else
				return (mNode->mPrev != nullptr);
		}

		void prev() {
			ASSERT(mNode != nullptr, "Node is null.");

			if (mReverse)
				mNode = mNode->mNext;
			else
				mNode = mNode->mPrev;
		}

		ListIterator getPrev() const {
			ASSERT(mNode != nullptr, "Node is null.");

			ListIterator it;

			if (mReverse)
				it.init(mNode->mNext);
			else
				it.init(mNode->mPrev);

			return it;
		}

		T& get() const {
			ASSERT(mNode != nullptr, "Node is null.");

			return mNode->mElement;
		}

		void set(T element) {
			mNode->mElement = element;
		}

		bool isReverse() const {
			return mReverse;
		}

		void setReverse(bool isReverse) {
			mReverse = isReverse;
		}

		bool operator==(const ListIterator &rhs) const {
			return this->mNode == rhs.mNode && this->mReverse == rhs.mReverse;
		}

		bool operator!=(const ListIterator &rhs) const {
			return !((*this) == rhs);
		}

		ListIterator& operator=(const ListIterator &rhs) {
			if (this == &rhs)
				return *this; // handle self assignment
			//assignment operator
			this->mNode = rhs.mNode;
			this->mReverse = rhs.mReverse;
			return (*this);
		}

		
	};

	
private:

	void allocate(u32 elementSize, u32 alignment) {
		BaseContainer::init(0, elementSize, alignment); // BaseContainer::mLength = 0
	}

	void _init(u32 elementSize) {
		List::allocate(elementSize, 1);
		resetCache();
	}

	mutable ListIterator mLastAccessedIt;
	mutable i32 mLastAccessedIndex;

	void resetCache() {
		mLastAccessedIndex = -1;
	}

	T& randomAccessOperator(u32 index) const {
		ASSERT(index >= 0 && index < BaseContainer::mLength, "Index out of bounds.");

		const i32 signedIndex = static_cast<i32>(index);

		// first time, init the index.
		if (mLastAccessedIndex == -1) {
			mLastAccessedIt = List::getIterator();
			mLastAccessedIndex = 0;
		}

		// if i == last, just return last accessed index

		if (signedIndex == mLastAccessedIndex + 1) { // if i == last+1
			if (mLastAccessedIt.hasNext()) {
				mLastAccessedIt.next();
				mLastAccessedIndex++;
			}
		} else if (signedIndex == mLastAccessedIndex - 1) { // if i == last-1
			if (mLastAccessedIt.hasPrev()) {
				mLastAccessedIt.prev();
				mLastAccessedIndex--;
			}
		} else if (signedIndex != mLastAccessedIndex) { // if i != last
			mLastAccessedIndex = 0;
			mLastAccessedIt = List::getIterator();

			for (; mLastAccessedIndex < signedIndex && mLastAccessedIt.hasNext(); mLastAccessedIt.next())
				mLastAccessedIndex++;
		}

		return mLastAccessedIt.get();
	}

	void checkPut(const SequentialContainer<T> &other, u32 destinyIndex, u32 sourceIndex, u32 length) override {
		ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
		ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");
		ASSERT(length <= other.getLength() - sourceIndex, "Not enough space to copy.");
	}

	
public:

	GENERATE_METADATA(NONE, List<T>)

	List() : 	SequentialContainer<T>() {
		mFirst = nullptr;
		mLast = nullptr;
		mLastAccessedIndex = -1;
	}

	virtual ~List() override {
		List::clear();
	}

	/*!
	\brief Copy Constructor.
	\param other Other List.
	*/
	void init(const List &other) {
		this->_init(other.mElementSize);

		if(!other.isEmpty()){
			ListIterator it = other.getIterator();
			for (; it.hasNext(); it.next())
				this->pushBack(it.get());

			this->pushBack(it.get());
		}
	}

	/*!
	\brief Constructor. Empty List.
	*/
	void init() {
		List::_init(sizeof(T));
	}

	void init(const T rawArray[], u32 length) override {
		List::init();
		const T* typedArray = reinterpret_cast<const T*>(rawArray);

		FOR_RANGE(i, 0, length)
			List::pushBack(typedArray[i]);
	}

	void init(const T rawArray[], u32 length, u32 alignment) override {
		List::init(rawArray, length);
	}

	void init(u32 length) override {
		List::init();
	}

	void init(u32 length, u32 alignment) override {
		List::init();
	}

	void fill(const T element) override {
		if (!List::isEmpty()) {
			ListIterator it = List::getIterator();
			for (; it.hasNext(); it.next())
				it.set(element);

			it.set(element);
		}
	}

	void put(const SequentialContainer<T> &other, u32 destinyIndex, u32 sourceIndex, u32 length) override {
		resetCache();
		this->checkPut(other, destinyIndex, sourceIndex, length);

		u32 i = 0;
		ListIterator it = List::getIterator();

		if (!List::isEmpty()) {
			for (; i < destinyIndex && it.hasNext(); it.next())
				++i;

			i = 0;

			// update nodes
			for (; i < length && it.hasNext(); it.next()) {
				it.set(other.get(i));
				++i;
			}
		}

		// create new nodes
		for (; i < length; ++i) {
			List::pushBack(other.get(i));
		}
	}

	ListIterator getIterator() const {
		return List::getFirst();
	}

	ListIterator getRevIterator() const {
		ListIterator it;
		it.init(this->mLast);
		it.setReverse(true);
		return it;
	}

	ListIterator getFirst() const {
		ListIterator it;
		it.init(this->mFirst);
		return it;
	}

	ListIterator getLast() const {
		ListIterator it;
		it.init(this->mLast);
		return it;
	}

	bool isEmpty() const {
		return BaseContainer::mLength == 0;
	}

	void clear() override {
		if (!List::isEmpty()) {
			FOR_LIST(it, this) {
				List::remove(it);
			}
		}

		mFirst = nullptr;
		mLast = nullptr;

	}

	void pushFront(T element) {
		resetCache();
		List<T>::Node* node = newNode();
		node->init(element);

		if (!List::isEmpty()) {
			mFirst->mPrev = node;
			node->mNext = mFirst;
		} else
			mLast = node; // if is empty then also sets mLast

		mFirst = node;

		BaseContainer::mLength++;

	}

	void pushBack(T element) {
		resetCache();
		List<T>::Node* node = newNode();
		node->init(element);

		if (!List::isEmpty()) {
			mLast->mNext = node;
			node->mPrev = mLast;
		} else
			mFirst = node; // if is empty then also sets mFirst

		mLast = node;

		BaseContainer::mLength++;

	}

	T popFront() {
		resetCache();
		T* element = nullptr;

		if (!List::isEmpty()) {
			BaseContainer::mLength--;

			element = &mFirst->mElement;
			List<T>::Node* old = mFirst;

			if (!List::isEmpty()) {
				mFirst = mFirst->mNext;
				mFirst->mPrev = nullptr;
			} else { // if len = 0
				mFirst = nullptr;
				mLast = nullptr;
			}

			List::freeNode(old);
		}

		return *element;
	}

	T popBack() {
		resetCache();
		T* element = nullptr;

		if (!List::isEmpty()) {
			BaseContainer::mLength--;

			element = &mLast->mElement;
			List<T>::Node* old = mLast;

			if (!List::isEmpty()) {
				mLast = mLast->mPrev;
				mLast->mNext = nullptr;
			} else { // if len = 0
				mFirst = nullptr;
				mLast = nullptr;
			}

			List::freeNode(old);
		}

		return *element;
	}

	T get(u32 index) const {
		return List::randomAccessOperator(index);
	}

	void set(u32 index, const T element) {
		ASSERT(index >= 0 && index < BaseContainer::mLength, "Index out of bounds.");

		u32 i = 0;
		ListIterator it = List::getIterator();

		for (; i < index && it.hasNext(); it.next())
			++i;

		it.set(element);
	}

	void swap(u32 index1, u32 index2) {
		u32 i = 0;
		ListIterator it1 = List::getIterator();
		ListIterator it2 = List::getIterator();

		for (; i < index1 && it1.hasNext(); it1.next())
			++i;

		T element1 = it1.get();

		i = 0;

		for (; i < index2 && it2.hasNext(); it2.next())
			++i;

		T element2 = it2.get();

		it1.set(element2);
		it2.set(element1);
	}

	void removeAt(u32 index) {
		resetCache();
		ASSERT(index >= 0 && index < BaseContainer::mLength, "Index out of bounds.");

		if (!List::isEmpty()) {

			u32 i = 0;
			ListIterator it = List::getIterator();

			for (; i < index && it.hasNext(); it.next())
				++i;

			List::remove(it);
		}

	}

	void remove(ListIterator it) {
		resetCache();
		if (!it.isNull() && !List::isEmpty()) {
			BaseContainer::mLength--;

			List<T>::Node* prev = it.mNode->mPrev;
			List<T>::Node* next = it.mNode->mNext;

			// check First and Last
			if (it.mNode == mFirst)
				mFirst = next;

			if (it.mNode == mLast)
				mLast = prev;

			if (prev != nullptr)
				prev->mNext = next;

			if (next != nullptr)
				next->mPrev = prev;

			List::freeNode(it.mNode);
		}

		if (List::isEmpty()) {
			mFirst = nullptr;
			mLast = nullptr;
		}

	}

	void remove(const T element) {
		remove(find(element));
	}

	void insert(u32 index, T element) {
		resetCache();
		ASSERT(index >= 0 && index < BaseContainer::mLength, "Index out of bounds.");

		// BaseContainer::mLength++;

		// TODO refactor loop
		u32 i = 0;
		ListIterator it = List::getIterator();

		for (; i < index && it.hasNext(); it.next())
			++i;

		List::insert(it, element);
	}

	void insert(ListIterator &it, T element) {
		resetCache();
		if (it.mNode != mFirst) {
			List<T>::Node* node = newNode();
			node->init(element);

			// this function inserts new node before the current node (it.mNode)
			it.mNode->mPrev->mNext = node;

			node->mPrev = it.mNode->mPrev;
			node->mNext = it.mNode;
			it.mNode->mPrev = node;

			BaseContainer::mLength++;
		} else
			List::pushFront(element);
	}

	/*!
	\brief Finds with default comparator.
	*/
	ListIterator find(const T element) const {
		//return find(element, defaultComparator);

		ListIterator selectedIt;
		bool found = false;

		FOR_LIST_COND(it, this, !found)
		{
			if (element == it.get()) {
				found = true;
				selectedIt = it;
			}
		}

		return selectedIt;
	}

	/*!
	\brief Finds with custom comparator.
	*/
	ListIterator find(const T element, u8 (*comparator)(const T &a, const T &b)) const {
		ListIterator selectedIt;
		bool found = false;

		FOR_LIST_COND(it, this, !found)
		{
			if (comparator(element, it.get()) == 0) {
				found = true;
				selectedIt = it;
			}
		}

		return selectedIt;
	}

	/*!
	\brief Sorts with default comparator.
	*/
	void sort() {
		sort(defaultComparator);
	}

	/*!
	\brief Sorts custom comparator.
	\param comparator Function with this form: u8 comparator(const T& a, const T& b).
	*/
	void sort(u8 (*comparator)(T a, T b)) {
		qsort(comparator, 0, this->getLength() - 1);
	}

};
}

