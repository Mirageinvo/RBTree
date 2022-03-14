#include <iostream>
#include "rbtree_impl.hpp"

int main() {
    std::string sign;
    int num;
    Trees::RBTree<int> tree;
    std::cin >> sign;
    while (!std::cin.eof()) {
        if (sign.size() == 1 && sign == "k") {
            std::cin >> num;
            tree.insert(num);
        }
        /*else if (sign.size() == 1 && sign == "m") {
            std::cin >> num;
        }*/
        else if (sign.size() == 1 && sign == "n") {
            std::cin >> num;
            std::cout << tree.num_of_less(num) << std::endl;
        }
        else {
            //std::cout << "Unknown command" << std::endl;
            return 0;
        }
        std::cin >> sign;
    }
    return 0;
}