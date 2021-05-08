#pragma once

#include "Containers/Array.hpp"
#include "Memory/Memory.hpp"

namespace DE {

/*!
 \brief Binary Tree.
 \tparam K Key class.
 \tparam T Talue class.
 */
template<class T>
class Tree : public BaseContainer {

private:

	class Node : public ObjectBase {

	public:
PRIVATE(Parent, NONE, Node*)
		T mElement;
		Array<Node*>* mChildren;

		GENERATE_METADATA(CONSTRUCTOR, Node)

		Node() : ObjectBase() {
			mChildren = nullptr;
		};

		virtual ~Node() override {
			mParent = nullptr;
			Memory::free(mChildren);
		};

		void init(Node *parent, const T element, u32 childrenCount) {
			mParent = parent;
			mElement = element;
			mChildren = Memory::allocate<Array<Node*>>();
			mChildren->init(childrenCount);
			mChildren->set(0, nullptr);
			mChildren->set(1, nullptr);
		};

		void addChild(Node *node) {
			u32 index = Hash::hash(node->mElement) < Hash::hash(mElement) ? 0 : 1;

			Node* child = mChildren->get(index);

			if (child == nullptr) {
				node->mParent = this;
				mChildren->set(index, node);
			} else
				child->addChild(node);
		};

		void removeChild(Node *node) {
			u32 index = Hash::hash(node->mElement) < Hash::hash(mElement) ? 0 : 1;

			mChildren->get(index)->mParent = nullptr;
			mChildren->set(index, nullptr);
		}
	};

	static const u32 smNodeSize = sizeof(Node);

	Node* newNode(Node *parent, const T element) {
		Node* node = Memory::allocate<Node>();
		node->init(parent, element, smChildrenCount);
		return node;
	};

	void freeNode(Node *node) {
		Memory::free(node);
	};

	void freeSubTree(Node *node) {
		Node* child0 = node->mChildren->get(0);
		Node* child1 = node->mChildren->get(1);

		if (child0 != nullptr)
			freeSubTree(child0);

		if (child1 != nullptr)
			freeSubTree(child1);

		node->mChildren->set(0, nullptr);
		node->mChildren->set(1, nullptr);

		mLength--;
	};

	Node* find(const T element, Node *node) {
		if (element == node->mElement)
			return node;

		u32 index = Hash::hash(element) < Hash::hash(node->mElement) ? 0 : 1;

		Node* child = node->mChildren->get(index);

		if (child == nullptr)
			return nullptr;
		else
			return find(element, child);
	};

PRIVATE(Root, NONE, Node*)
	static const u32 smChildrenCount = 2;

public:

	GENERATE_METADATA(NONE, Tree<T>)

	Tree() : BaseContainer() {
		mRoot = nullptr;
	}

	virtual ~Tree() override {
		Tree<T>::clear();
	}

	/*!
	\brief Constructor.
	*/
	void init() {
		BaseContainer::init(0, sizeof(T), 1);
		mRoot = nullptr;
	}

	void add(const T element) {
		Node* node = newNode(nullptr, element);

		if (mRoot == nullptr)
			mRoot = node;
		else
			mRoot->addChild(node);

		mLength++;
	}

	void remove(const T element) {
		Node* node = find(element, mRoot);

		if (node != nullptr) {

			Node* child0 = node->mChildren->get(0);
			Node* child1 = node->mChildren->get(1);

			// 3 cases: a, b, c.

			if (child0 == nullptr && child1 == nullptr) { // a) If no children
				Node* parent = node->mParent;
				node->mParent = nullptr;

				if (parent != nullptr)
					parent->removeChild(node);
				else
					// Is the root!
					mRoot = nullptr;

				freeNode(node);

			} else if (child0 != nullptr && child1 != nullptr) { // b) If 2 children

			// Find min value in right subtree.

				Node* successor = nullptr; // successor
				Node* successorParent = node->mChildren->get(1); // root of right subtree.
				while (successorParent->mChildren->getLength() > 0 && successorParent->mChildren->get(0) != nullptr) {
					successor = successorParent->mChildren->get(0); // explore left side (where values are always min).
					successorParent = successor;
				}

				if (successor != nullptr) // if successor prune the child.
					successor->mParent->removeChild(successor);
				else
					// if no successor then choose the root of the right subtree.
					successor = successorParent;

				node->mElement = successor->mElement;

				freeNode(successor); // delete successor node

			} else { // c) If only 1 child
				Node* child = child0 != nullptr ? child0 : child1;
				Node* parent = node->mParent;
				node->mParent = nullptr;

				if (parent != nullptr) {
					parent->removeChild(node);
					parent->addChild(child);
				} else { // Is the root!
					child->mParent = nullptr;
					mRoot = child;
				}

				freeNode(node);
			}

			mLength--;

			if (mLength == 0) {
				Tree::clear();
			}
		}
	}

	void clear() override {
		if (mLength > 0) {
			freeSubTree(mRoot);
			freeNode(mRoot);
		}

		mRoot = nullptr;
	}

};
}

