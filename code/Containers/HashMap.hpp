#ifndef DE_HASHMAP_H
#define DE_HASHMAP_H

#include "List.hpp"
#include "Array.hpp"
#include "Memory.hpp"
#include <functional>

namespace DE {

// ---------------------------------------------------------------------------

/*!
 \brief HashMap of elements.
 \tparam K Key class.
 \tparam V Value class.
 */
template<class K, class V>
class HashMap: public BaseContainer {

private:

	// ---------------------------------------------------------------------------

	class Node: public DE_Class {

	public:

		K mKey;
		V mElement;

		DE_CLASS_TEMPLATE2(Node, DE_Class, K, V);

		Node() : DE_Class() {
		};

		virtual ~Node() override {
		};

		void init(const K key, const V element) {
			mKey = key;
			mElement = element;
		}
	};

	// ---------------------------------------------------------------------------

	static const u32 smNodeSize = sizeof(HashMap<K,V>::Node);
	using NodeClass = HashMap<K,V>::Node;

	HashMap<K,V>::Node* newNode(const K key, const V element) {
		HashMap<K,V>::Node* node = Memory::allocate<Node>();
		node->init(key, element);
		return node;
	}

	void freeNode(HashMap<K,V>::Node* node) {
		Memory::free<HashMap<K,V>::Node>(node);
	}

	// ---------------------------------------------------------------------------

	Array<List<HashMap<K,V>::Node*>*>* mArray;

	List<K>* mKeys;
	List<V>* mValues;

public:

	using HashMapKV = HashMap<K,V>;

	DE_CLASS_TEMPLATE2(HashMapKV, BaseContainer, K, V);

	/*!
	 \brief Default Constructor.
	 */
	HashMap() : 	BaseContainer() {
		mArray = nullptr;
		mKeys = nullptr;
		mValues = nullptr;

		// check class
		bool class_ok = std::is_base_of<Hash, K>::value || std::is_same<K, std::string>::value
				|| std::is_arithmetic<K>::value || std::is_pointer<K>::value;
		DE_ASSERT(class_ok, "K must be integer, std::string, pointer or extend Hash class.");
	}

	/*!
	 \brief Destructor.
	 */
	virtual ~HashMap() override {
		HashMap<K, V>::clear();

		Memory::free<Array<List<HashMap<K,V>::Node*>*>>(mArray);

		Memory::free<List<K>>(mKeys);
		Memory::free<List<V>>(mValues);
	}

	// ---------------------------------------------------------------------------

	/*!
	 \brief Constructor.
	 */
	void init() {
		BaseContainer::init(0, sizeof(V), 1);
		mArray = Memory::allocate<Array<List<HashMap<K,V>::Node*>*>>();
		mArray->init(20); // TODO : find a good number.

		mKeys = Memory::allocate<List<K>>();
		mKeys->init();

		mValues = Memory::allocate<List<V>>();
		mValues->init();
	}

	// ---------------------------------------------------------------------------

	void set(K key, V element) {

		u64 hashIndex = Hash::hash<K>(key) % mArray->BaseContainer::getLength();

		List<HashMap<K,V>::Node*>* list = mArray->get(hashIndex);

		// if there is no list, create it
		if (list == nullptr) {
			list = Memory::allocate<List<HashMap<K,V>::Node*>>();
			list->init();
			mArray->set(hashIndex, list);
		}

		bool found = false;
		HashMap<K,V>::Node* node = nullptr;

		FOR_LIST_COND(it, list, !found)
		{
			if (it.get()->mKey == key) {
				found = true;
				node = it.get();
			}
		}

		// if element exists, update element
		if (found) {
			mValues->remove(mValues->find(node->mElement)); // remove outdated element
			node->mElement = element; // update the elemetn
			mValues->pushBack(element); // add updated element
		} else {
			list->pushBack(newNode(key, element));
			BaseContainer::mLength++;

			mKeys->pushBack(key);
			mValues->pushBack(element);
		}

	}

	// ---------------------------------------------------------------------------

	V get(K key) const {

		u64 hashIndex = Hash::hash<K>(key) % mArray->BaseContainer::getLength();

		// iterate over list to find element.

		auto list = mArray->get(hashIndex);

		bool found = false;
		V element;

		if (list) {

			FOR_LIST_COND(it, list, !found) {
				if (it.get()->mKey == key) {
					found = true;
					element = it.get()->mElement;
				}
			}
		}

		if (found) {
			return element;
		} else {
			DE_ASSERT(false, "Can't find the element with given key.");
		}
	}

	// ---------------------------------------------------------------------------

	bool contains(const K key) {
		u64 hashIndex = Hash::hash<K>(key) % mArray->BaseContainer::getLength();

		// iterate over list to find element.

		auto list = mArray->get(hashIndex);

		bool found = false;

		if (list) {

			FOR_LIST_COND(it, list, !found) {
				if (it.get()->mKey == key) {
					found = true;
				}
			}
		}

		return found;
	}

	// ---------------------------------------------------------------------------

	void remove(K key) {

		u64 hashIndex = Hash::hash<K>(key) % mArray->BaseContainer::getLength();

		auto list = mArray->get(hashIndex);

		bool found = false;
		u32 index = 0;
		typename List<HashMap<K,V>::Node*>::ListIterator selectedIt;

		if (list) {
			// iterate over list to find element.
			FOR_LIST_COND(it, list, !found) {
				if (it.get()->mKey == key) {
					found = true;
					selectedIt = it;
				}

				index++;
			}
		}

		if (found) {
			mValues->remove(selectedIt.get()->mElement);
			list->removeAt(index - 1);
			BaseContainer::mLength--;

			if (list->getLength() == 0) {
				mKeys->remove(mKeys->find(selectedIt.get()->mKey));
			}

			freeNode(selectedIt.get());

		} else
			DE_ASSERT(false, "Can't find the element with given key.");
	}

	// ---------------------------------------------------------------------------

	const List<K>* getKeys() const {
		return mKeys;
	};

	const List<V>* getValues() const {
		return mValues;
	};


	// ---------------------------------------------------------------------------

	void clear() override {

		if (mArray) {

			FOR_ARRAY(i, mArray) {

				List<HashMap<K,V>::Node*>* list = mArray->get(i);
				if (list) {

					FOR_LIST(it, list) {
						freeNode(it.get());
					}

					Memory::free<List<HashMap<K,V>::Node*>>(list);
				}

				mArray->set(i, nullptr);
			}

			mKeys->clear();
			mValues->clear();
		}
	}

	// ---------------------------------------------------------------------------

};

} /* namespace DE */

#endif /* DE_HASHMAP_H */