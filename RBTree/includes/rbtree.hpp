#ifndef RBTREE_INCLUDES_RBTREE
#define RBTREE_INCLUDES_RBTREE

namespace Trees {
	enum {BLACK, RED};

	template<typename T>
	struct node {
		node(int col = BLACK);
		node(const node& another);
		~node();
		node& operator=(const node& another);

		int color;
		size_t num_of_less;
		size_t num_of_greater;
		node<T>* left;
		node<T>* right;
		node<T>* parent;
		T* data;
	};

	template<typename T>
	class RBTree {
	public:
		RBTree();
		RBTree(const RBTree& another);
		RBTree(RBTree&& another);
		~RBTree();
		RBTree& operator=(RBTree&& another);
		RBTree& operator=(const RBTree& another);

		void insert(T el);
	private:
		node<T>* nil_;
		node<T>* head_;
		bool fix_tree(node<T>* init);
		node<T>* grandfather(node<T>* init);
	};
}

#endif // RBTREE_INCLUDES_RBTREE