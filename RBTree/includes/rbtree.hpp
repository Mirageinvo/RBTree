#ifndef RBTREE_INCLUDES_RBTREE
#define RBTREE_INCLUDES_RBTREE

namespace Trees {
enum { BLACK, RED };

template <typename T>
struct node {
  node(int col = BLACK);
  ~node();

  int color;
  size_t num_of_less;
  size_t num_of_greater;
  node<T>* left;
  node<T>* right;
  node<T>* parent;
  T* data;
};

template <typename T>
class RBTree {
 public:
  RBTree();
  RBTree(const RBTree& another);
  RBTree(RBTree&& another);
  ~RBTree();
  RBTree& operator=(RBTree&& another);
  RBTree& operator=(const RBTree& another);

  void insert(T el);
  size_t num_of_less(T el) const;
  T mth_statistic(size_t stat) const;

 private:
  node<T>* nil_;
  node<T>* head_;
  bool fix_tree(node<T>* init);
  node<T>* grandfather(node<T>* init) const;
  node<T>* uncle(node<T>* init) const;
  void rotate_left(node<T>* init);
  void rotate_right(node<T>* init);
};
}  // namespace Trees

#endif  // RBTREE_INCLUDES_RBTREE