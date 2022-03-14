#ifndef RBTREE_INCLUDES_RBTREE_IMPL
#define RBTREE_INCLUDES_RBTREE_IMPL

#include <cassert>
#include <utility>
#include "rbtree.hpp"

namespace Trees {

	template<typename T>
	node<T>::node(int col) : color(col), num_of_less(0), num_of_greater(0), left(nullptr), right(nullptr), parent(nullptr), data(nullptr) {}

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
			bool go_again = false;
			nil_ = new node<T>;
			head_ = new node<T>;
			*head_ = *another.head_;
			head_->data = new T;
			*head_->data = *another.head_->data;
			head_->left = nil_;
			head_->right = nil_;
			node<T>* cur1 = head_;
			node<T>* cur2 = another.head_;
			while (true) {
				while (cur2->left != another.nil_ && cur1->left == nil_) {
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
				while (cur2->right != another.nil_ && cur1->right == nil_) {
					cur2 = cur2->right;
					assert(cur1->right != nullptr);
					cur1->right = new node<T>;
					*cur1->right = *cur2;
					cur1->right->data = new T;
					*cur1->right->data = *cur2->data;
					cur1->right->parent = cur1;
					cur1->right->left = nil_;
					cur1->right->right = nil_;
					cur1 = cur1->right;
					go_again = true;
					break;
				}
				if (go_again) {
					go_again = false;
					continue;
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
		bool go_again = false;
		while (head_ != nullptr && head_ != nil_) {
			while (cur->left != nullptr && cur->left != nil_) {
				cur = cur->left;
			}
			while (cur->right != nullptr && cur->right != nil_) {
				cur = cur->right;
				go_again = true;
				break;
			}
			if (go_again) {
				go_again = false;
				continue;
			}
			if (cur != head_) {
				tmp = cur;
				cur = cur->parent;
				if (tmp == cur->right) {
					delete tmp;
					cur->right = nullptr;
				}
				else {
					delete tmp;
					cur->left = nullptr;
				}
			}
			else if ((cur->left == nullptr || cur->left == nil_) && (cur->right != nullptr || cur->right != nil_)) {
				delete head_;
				head_ = nullptr;
			}
		}
		delete nil_;
	}

	template<typename T>
	RBTree<T>& RBTree<T>::operator=(RBTree&& another) {
		if (this == &another) {
			return *this;
		}
		std::swap(this->head_, another.head_);
		std::swap(this->nil_, another.nil_);
		return *this;
	}

	template<typename T>
	RBTree<T>& RBTree<T>::operator=(const RBTree& another) {
		if (this == &another) {
			return *this;
		}
		RBTree<T> tmp(another);
		*this = std::move(tmp);
		return *this;
	}

	template<typename T>
	node<T>* RBTree<T>::grandfather(node<T>* init) const {
		return (init->parent != nullptr && init->parent->parent != nullptr) ? init->parent->parent : nullptr;
	}

	template<typename T>
	node<T>* RBTree<T>::uncle(node<T>* init) const {
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
		assert(init != head_);
		if (init->parent == head_) {
			init->parent->right = init->left;
			init->parent->parent = init;
			init->left->parent = init->parent;
			init->left = init->parent;
			init->parent = nullptr;
			head_ = init;
		}
		else {
			node<T>* grnd = grandfather(init);
			if (init->parent == grnd->right) {
				grnd->right = init;
			}
			else {
				grnd->left = init;
			}
			init->parent->right = init->left;
			init->parent->parent = init;
			init->left->parent = init->parent;
			init->left = init->parent;
			init->parent = grnd;
		}
		init->left->num_of_greater = init->num_of_less;
		init->num_of_less = init->left->num_of_greater + init->left->num_of_less + 1;
	}

	template<typename T>
	void RBTree<T>::rotate_right(node<T>* init) {
		assert(init != head_);
		if (init->parent == head_) {
			init->parent->left = init->right;
			init->parent->parent = init;
			init->right->parent = init->parent;
			init->right = init->parent;
			init->parent = nullptr;
			head_ = init;
		}
		else {
			node<T>* grnd = grandfather(init);
			if (init->parent == grnd->right) {
				grnd->right = init;
			}
			else {
				grnd->left = init;
			}
			init->parent->left = init->right;
			init->parent->parent = init;
			init->right->parent = init->parent;
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
			return true;
		}
		else {
			if (init->parent->color == BLACK) {
				return true;
			}
			else {
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
						assert(init->right->color == RED && init->parent->color == BLACK);
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

	template<typename T>
	size_t RBTree<T>::num_of_less(T el) const {
		if (head_ == nil_) {
			return 0;
		}
		size_t ans = 0;
		node<T>* cur = head_;
		while (true) {
			if (*cur->data == el) {
				ans += cur->num_of_less;
				break;
			}
			else if (*cur->data < el) {
				ans += cur->num_of_less + 1;
				if (cur->right != nil_) {
					cur = cur->right;
				}
				else {
					break;
				}
			}
			else {
				if (cur->left != nil_) {
					cur = cur->left;
				}
				else {
					break;
				}
			}
		}
		return ans;
	}

	template<typename T>
	T RBTree<T>::mth_statistic(size_t stat) const {
		if (head_->num_of_less + head_->num_of_greater + 1 < stat) {
			return 0;
		}
		size_t actual_value = stat;
		node<T>* cur = head_;
		while (true) {
			if (actual_value == cur->num_of_less + 1) {
				return *cur->data;
			}
			else if (actual_value < cur->num_of_less + 1) {
				assert(cur->left != nil_ && cur->left != nullptr);
				cur = cur->left;
			}
			else {
				assert(cur->right != nil_ && cur->right != nullptr);
				actual_value -= cur->num_of_less + 1;
				cur = cur->right;
			}
		}
	}

}
#endif // RBTREE_INCLUDES_RBTREE_IMPL