#ifndef RBTREE_INCLUDES_RBTREE_IMPL
#define RBTREE_INCLUDES_RBTREE_IMPL

#include <cassert>
#include "rbtree.hpp"

namespace Trees {

	template<typename T>
	node<T>::node(int col) : color(col), num_of_less(0), num_of_greater(0), left(nullptr), right(nullptr), parent(nullptr), data(nullptr) {}

	template<typename T>
	node<T>::node(const node& another) : left(nullptr), right(nullptr), parent(nullptr) {
		color = another.color;
		num_of_less = another.num_of_less;
		num_of_greater = another.num_of_greater;
		data = new T;
		*data = *another.data;
	}

	template<typename T>
	node<T>::~node() {
		delete left;
		delete right;
		delete parent;
		delete[] data;
	}

	template<typename T>
	node<T>& node<T>::operator=(const node& another) {
		if (this == &another) {
			return *this;
		}
		color = another.color;
		num_of_less = another.num_of_less;			//left, right, parent pointers aren't changed
		num_of_greater = another.num_of_greater;    //it is made for implementation purposes
		data = new T;
		*data = *another.data;
		return *this;
	}

	template<typename T>
	RBTree<T>::RBTree() : nil_(new node<T>), head_(nil_) {}

	template<typename T>
	RBTree<T>::RBTree(const RBTree& another) {
		if (another.head_ == nullptr) {
			return nullptr;
		}
		else if (another.head_ == another.nil_) {
			return RBTree();
		}
		nil_ = new node<T>;
		head_ = new node<T>;
		head_->left = nil_;
		head_->right = nil_;
		*head_ = *another.head_;
		node<T>* cur1 = head_;
		node<T>* cur2 = another.head_;
		while (true) {
			while (cur2->left != nil_ && cur1->left == nil_) {
				cur2 = cur2->left;
				assert(cur1->left != nullptr);
				cur1->left = new node<T>;
				*cur1->left = *cur2;
				cur1->left->parent = cur1;
				cur1->left->left = nil_;
				cur1->left->right = nil_;
				cur1 = cur1->left;
			}
			while (cur2->right != nil_ && cur1->right == nil_) {
				cur2 = cur2->right;
				assert(cur1->right != nullptr);
				cur1->right = new node<T>;
				*cur1->right = *cur2;
				cur1->right->parent = cur1;
				cur1->right->left = nil_;
				cur1->right->right = nil_;
				cur1 = cur1->right;
			}
			if (cur2 != another.head_) {
				cur1 = cur1->parent;
				cur2 = cur2->parent;
			}
			else {
				if ((cur2->left == another.nil_ && cur1->left == nil_ && cur2->right == another.nil_ && cur1->right == nil_) ||
					(cur2->left == another.nil_ && cur1->left == nil_ && cur2->right != another.nil_ && cur1->right != nil_) ||
					(cur2->left != another.nil_ && cur1->left != nil_ && cur2->right == another.nil_ && cur1->right == nil_) ||
					(cur2->left != another.nil_ && cur1->left != nil_ && cur2->right != another.nil_ && cur1->right != nil_)) {
					break;
				}
			}
		}
	}

	template<typename T>
	RBTree<T>::~RBTree() {
		node<T>* cur = head_;
		node<T>* tmp;
		while (head_ != nullptr && head_ != nil_) {
			while (cur->left != nullptr && cur->left != nil_) {
				cur = cur->left;
			}
			while (cur->right != nullptr && cur->right != nil_) {
				cur = cur->right;
			}
			if (cur != head_) {
				tmp = cur;
				cur = cur->parent;
				delete tmp;
			}
			else if ((cur->left == nullptr || cur->left == nil_) && (cur->right != nullptr && cur->right != nil_)) {
				delete head_;
			}
		}
		delete nil_;
	}



}
#endif // RBTREE_INCLUDES_RBTREE_IMPL