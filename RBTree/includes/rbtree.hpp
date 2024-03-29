#ifndef RBTREE_INCLUDES_RBTREE
#define RBTREE_INCLUDES_RBTREE

namespace trees {
enum node_color { BLACK, RED };

template <typename T>
class RBTree final {
 public:
  struct node {
    node() : left(nullptr), right(nullptr), parent(nullptr), data(nullptr) {}
    node(T el, node *nil, node *par = nullptr, int col = BLACK);
    node(node *el, node *nil, node *par);
    node(const RBTree<T>::node &another);
    node(RBTree<T>::node &&another) noexcept;
    ~node();
    node &operator=(const node &another);
    node &operator=(node &&another) noexcept;
    int color = BLACK;
    size_t num_of_less = 0;
    size_t num_of_greater = 0;
    node *left;
    node *right;
    node *parent;
    T *data;
  };

  RBTree();
  RBTree(const RBTree &another);
  RBTree(RBTree &&another) noexcept;
  ~RBTree();
  RBTree &operator=(const RBTree &another);
  RBTree &operator=(RBTree &&another) noexcept;
  void insert(T el);
  size_t num_of_less(T el) const;
  T mth_statistic(size_t stat) const;

 private:
  node *nil_;
  node *head_;
  bool fix_tree(node *init);
  node *grandfather(node *init) const noexcept;
  node *uncle(node *init) const noexcept;
  void rotate_left(node *init) noexcept;
  void rotate_right(node *init) noexcept;
  void move_pointers(node *&cur1, node *&cur2, bool &go_again,
                     const RBTree &another);
  void delete_tree();
};
}  // namespace trees

#endif  // RBTREE_INCLUDES_RBTREE