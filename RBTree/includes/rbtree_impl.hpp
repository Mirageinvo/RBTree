#ifndef RBTREE_INCLUDES_RBTREE_IMPL
#define RBTREE_INCLUDES_RBTREE_IMPL

#include "rbtree.hpp"

namespace Trees {

	template<typename T>
	node<T>::node(int col) : color(col), num_of_less(0), num_of_greater(0), left(nullptr), right(nullptr), parent(nullptr), data(nullptr) {}

	template<typename T>
	node<T>::~node() {
		delete left;
		delete right;
		delete parent;
		delete[] data;
	}

	template<typename T>
	RBTree<T>::RBTree() : nil_(new node<T>), head_(nil_) {}

	template<typename T>
	RBTree<T>::~RBTree() {
		node<T> cur = head_;
		while (head_ != nullptr && head_ != nil_) {
			while (cur->left != nullptr && cur->left != nil_) {
				cur = cur->left;
			}
			while (cur->right != nullptr && cur->right != nil_) {
				cur = cur->right;
			}
			if (cur != head_) {
				cur = cur->parent;
				
			}

		}
	}

}

#endif // RBTREE_INCLUDES_RBTREE_IMPL