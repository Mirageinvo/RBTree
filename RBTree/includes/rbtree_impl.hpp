#ifndef RBTREE_INCLUDES_RBTREE_IMPL
#define RBTREE_INCLUDES_RBTREE_IMPL

#include <cassert>
#include <utility>
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
		delete data;
	}

	template<typename T>
	RBTree<T>::RBTree() : nil_(new node<T>), head_(nil_) {}

	template<typename T>
	RBTree<T>::RBTree(const RBTree& another) {
		if (another.head_ == nullptr) {
			head_ = nil_ = nullptr;
		}
		else if (another.head_ == another.nil_) {
			nil_ = new node<T>;
			head_ = nil_;
		}
		else {
			nil_ = new node<T>;
			head_ = new node<T>;
			*head_ = *another.head_;//////////= doesn't work
			head_->data = new T;
			*head_->data = *another.head_->data;
			head_->left = nil_;
			head_->right = nil_;//head_->parent = nullptr;
			node<T>* cur1 = head_;
			node<T>* cur2 = another.head_;
			while (true) {
				while (cur2->left != nil_ && cur1->left == nil_) {
					cur2 = cur2->left;
					assert(cur1->left != nullptr);
					cur1->left = new node<T>;
					*cur1->left = *cur2;
					cur1->left->data = new T;
					*cur1->left->data = *cur2->data;
					cur1->left->parent = cur1;
					cur1->left->left = nil_;
					cur1->left->right = nil_;
					cur1 = cur1->left;
				}
				while (cur2->right != nil_ && cur1->right == nil_) {
					cur2 = cur2->right;
					assert(cur1->right != nullptr);
					cur1->right = new node<T>;
					*cur1->right = *cur2;///////////////////= doesn't work?????????????
					cur1->right->data = new T;
					*cur1->right->data = *cur2->data;
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
	}

	template<typename T>
	RBTree<T>::RBTree(RBTree&& another) {
		head_ = another.head_;
		nil_ = another.nil_;
		another.head_ = nullptr;
		another.nil_ = nullptr;
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
				if (tmp == cur->right) {
					cur->right = nullptr;
				}
				else {
					cur->left = nullptr;
				}
			}
			else if ((cur->left == nullptr || cur->left == nil_) && (cur->right != nullptr || cur->right != nil_)) {
				delete head_;
				head_ = nullptr;
			}
		}
		delete nil_;
		//delete head_;
	}

	template<typename T>
	RBTree<T>& RBTree<T>::operator=(RBTree&& another) {
		if (this == &another) {
			return *this;
		}
		delete this;
		this->head_ = another.head_;
		this->nil_ = another.nil_;
		another.head_ = nullptr;
		another.nil_ = nullptr;
		return *this;
	}

	template<typename T>
	RBTree<T>& RBTree<T>::operator=(const RBTree& another) {
		if (this == &another) {
			return *this;
		}
		RBTree<T> tmp(another);
		*this = std::move(another);
		return *this;
	}

	template<typename T>
	node<T>* RBTree<T>::grandfather(node<T>* init) {
		return (init->parent != nullptr && init->parent->parent != nullptr) ? init->parent->parent : nullptr;
	}

	template<typename T>
	node<T>* RBTree<T>::uncle(node<T>* init) {
		node<T>* grdf = grandfather(init);
		if (grdf == nullptr) {
			return nullptr;
		}
		if (init->parent == grdf->left) {
			return grdf->right;
		}
		else {
			return grdf->left;
		}
	}

	template<typename T>
	void RBTree<T>::rotate_left(node<T>* init) {
		assert(init != head_); // && init == init->parent->right
		if (init->parent == head_) {
			init->parent->right = init->left;
			init->parent->parent = init;
			init->left = init->parent;
			init->parent = nullptr;
			head_ = init;
		}
		else {
			node<T>* grnd = grandfather(init);
			init->parent->right = init->left;
			init->parent->parent = init;
			init->left = init->parent;
			init->parent = grnd;
		}
		init->left->num_of_greater = init->num_of_less;
		init->num_of_less = init->left->num_of_greater + init->left->num_of_less + 1;
	}

	template<typename T>
	void RBTree<T>::rotate_right(node<T>* init) {
		assert(init != head_); // && init == init->parent->left
		if (init->parent == head_) {
			init->parent->left = init->right;
			init->parent->parent = init;
			init->right = init->parent;
			init->parent = nullptr;
			head_ = init;
		}
		else {
			node<T>* grnd = grandfather(init);
			init->parent->left = init->right;
			init->parent->parent = init;
			init->right = init->parent;
			init->parent = grnd;
		}
		init->right->num_of_less = init->num_of_greater;
		init->num_of_greater = init->right->num_of_greater + init->right->num_of_less + 1;
	}

	template<typename T>
	bool RBTree<T>::fix_tree(node<T>* init) {
		if (init == head_) {
			init->color = BLACK;
			return true;
		}
		else if (init->parent == head_) {
			return true;//////////////???????????????????????? mb ok
		}
		else {
			if (init->parent->color == BLACK) {
				return true;
			}
			else { //if color of parent is red, it means that parent is not a root
				node<T>* unc = uncle(init);
				if (unc->color == RED) {
					unc->color = BLACK;
					init->parent->color = BLACK;
					init->parent->parent->color = RED;
					return false;
				}
				else {
					if (init->parent == init->parent->parent->left) {
						if (init == init->parent->right) {
							rotate_left(init);
						}
						else {
							init = init->parent;
						}
						assert(init->left->color == RED && init->parent->color == BLACK);
						rotate_right(init);
						init->color = BLACK;
						init->right->color = RED;
					}
					else {
						if (init == init->parent->left) {
							rotate_right(init);
						}
						else {
							init = init->parent;
						}
						assert(init->right->color == RED && init->parent->color == BLACK);//fail
						rotate_left(init);
						init->color = BLACK;
						init->left->color = RED;
					}
					return true;
				}
			}
		}
	}

	template<typename T>
	void RBTree<T>::insert(T el) {
		if (head_ == nil_) {
			head_ = new node<T>;
			head_->data = new T;
			*head_->data = el;
			head_->left = nil_;
			head_->right = nil_;
			assert(head_->color == BLACK && head_->num_of_less == 0 && head_->num_of_greater == 0 && head_->parent == nullptr);
			return;
		}
		node<T>* tmp = head_;
		while (true) {
			assert(el != *tmp->data);
			if (el > *tmp->data) {
				tmp->num_of_greater++;
				if (tmp->right == nil_) {
					tmp->right = new node<T>;
					tmp->right->data = new T;
					*tmp->right->data = el;
					tmp->right->left = nil_;
					tmp->right->right = nil_;
					tmp->right->parent = tmp;
					tmp->right->color = RED;
					tmp = tmp->right;
					while (!fix_tree(tmp)) {
						tmp = grandfather(tmp);
					}
					return;
				}
				else {
					tmp = tmp->right;
					continue;
				}
			}
			else {
				tmp->num_of_less++;
				if (tmp->left == nil_) {
					tmp->left = new node<T>;
					tmp->left->data = new T;
					*tmp->left->data = el;
					tmp->left->left = nil_;
					tmp->left->right = nil_;
					tmp->left->parent = tmp;
					tmp->left->color = RED;
					tmp = tmp->left;
					while (!fix_tree(tmp)) {
						tmp = grandfather(tmp);
					}
					return;
				}
				else {
					tmp = tmp->left;
				}
			}
		}
	}

}
#endif // RBTREE_INCLUDES_RBTREE_IMPL