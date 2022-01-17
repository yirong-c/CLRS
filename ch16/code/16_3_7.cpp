#include <queue>
#include <vector>

struct Node
{
    Node *child_1;
    Node *child_2;
    Node *child_3;
    int freq;
    char character;
};

Node* Huffman(std::vector<Node*>&& alphabet)
{
    size_t n = alphabet.size(), m, i;
    auto cmp = [](Node *a, Node *b) { return a->freq > b->freq; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> 
        q(cmp, std::forward<std::vector<Node*> >(alphabet));
    Node *root;
    if (n % 2 == 0)
    {
        root = new Node;
        root->child_1 = q.top();
        q.pop();
        root->child_2 = q.top();
        q.pop();
        root->child_3 = nullptr;
        root->freq = root->child_1->freq + root->child_2->freq;
        q.push(root);
        m = (n >> 1) - 1;
    }
    else
    {
        m = (n - 1) >> 1;
    }
    for (i = 0; i < m; ++i)
    {
        root = new Node;
        root->child_1 = q.top();
        q.pop();
        root->child_2 = q.top();
        q.pop();
        root->child_3 = q.top();
        q.pop();
        root->freq = root->child_1->freq + root->child_2->freq + root->child_3->freq;
        q.push(root);
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
    a->child_1 = a->child_2 = a->child_3 = 
        b->child_1 = b->child_2 = b->child_3 = 
        c->child_1 = c->child_2 = c->child_3 =
        d->child_1 = d->child_2 = d->child_3 = 
        e->child_1 = e->child_2 = e->child_3 = 
        f->child_1 = f->child_2 = f->child_3 = nullptr;
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
    REQUIRE(root->child_1->character == 'd');
    REQUIRE(root->child_2->child_1->character == 'c');
    REQUIRE(root->child_2->child_2->character == 'b');
    REQUIRE(root->child_2->child_3->child_1->character == 'f');
    REQUIRE(root->child_2->child_3->child_2->character == 'e');
    REQUIRE(root->child_3->character == 'a');
}
