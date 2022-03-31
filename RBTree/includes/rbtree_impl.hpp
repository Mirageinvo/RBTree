#ifndef RBTREE_INCLUDES_RBTREE_IMPL
#define RBTREE_INCLUDES_RBTREE_IMPL

#include <cassert>
#include <utility>

#include "rbtree.hpp"

namespace trees {

template <typename T>
RBTree<T>::node::node(T el, node *nil, node *par, int col)
    : color(col), left(nil), right(nil), parent(par), data(nullptr) {
  data = new T;
  try {
    *data = el;
  } catch (...) {
    delete data;
    throw;
  }
}

template <typename T>
RBTree<T>::node::node(node *el, node *nil, node *par)
    : color(el->color),
      num_of_less(el->num_of_less),
      num_of_greater(el->num_of_greater),
      left(nil),
      right(nil),
      parent(par),
      data(nullptr) {
  assert(el);
  if (el && el->data) {
    data = new T;
    try {
      *data = *el->data;
    } catch (...) {
      delete data;
      throw;
    }
  }
}

template <typename T>
RBTree<T>::node::node(const node &another)
    : color(another.color),
      num_of_less(another.num_of_less),
      num_of_greater(another.num_of_greater),
      left(another.left),
      right(another.right),
      parent(another.parent),
      data(nullptr) {
  if (another.data) {
    data = new T;
    try {
      *data = *another.data;
    } catch (...) {
      delete data;
      throw;
    }
  }
}

template <typename T>
RBTree<T>::node::node(node &&another) noexcept
    : color(another.color),
      num_of_less(another.num_of_less),
      num_of_greater(another.num_of_greater),
      left(another.left),
      right(another.right),
      parent(another.parent),
      data(another.data) {
  another.left = nullptr;
  another.right = nullptr;
  another.parent = nullptr;
  another.data = nullptr;
}

template <typename T>
RBTree<T>::node::~node() {
  delete data;
}

template <typename T>
typename RBTree<T>::node &RBTree<T>::node::operator=(const node &another) {
  if (this == &another) {
    return *this;
  }
  if (another.data) {
    T *tmp = new T;
    try {
      *tmp = *another.data;
    } catch (...) {
      delete tmp;
      throw;
    }
    std::swap(data, tmp);
    delete tmp;
  } else {
    data = nullptr;
  }
  color = another.color;
  num_of_less = another.num_of_less;
  num_of_greater = another.num_of_greater;
  left = another.left;
  right = another.right;
  parent = another.parent;
  return *this;
}

template <typename T>
typename RBTree<T>::node &RBTree<T>::node::operator=(node &&another) noexcept {
  if (this == &another) {
    return *this;
  }
  color = another.color;
  num_of_less = another.num_of_less;
  num_of_greater = another.num_of_greater;
  std::swap(left, another.left);
  std::swap(right, another.right);
  std::swap(parent, another.parent);
  std::swap(data, another.data);
  return *this;
}

template <typename T>
RBTree<T>::RBTree() : nil_(new node) {
  head_ = nil_;
}

template <typename T>
RBTree<T>::RBTree(const RBTree &another) {
  head_ = nil_ = nullptr;
  try {
    if (another.head_ == nullptr) {
      head_ = nil_ = nullptr;
    } else if (another.head_ == another.nil_) {
      nil_ = new node;
      head_ = nil_;
    } else {
      bool go_again = false;
      nil_ = new node;
      head_ = new node(another.head_, nil_, nullptr);
      node *cur1 = head_;
      node *cur2 = another.head_;
      while (true) {
        move_pointers(cur1, cur2, go_again, another);
        if (go_again) {
          go_again = false;
          continue;
        }
        if (cur2 != another.head_) {
          cur1 = cur1->parent;
          cur2 = cur2->parent;
        } else if ((cur2->left == another.nil_ && cur1->left == nil_ &&
                    cur2->right == another.nil_ && cur1->right == nil_) ||
                   (cur2->left == another.nil_ && cur1->left == nil_ &&
                    cur2->right != another.nil_ && cur1->right != nil_) ||
                   (cur2->left != another.nil_ && cur1->left != nil_ &&
                    cur2->right == another.nil_ && cur1->right == nil_) ||
                   (cur2->left != another.nil_ && cur1->left != nil_ &&
                    cur2->right != another.nil_ && cur1->right != nil_)) {
          break;
        }
      }
    }
  } catch (...) {
    delete_tree();
    throw;
  }
}

template <typename T>
RBTree<T>::RBTree(RBTree &&another) noexcept {
  head_ = another.head_;
  nil_ = another.nil_;
  another.head_ = nullptr;
  another.nil_ = nullptr;
}

template <typename T>
RBTree<T>::~RBTree() {
  delete_tree();
}

template <typename T>
RBTree<T> &RBTree<T>::operator=(RBTree &&another) noexcept {
  if (this == &another) {
    return *this;
  }
  std::swap(head_, another.head_);
  std::swap(nil_, another.nil_);
  return *this;
}

template <typename T>
RBTree<T> &RBTree<T>::operator=(const RBTree &another) {
  if (this == &another) {
    return *this;
  }
  RBTree<T> tmp(another);
  *this = std::move(tmp);
  return *this;
}

template <typename T>
typename RBTree<T>::node *RBTree<T>::grandfather(node *init) const noexcept {
  assert(init);
  return (init->parent && init->parent->parent) ? init->parent->parent
                                                : nullptr;
}

template <typename T>
typename RBTree<T>::node *RBTree<T>::uncle(node *init) const noexcept {
  assert(init);
  node *grdf = grandfather(init);
  if (grdf == nullptr) {
    return nullptr;
  }
  if (init->parent == grdf->left) {
    return grdf->right;
  } else {
    return grdf->left;
  }
}

template <typename T>
void RBTree<T>::rotate_left(node *init) noexcept {
  assert(init && init != head_);
  if (init->parent == head_) {
    init->parent->right = init->left;
    init->parent->parent = init;
    init->left->parent = init->parent;
    init->left = init->parent;
    init->parent = nullptr;
    head_ = init;
  } else {
    node *grnd = grandfather(init);
    if (init->parent == grnd->right) {
      grnd->right = init;
    } else {
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

template <typename T>
void RBTree<T>::rotate_right(node *init) noexcept {
  assert(init && init != head_);
  if (init->parent == head_) {
    init->parent->left = init->right;
    init->parent->parent = init;
    init->right->parent = init->parent;
    init->right = init->parent;
    init->parent = nullptr;
    head_ = init;
  } else {
    node *grnd = grandfather(init);
    if (init->parent == grnd->right) {
      grnd->right = init;
    } else {
      grnd->left = init;
    }
    init->parent->left = init->right;
    init->parent->parent = init;
    init->right->parent = init->parent;
    init->right = init->parent;
    init->parent = grnd;
  }
  init->right->num_of_less = init->num_of_greater;
  init->num_of_greater =
      init->right->num_of_greater + init->right->num_of_less + 1;
}

template <typename T>
bool RBTree<T>::fix_tree(node *init) {
  assert(init);
  if (init == head_) {
    init->color = BLACK;
    return true;
  } else if (init->parent == head_) {
    return true;
  } else {
    if (init->parent->color == BLACK) {
      return true;
    } else {
      node *unc = uncle(init);
      if (unc->color == RED) {
        unc->color = BLACK;
        init->parent->color = BLACK;
        init->parent->parent->color = RED;
        return false;
      } else {
        if (init->parent == init->parent->parent->left) {
          if (init == init->parent->right) {
            rotate_left(init);
          } else {
            init = init->parent;
          }
          assert(init->left->color == RED && init->parent->color == BLACK);
          rotate_right(init);
          init->color = BLACK;
          init->right->color = RED;
        } else {
          if (init == init->parent->left) {
            rotate_right(init);
          } else {
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

template <typename T>
void RBTree<T>::insert(T el) {
  try {
    assert(head_);
    if (head_ == nil_) {
      node *tmp = new node(el, nil_);
      head_ = tmp;
      assert(head_->color == BLACK && head_->num_of_less == 0 &&
             head_->num_of_greater == 0 && head_->parent == nullptr);
      return;
    }
    node *tmp = head_;
    while (true) {
      assert(el != *tmp->data);
      if (el > *tmp->data) {
        tmp->num_of_greater++;
        if (tmp->right == nil_) {
          node *tmp2 = new node(el, nil_, tmp, RED);
          tmp->right = tmp2;
          tmp = tmp->right;
          while (!fix_tree(tmp)) {
            tmp = grandfather(tmp);
          }
          return;
        } else {
          tmp = tmp->right;
        }
      } else {
        tmp->num_of_less++;
        if (tmp->left == nil_) {
          node *tmp2 = new node(el, nil_, tmp, RED);
          tmp->left = tmp2;
          tmp = tmp->left;
          while (!fix_tree(tmp)) {
            tmp = grandfather(tmp);
          }
          return;
        } else {
          tmp = tmp->left;
        }
      }
    }
  } catch (...) {
    delete_tree();
    throw;
  }
}

template <typename T>
size_t RBTree<T>::num_of_less(T el) const {
  if (head_ == nullptr || head_ == nil_) {
    return 0;
  }
  size_t ans = 0;
  node *cur = head_;
  while (true) {
    if (*cur->data == el) {
      ans += cur->num_of_less;
      break;
    } else if (*cur->data < el) {
      ans += cur->num_of_less + 1;
      if (cur->right != nil_) {
        cur = cur->right;
      } else {
        break;
      }
    } else {
      if (cur->left != nil_) {
        cur = cur->left;
      } else {
        break;
      }
    }
  }
  return ans;
}

template <typename T>
T RBTree<T>::mth_statistic(size_t stat) const {
  assert(head_ && head_->num_of_less + head_->num_of_greater + 1 >= stat);
  if (head_ == nullptr) {
    throw std::runtime_error("The tree does not exist");
  }
  if (head_ == nil_ || head_->num_of_less + head_->num_of_greater + 1 < stat) {
    throw std::runtime_error("The value is out of range");
  }
  size_t actual_value = stat;
  node *cur = head_;
  while (true) {
    if (actual_value == cur->num_of_less + 1) {
      return *cur->data;
    } else if (actual_value < cur->num_of_less + 1) {
      assert(cur->left != nil_ && cur->left);
      cur = cur->left;
    } else {
      assert(cur->right != nil_ && cur->right);
      actual_value -= cur->num_of_less + 1;
      cur = cur->right;
    }
  }
}

template <typename T>
void RBTree<T>::move_pointers(node *&cur1, node *&cur2, bool &go_again,
                              const RBTree &another) {
  while (cur2->left != another.nil_ && cur1->left == nil_) {
    cur2 = cur2->left;
    assert(cur1->left);
    node *tmp = new node(cur2, nil_, cur1);
    cur1->left = tmp;
    cur1 = cur1->left;
  }
  while (cur2->right != another.nil_ && cur1->right == nil_) {
    cur2 = cur2->right;
    assert(cur1->right);
    node *tmp = new node(cur2, nil_, cur1);
    cur1->right = tmp;
    cur1 = cur1->right;
    go_again = true;
    break;
  }
}

template <typename T>
void RBTree<T>::delete_tree() {
  node *cur = head_;
  node *tmp;
  bool go_again = false;
  while (head_ && head_ != nil_) {
    while (cur->left && cur->left != nil_) {
      cur = cur->left;
    }
    while (cur->right && cur->right != nil_) {
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
      } else {
        delete tmp;
        cur->left = nullptr;
      }
    } else if ((cur->left == nullptr || cur->left == nil_) &&
               (cur->right != nullptr || cur->right != nil_)) {
      delete head_;
      head_ = nullptr;
    }
  }
  delete nil_;
  nil_ = nullptr;
}

}  // namespace trees
#endif  // RBTREE_INCLUDES_RBTREE_IMPL