#ifndef RBTREE_INCLUDES_RBTREE
#define RBTREE_INCLUDES_RBTREE

namespace Trees {
enum node_color { BLACK, RED };

template <typename T>
class RBTree {
  struct node {
    node();
    node(T el);
    ~node();
    int color;
    size_t num_of_less;
    size_t num_of_greater;
    node* left;
    node* right;
    node* parent;
    T* data;
  };

 public:
  RBTree();
  RBTree(const RBTree& another);
  RBTree(RBTree&& another) noexcept;
  ~RBTree();
  RBTree& operator=(RBTree&& another) noexcept;
  RBTree& operator=(const RBTree& another);
  void insert(T el);
  size_t num_of_less(T el) const;
  T mth_statistic(size_t stat) const;

 private:
  node* nil_;
  node* head_;
  bool fix_tree(node* init);
  node* grandfather(node* init) const;
  node* uncle(node* init) const;
  void rotate_left(node* init);
  void rotate_right(node* init);
};
}  // namespace Trees

#endif  // RBTREE_INCLUDES_RBTREE