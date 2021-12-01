#include "persistent_dynamic_sets.hpp"

#include <iostream>

template <typename Key, typename T>
void InorderTreeWalkPrint(const Node<Key, T>* node)
{
    if (node != nullptr)
    {
        InorderTreeWalkPrint(node->left);
        std::cout << node->value.first << " " << node->value.second << std::endl;
        InorderTreeWalkPrint(node->right);
    }
}

int main()
{
    int i;
    typedef Node<int, int>* TestNodePtr;
    TestNodePtr roots[10];
    roots[0] = nullptr;
    for (i = 1; i < 10; ++i)
    {
        roots[i] = PersistentTreeInsert(roots[i - 1], {(3 * i) % 10, i});
    }
    for (i = 1; i < 10; ++i)
    {
        std::cout << "----- " << i << " -----" << std::endl;
        InorderTreeWalkPrint(roots[i]);
    }
    return 0;
}
