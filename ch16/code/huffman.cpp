#include <queue>
#include <vector>

struct Node
{
    Node *left;
    Node *right;
    int freq;
    char character;
};

Node* Huffman(std::vector<Node*>&& alphabet)
{
    size_t n = alphabet.size();
    auto cmp = [](Node *a, Node *b) { return a->freq > b->freq; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> 
        q(cmp, std::forward<std::vector<Node*> >(alphabet));
    for (size_t i = 0; i < n - 1; ++i)
    {
        Node *z = new Node;
        z->left = q.top();
        q.pop();
        z->right = q.top();
        q.pop();
        z->freq = z->left->freq + z->right->freq;
        q.push(z);
    }
    return q.top();
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("Example")
{
    std::cout << "------------------------" << std::endl;
    Node *a = new Node, *b = new Node, *c = new Node, 
         *d = new Node, *e = new Node, *f = new Node;
    a->left = a->right = b->left = b->right = c->left = c->right = 
        d->left = d->right = e->left = e->right = f->left = f->right = nullptr;
    a->freq = 45;
    b->freq = 13;
    c->freq = 12;
    d->freq = 16;
    e->freq = 9;
    f->freq = 5;
    a->character = 'a';
    b->character = 'b';
    c->character = 'c';
    d->character = 'd';
    e->character = 'e';
    f->character = 'f';
    std::vector<Node*> alphabet = { a, b, c, d, e, f };
    clock_t start = clock();
    Node *root = Huffman(std::forward<std::vector<Node*> >(alphabet));
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    REQUIRE(root->left->character == 'a');
    REQUIRE(root->right->left->left->character == 'c');
    REQUIRE(root->right->left->right->character == 'b');
    REQUIRE(root->right->right->left->left->character == 'f');
    REQUIRE(root->right->right->left->right->character == 'e');
    REQUIRE(root->right->right->right->character == 'd');
}
