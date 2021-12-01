// Note: sentinel leaf NIL require

#include <algorithm>

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>

class AvlTree
{
public:
    struct Node
    {
        Node *left;
        Node *right;
        int height;
        int key;
    };

    Node *nil_;
    Node *root_;

    AvlTree()
    {
        root_ = nil_ = new Node();
        nil_->height = 0;
    }

    ~AvlTree()
    {
        delete nil_;
    }

    void LeftRotate(Node **subtree_root_ptr)
    {
        Node *new_root;
        new_root = (*subtree_root_ptr)->right;
        (*subtree_root_ptr)->right = new_root->left;
        new_root->left = *subtree_root_ptr;
        *subtree_root_ptr = new_root;
        new_root->left->height = 
            std::max(new_root->left->left->height, 
                new_root->left->right->height) 
            + 1;
        new_root->height = 
            std::max(new_root->left->height, new_root->right->height) 
            + 1;
    }

    void RightRotate(Node **subtree_root_ptr)
    {
        Node *new_root;
        new_root = (*subtree_root_ptr)->left;
        (*subtree_root_ptr)->left = new_root->right;
        new_root->right = *subtree_root_ptr;
        *subtree_root_ptr = new_root;
        new_root->right->height = 
            std::max(new_root->right->left->height, 
                new_root->right->right->height) 
            + 1;
        new_root->height = 
            std::max(new_root->left->height, new_root->right->height) 
            + 1;
    }

    // 13-p-3 (b)
    void Balance(Node **subtree_root_ptr)
    {
        if (std::abs((*subtree_root_ptr)->left->height - 
            (*subtree_root_ptr)->right->height) > 1)
        {
            if ((*subtree_root_ptr)->right->height > 
                (*subtree_root_ptr)->left->height)
            {
                if ((*subtree_root_ptr)->right->right->height < 
                    (*subtree_root_ptr)->right->left->height)
                    RightRotate(&((*subtree_root_ptr)->right));
                LeftRotate(subtree_root_ptr);
            }
            else
            {
                if ((*subtree_root_ptr)->left->left->height < 
                    (*subtree_root_ptr)->left->right->height)
                    LeftRotate(&((*subtree_root_ptr)->left));
                RightRotate(subtree_root_ptr);
            }
        }
    }

    // 13-p-3 (c)
    void InsertRecursive(Node **subtree_root_ptr, int key)
    {
        if (*subtree_root_ptr == nil_)
        {
            *subtree_root_ptr = new Node();
            (*subtree_root_ptr)->key = key;
            (*subtree_root_ptr)->left = nil_;
            (*subtree_root_ptr)->right = nil_;
            (*subtree_root_ptr)->height = 1;
        }
        else if ((*subtree_root_ptr)->key < key)
        {
            InsertRecursive(&((*subtree_root_ptr)->right), key);
            (*subtree_root_ptr)->height = (*subtree_root_ptr)->right->height + 1;
            Balance(subtree_root_ptr);
        }
        else
        {
            InsertRecursive(&((*subtree_root_ptr)->left), key);
            (*subtree_root_ptr)->height = (*subtree_root_ptr)->left->height + 1;
            Balance(subtree_root_ptr);
        }
    }

    void InsertRecursive(int key)
    {
        InsertRecursive(&root_, key);
    }

    bool TestAvlValid(Node *subtree_root)
    {
        if (subtree_root == nil_) return true;
        if (std::abs(subtree_root->left->height - subtree_root->right->height) > 1)
            return false;
        if (std::abs(subtree_root->height - 
            std::max(subtree_root->left->height, subtree_root->right->height)) > 1)
            return false;
        return TestAvlValid(subtree_root->left) && TestAvlValid(subtree_root->right);
    }

    bool TestAvlValid()
    {
        return TestAvlValid(root_);
    }

};

TEST_CASE("avl insert example")
{
    AvlTree avl;
    avl.InsertRecursive(11);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(2);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(14);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(7);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(15);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(8);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(5);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(1);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(4);
    REQUIRE(avl.TestAvlValid());
}

TEST_CASE("avl insert in order")
{
    AvlTree avl;
    avl.InsertRecursive(1);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(2);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(3);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(4);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(5);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(6);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(7);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(8);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(9);
    REQUIRE(avl.TestAvlValid());
}

TEST_CASE("avl insert in reverse order")
{
    AvlTree avl;
    avl.InsertRecursive(9);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(8);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(7);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(6);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(5);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(4);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(3);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(2);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(1);
    REQUIRE(avl.TestAvlValid());
}

TEST_CASE("avl insert random 1")
{
    AvlTree avl;
    avl.InsertRecursive(324);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(16);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(43);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(6432);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(4321);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(466);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(3211);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(543);
    REQUIRE(avl.TestAvlValid());
    avl.InsertRecursive(21561);
    REQUIRE(avl.TestAvlValid());
}
