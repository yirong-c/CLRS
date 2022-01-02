#include <vector>

struct Node
{
    Node *left;
    Node *right;
    int size;
    int key;
};

void RebuildTreeTranverse(Node* root, std::vector<Node*>& elements)
{
    if (root != nullptr)
    {
        RebuildTreeTranverse(root->left, elements);
        elements.push_back(root);
        RebuildTreeTranverse(root->right, elements);
    }
}

Node* RebuildTreeBuild(std::vector<Node*>& elements, int lower, int upper)
{
    int middle;
    if (lower > upper)
        return nullptr;
    middle = lower + ((upper - lower) >> 1);
    elements[middle]->size = upper - lower + 1;
    elements[middle]->left = RebuildTreeBuild(elements, lower, middle - 1);
    elements[middle]->right = RebuildTreeBuild(elements, middle + 1, upper);
    return elements[middle];
}

Node* RebuildTree(Node* root)
{
    std::vector<Node*> elements;
    elements.reserve(root->size);
    RebuildTreeTranverse(root, elements);
    return RebuildTreeBuild(elements, 0, root->size - 1);
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

void InitNode(Node *node, Node *left, Node *right, int size, int key)
{
    node->left = left;
    node->right = right;
    node->size = size;
    node->key = key;
}

TEST_CASE("rebuild 1/2-balanced tree")
{
    int i, j, h;
    std::cout << "-------------------\n";
    Node n_2, n_3, n_5, n_6, n_7, n_8;
    InitNode(&n_2, nullptr, &n_3, 6, 2);
    InitNode(&n_3, nullptr, &n_7, 5, 3);
    InitNode(&n_5, nullptr, nullptr, 1, 5);
    InitNode(&n_6, &n_5, nullptr, 2, 6);
    InitNode(&n_7, &n_6, &n_8, 4, 7);
    InitNode(&n_8, nullptr, nullptr, 1, 8);
    clock_t start = clock();
    Node *root = RebuildTree(&n_2);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks\n";
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds\n";
    std::cout << "\n";
    REQUIRE(root == &n_5);
    REQUIRE(n_2.left == nullptr);
    REQUIRE(n_2.right == &n_3);
    REQUIRE(n_3.left == nullptr);
    REQUIRE(n_3.right == nullptr);
    REQUIRE(n_5.left == &n_2);
    REQUIRE(n_5.right == &n_7);
    REQUIRE(n_6.left == nullptr);
    REQUIRE(n_6.right == nullptr);
    REQUIRE(n_7.left == &n_6);
    REQUIRE(n_7.right == &n_8);
    REQUIRE(n_8.left == nullptr);
    REQUIRE(n_8.right == nullptr);
}
