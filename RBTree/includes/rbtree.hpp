#ifndef RBTREE_INCLUDES_RBTREE
#define RBTREE_INCLUDES_RBTREE

namespace Trees {
enum node_color { BLACK, RED };

template <typename T>
class RBTree {
  struct node {
    node();
    node(T el, node* nil, node* par = nullptr, int col = BLACK);
    node(node* el, node* nil, node* par);
    node(const RBTree<T>::node& another);
    node(RBTree<T>::node&& another) noexcept;
    ~node();
    node& operator=(const node& another);
    node& operator=(node&& another) noexcept;
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
  void move_pointers(node*& cur1, node*& cur2, bool& go_again,
                     const RBTree& another);
  void rotate_left(node* init);
  void rotate_right(node* init);
};
}  // namespace Trees

#endif  // RBTREE_INCLUDES_RBTREE