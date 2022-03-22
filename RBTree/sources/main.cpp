#include <iostream>

#include "rbtree_impl.hpp"

int main() {
  std::string sign;
  size_t stat;
  int64_t num;
  trees::RBTree<int64_t> tree;
  std::cin >> sign;
  while (!std::cin.eof()) {
    if (sign.size() == 1 && sign == "k") {
      std::cin >> num;
      tree.insert(num);
    } else if (sign.size() == 1 && sign == "m") {
      std::cin >> stat;
      std::cout << tree.mth_statistic(stat) << std::endl;
    } else if (sign.size() == 1 && sign == "n") {
      std::cin >> num;
      std::cout << tree.num_of_less(num) << std::endl;
    } else {
      std::cout << "Unknown command" << std::endl;
    }
    std::cin >> sign;
  }
  return 0;
}