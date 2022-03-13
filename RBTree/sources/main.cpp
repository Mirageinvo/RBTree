#include <iostream>
#include "rbtree_impl.hpp"

int main() {
    char sign;
    int num;
    Trees::RBTree<int> tree;
    std::cin >> sign;
    while (!std::cin.eof()) {
        if (sign == 'k') {
            std::cin >> num;
            tree.insert(num);
        }
        /*else if (sign == 'm') {

        }
        else if (sign == 'n') {

        }*/
        else {
            //std::cout << "Unknown command" << std::endl;
            return 0;
        }
        std::cin >> sign;
    }
    return 0;
}