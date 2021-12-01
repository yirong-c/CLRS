#ifndef _BINARY_SEARCH_TREES_HPP
#define _BINARY_SEARCH_TREES_HPP

#include <iostream>
#include <stack>
#include "stdafx.h"

template <typename T1, typename T2>
struct BinaryTreeNode
{
    BinaryTreeNode<T1, T2>* parent;
    BinaryTreeNode<T1, T2>* left;
    BinaryTreeNode<T1, T2>* right;
    T1 data;
    T2 key;

    BinaryTreeNode(T2 key)
    {
        this->key = key;
        this->parent = nullptr;
        this->left = nullptr;
        this->right = nullptr;
    }

    void SetRelation(BinaryTreeNode<T1, T2>* parent,
        BinaryTreeNode<T1, T2>* left, 
        BinaryTreeNode<T1, T2>* right)
    {
        this->parent = parent;
        this->left = left;
        this->right = right;
    }
};

template <typename T1, typename T2>
class BinarySearchTree
{
public:
    BinaryTreeNode<T1, T2>* root_node_;

    //O(n)
    void InorderTreeWalkRecursive(BinaryTreeNode<T1, T2>* x)
    {
        if (x != nullptr)
        {
            InorderTreeWalkRecursive(x->left);
            std::cout << x->key << std::endl;
            InorderTreeWalkRecursive(x->right);
        }
    }

    //O(n)
    void InorderTreeWalkRecursive()
    {
        InorderTreeWalkRecursive(root_node_);
    }

    //12.1-3
    //O(n)
    //space complexity: O(n)
    void InorderTreeWalkNonRecursive()
    {
        std::stack<BinaryTreeNode<T1, T2>*> stack;
        BinaryTreeNode<T1, T2>* x;
        x = root_node_;
        while (true)
        {
            while (x != nullptr)
            {
                stack.push(x);
                x = x->left;
            }
            if (stack.empty())
            {
                break;
            }
            x = stack.top();
            std::cout << x->key << std::endl;
            stack.pop();
            x = x->right;
        }
    }

    //12.1-3
    //O(n)
    //space complexity: O(1)
    void InorderTreeWalkNonRecursiveNonStack()
    {
        BinaryTreeNode<T1, T2> *last, *x;
        x = root_node_;
        goto Start;
        while (x->parent)
        {
            last = x;
            x = x->parent;
            if (last == x->left)
            {
                std::cout << x->key << std::endl;
                while (x->right)
                {
                    x = x->right;
                    Start:
                    while (x->left)
                    {
                        x = x->left;
                    }
                    std::cout << x->key << std::endl;
                }
            }
        }
    }

    //12.1-4
    //O(n)
    void PreorderTreeWalkRecursive(BinaryTreeNode<T1, T2>* x)
    {
        if (x != nullptr)
        {
            std::cout << x->key << std::endl;
            PreorderTreeWalkRecursive(x->left);
            PreorderTreeWalkRecursive(x->right);
        }
    }

    //12.1-4
    //O(n)
    void PreorderTreeWalkRecursive()
    {
        PreorderTreeWalkRecursive(root_node_);
    }

    //12.1-4
    //O(n)
    void PostorderTreeWalkRecursive(BinaryTreeNode<T1, T2>* x)
    {
        if (x != nullptr)
        {
            PostorderTreeWalkRecursive(x->left);
            PostorderTreeWalkRecursive(x->right);
            std::cout << x->key << std::endl;
        }
    }

    //12.1-4
    //O(n)
    void PostorderTreeWalkRecursive()
    {
        PostorderTreeWalkRecursive(root_node_);
    }

    //O(h)
    //h: height (h >= lgn)
    //space complexity: O(h)
    BinaryTreeNode<T1, T2>* TreeSearchRecursive(BinaryTreeNode<T1, T2>* root_node, T2 key)
    {
        if (root_node == nullptr || root_node->key == key)
            return root_node;
        if (key < root_node->key)
            return TreeSearchRecursive(root_node->left, key);
        else
            return TreeSearchRecursive(root_node->right, key);
    }

    //O(h)
    //h: height (h >= lgn)
    //space complexity: O(h)
    BinaryTreeNode<T1, T2>* TreeSearchRecursive(T2 key)
    {
        return TreeSearchRecursive(root_node_, key);
    }

    //O(h)
    //h: height (h >= lgn)
    //space complexity: O(1)
    BinaryTreeNode<T1, T2>* IterativeTreeSearch(BinaryTreeNode<T1, T2>* root_node, T2 key)
    {
        while (root_node != nullptr && root_node->key != key)
        {
            if (key < root_node->key)
                root_node = root_node->left;
            else
                root_node = root_node->right;
        }    
        return root_node;
    }

    //O(h)
    //h: height (h >= lgn)
    //space complexity: O(1)
    BinaryTreeNode<T1, T2>* IterativeTreeSearch(T2 key)
    {
        return IterativeTreeSearch(root_node_, key);
    }

    //O(h)
    //root_node cannot be nullptr
    BinaryTreeNode<T1, T2>* TreeMinimum(BinaryTreeNode<T1, T2>* root_node)
    {
        while (root_node->left)
            root_node = root_node->left;
        return root_node;
    }

    //O(h)
    //root_node cannot be nullptr
    BinaryTreeNode<T1, T2>* TreeMaximum(BinaryTreeNode<T1, T2>* root_node)
    {
        while (root_node->right)
            root_node = root_node->right;
        return root_node;
    }

    //O(h)
    //node cannot be nullptr
    BinaryTreeNode<T1, T2>* TreeSuccessor(BinaryTreeNode<T1, T2>* node)
    {
        BinaryTreeNode<T1, T2>* parent;
        if (node->right != nullptr)
            return TreeMinimum(node->right);
        parent = node->parent;
        while (parent != nullptr && parent->right == node)
        {
            node = parent;
            parent = node->parent;
        }
        return parent;
    }

    //12.2-2
    //O(h)
    //root_node cannot be nullptr
    BinaryTreeNode<T1, T2>* TreeMinimumRecursive(BinaryTreeNode<T1, T2>* root_node)
    {
        if (root_node->left)
            return TreeMinimumRecursive(root_node->left);
        else
            return root_node;
    }

    //12.2-2
    //O(h)
    //root_node cannot be nullptr
    BinaryTreeNode<T1, T2>* TreeMaximumRecursive(BinaryTreeNode<T1, T2>* root_node)
    {
        if (root_node->right)
            return TreeMaximumRecursive(root_node->right);
        else
            return root_node;
    }

    //12.2-3
    //O(h)
    //node cannot be nullptr
    BinaryTreeNode<T1, T2>* TreePredecessor(BinaryTreeNode<T1, T2>* node)
    {
        BinaryTreeNode<T1, T2>* parent;
        if (node->left)
            return TreeMaximum(node->left);
        parent = node->parent;
        while (parent && parent->left == node)
        {
            node = parent;
            parent = node->parent;
        }
        return parent;
    }

    //O(h)
    void TreeInsert(BinaryTreeNode<T1, T2>* to_insert)
    {
        //hardcore version
        BinaryTreeNode<T1, T2>** now_ptr;
        if (root_node_ == nullptr)
        {
            root_node_ = to_insert;
        }
        else
        {
            now_ptr = &root_node_;
            while (*now_ptr)
            {
                to_insert->parent = *now_ptr;
                if (to_insert->key < (*now_ptr)->key)
                    now_ptr = &((*now_ptr)->left);
                else
                    now_ptr = &((*now_ptr)->right);
            }
            *now_ptr = to_insert;
        }
        /*
        //normal version
                BinaryTreeNode<T1, T2>* now;
        if (root_node_ == nullptr)
        {
            root_node_ = to_insert;
        }
        else
        {
            now = root_node_;
            while (now)
            {
                to_insert->parent = now;
                if (to_insert->key < now->key)
                    now = now->left;
                else
                    now = now->right;
            }
            if (to_insert->key < to_insert->parent->key)
                to_insert->parent->left = to_insert;
            else
                to_insert->parent->right = to_insert;
        }
        */
    }

    //replace subtree rooted at node u with subtree rooted at node v
    //u cannot be nullptr
    void Transplant(BinaryTreeNode<T1, T2>* u, BinaryTreeNode<T1, T2>* v)
    {
        if (u->parent == nullptr)
            root_node_ = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
        //u->parent = nullptr;
    }

    //O(h)
    //to_delete cannot be nullptr
    void TreeDeleteReplaceWithSucc(BinaryTreeNode<T1, T2>* to_delete)
    {
        BinaryTreeNode<T1, T2>* will_replace;
        if (to_delete->left == nullptr)
        {
            Transplant(to_delete, to_delete->right);
        }
        else if (to_delete->right == nullptr)
        {
            Transplant(to_delete, to_delete->left);
        }
        else
        {
            will_replace = TreeMinimum(to_delete->right);
            if (will_replace->parent != to_delete)
            {
                Transplant(will_replace, will_replace->right);
                will_replace->right = to_delete->right;
                will_replace->right->parent = will_replace;
            }
            Transplant(to_delete, will_replace);
            will_replace->left = to_delete->left;
            will_replace->left->parent = will_replace;
        }
    }

    //12.3-6
    //O(h)
    //to_delete cannot be nullptr
    void TreeDeleteReplaceWithPred(BinaryTreeNode<T1, T2>* to_delete)
    {
        BinaryTreeNode<T1, T2>* will_replace;
        if (to_delete->left == nullptr)
        {
            Transplant(to_delete, to_delete->right);
        }
        else if (to_delete->right == nullptr)
        {
            Transplant(to_delete, to_delete->left);
        }
        else
        {
            will_replace = TreeMaximum(to_delete->left);
            if (will_replace->parent != to_delete)
            {
                Transplant(will_replace, will_replace->left);
                will_replace->left = to_delete->left;
                will_replace->left->parent = will_replace;
            }
            Transplant(to_delete, will_replace);
            will_replace->right = to_delete->right;
            will_replace->right->parent = will_replace;
        }
    }

    //12.3-6
    //O(h)
    //to_delete cannot be nullptr
    void TreeDeleteReplaceWithRandomSuccPred(BinaryTreeNode<T1, T2>* to_delete)
    {
        if (GetRangedRandomNum(0, 1))
            TreeDeleteReplaceWithPred(to_delete);
        else
            TreeDeleteReplaceWithSucc(to_delete);
    }

    //12.3-1
    //O(h)
    void TreeInsertRecursive(BinaryTreeNode<T1, T2>* root_node, BinaryTreeNode<T1, T2>* to_insert)
    {
        //hardcore version
        BinaryTreeNode<T1, T2>** next_ptr;
        if (to_insert->key < root_node->key)
            next_ptr = &root_node->left;
        else
            next_ptr = &root_node->right;
        if (*next_ptr == nullptr)
        {
            *next_ptr = to_insert;
            to_insert->parent = root_node;
            return;
        }
        TreeInsertRecursive(*next_ptr, to_insert);
        /*
        //normal version
        if (to_insert->key < root_node->key)
        {
            if (root_node->left)
            {
                TreeInsertRecursive(root_node->left, to_insert);
            }
            else
            {
                root_node->left = to_insert;
                to_insert->parent = root_node;
            }
        }
        else
        {
            if (root_node->right)
            {
                TreeInsertRecursive(root_node->right, to_insert);
            }
            else
            {
                root_node->right = to_insert;
                to_insert->parent = root_node;
            }
        }
        */
    }

    //12.3-1
    //O(h)
    void TreeInsertRecursive(BinaryTreeNode<T1, T2>* to_insert)
    {
        if (root_node_ == nullptr)
            root_node_ = to_insert;
        else
            TreeInsertRecursive(root_node_, to_insert);
    }



};

#endif
