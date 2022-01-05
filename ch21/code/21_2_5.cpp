struct Node;

struct Node
{
    int key;
    Node *next;
    // let the tail element be the set's representative
    union
    {
        Node *tail;// for non-tail elements
        Node *head;// for the tail element
    } representative;
    int size;// only for the tail element
};

void MakeSet(Node *x)
{
    x->next = nullptr;
    x->representative.head = x;
    x->size = 1;
}

Node* FindSet(Node *x)
{
    return x->next ? x->representative.tail : x;
}

void Union(Node *x, Node *y)
{
    Node **node, *x_head, *y_head, *x_representative, *y_representative;
    if (x->representative.tail->size < y->representative.tail->size)
    {
        Union(y, x);
    }
    else
    {
        x_representative = FindSet(x);
        y_representative = FindSet(y);
        x_head = x_representative->representative.head;
        y_head = y_representative->representative.head;
        x_representative->size += y_representative->size;
        node = &y_head;
        while (*node)
        {
            (*node)->representative.tail = x_representative;
            node = &((*node)->next);
        }
        *node = x_head;
        x_representative->representative.head = y_head;
    }
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

#include <list>
#include <unordered_map>
#include <vector>

Node* AllocateNodeAndMakeSet(int key)
{
    Node *node;
    node = new Node;
    node->key = key;
    MakeSet(node);
    return node;
}

void Check(const std::list<Node*>& nodes, 
    const std::list< std::list<int> >& require_collection)
{
    int i;
    std::unordered_map<int, int> solution_map;
    std::unordered_map<Node*, int> node_map;
    for (Node *node : nodes)
    {
        node_map[node] = 1;
    }
    i = 0;
    for (const std::list<int>& set : require_collection)
    {
        for (int key : set)
        {
            solution_map[key] = i;
        }
        ++i;
    }
    std::vector<bool> set_repeat_counter(i, false);
    while (node_map.empty() == false)
    {
        Node *node = node_map.begin()->first;
        assert(solution_map.count(node->key) != 0);
        int set_id = solution_map[node->key];
        Node *representative = FindSet(node);
        assert(set_repeat_counter[set_id] == false);
        set_repeat_counter[set_id] = true;
        i = 0;
        for (node = representative->representative.head; node; node = node->next)
        {
            assert(solution_map.count(node->key) != 0);
            assert(solution_map[node->key] == set_id);
            assert(FindSet(node) == representative);
            solution_map.erase(node->key);
            node_map.erase(node);
            ++i;
        }
        assert(i == representative->size);
    }
    assert(solution_map.empty() == true);
}

TEST_CASE("example")
{
    std::cout << "-------------------\n";
    Node *a = AllocateNodeAndMakeSet('a');
    Node *b = AllocateNodeAndMakeSet('b');
    Node *c = AllocateNodeAndMakeSet('c');
    Node *d = AllocateNodeAndMakeSet('d');
    Node *e = AllocateNodeAndMakeSet('e');
    Node *f = AllocateNodeAndMakeSet('f');
    Node *g = AllocateNodeAndMakeSet('g');
    Node *h = AllocateNodeAndMakeSet('h');
    Node *i = AllocateNodeAndMakeSet('i');
    Node *j = AllocateNodeAndMakeSet('j');
    Check({ a, b, c, d, e, f, g, h, i, j }, { { 'a' }, { 'b' }, { 'c' }, { 'd' }, { 'e' }, 
        { 'f' }, { 'g' }, { 'h' }, { 'i' }, { 'j' },  });
    Union(b, d);
    Check({ a, b, c, d, e, f, g, h, i, j }, { { 'a' }, { 'b', 'd' }, { 'c' }, { 'e' }, 
        { 'f' }, { 'g' }, { 'h' }, { 'i' }, { 'j' },  });
    Union(e, g);
    Check({ a, b, c, d, e, f, g, h, i, j }, { { 'a' }, { 'b', 'd' }, { 'c' }, { 'e', 'g' }, 
        { 'f' }, { 'h' }, { 'i' }, { 'j' },  });
    Union(a, c);
    Check({ a, b, c, d, e, f, g, h, i, j }, { { 'a', 'c' }, { 'b', 'd' }, { 'e', 'g' }, 
        { 'f' }, { 'h' }, { 'i' }, { 'j' },  });
    Union(h, i);
    Check({ a, b, c, d, e, f, g, h, i, j }, { { 'a', 'c' }, { 'b', 'd' }, { 'e', 'g' }, 
        { 'f' }, { 'h', 'i' }, { 'j' },  });
    Union(a, b);
    Check({ a, b, c, d, e, f, g, h, i, j }, { { 'a', 'b', 'c', 'd' }, { 'e', 'g' }, 
        { 'f' }, { 'h', 'i' }, { 'j' },  });
    Union(e, f);
    Check({ a, b, c, d, e, f, g, h, i, j }, { { 'a', 'b', 'c', 'd' }, { 'e', 'f', 'g' }, 
        { 'h', 'i' }, { 'j' },  });
    REQUIRE(FindSet(b) == FindSet(c));
    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    delete g;
    delete h;
    delete i;
    delete j;
}
