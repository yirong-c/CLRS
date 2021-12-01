#ifndef _PERSISTENT_DYNAMIC_SETS_HPP
#define _PERSISTENT_DYNAMIC_SETS_HPP

#include <utility>
#include <stdexcept>

template <typename Key, typename T>
struct Node
{
    Node* left;
    Node* right;
    std::pair<const Key, T> value;
    Node(const std::pair<const Key, T>& value) : value(value) {}
};

template <typename Key, typename T>
Node<Key, T>* PersistentTreeInsert
    (const Node<Key, T>* tree_root, const std::pair<const Key, T>& value)
{
    const Node<Key, T> *now;
    Node<Key, T> *new_root, **copy_next_ptr;
    new_root = nullptr;
    copy_next_ptr = &new_root;
    now = tree_root;
    while (now != nullptr)
    {
        *copy_next_ptr = new Node(now->value);
        if (now->value.first == value.first)
            throw std::runtime_error("repeat key found");
        else if (now->value.first < value.first)
        {
            (*copy_next_ptr)->left = now->left;
            now = now->right;
            copy_next_ptr = &((*copy_next_ptr)->right);
        }
        else
        {
            (*copy_next_ptr)->right = now->right;
            now = now->left;
            copy_next_ptr = &((*copy_next_ptr)->left);
        }
    }
    *copy_next_ptr = new Node(value);
    (*copy_next_ptr)->left = (*copy_next_ptr)->right = nullptr;
    return new_root;
}

#endif