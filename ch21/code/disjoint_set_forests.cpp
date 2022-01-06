struct Node
{
    int key;
    Node *p;
    int rank;
};

void MakeSet(Node *x)
{
    x->p = x;
    x->rank = 0;
}

void Link(Node *x, Node *y)
{
    if (x->rank > y->rank)
    {
        y->p = x;
    }
    else
    {
        x->p = y;
        if (x->rank == y->rank)
            ++y->rank;
    }
}

/* // recursive version
Node* FindSet(Node *x)
{
    if (x != x->p)
        x->p = FindSet(x->p);
    return x->p;
} */

// nonrecursive version
Node* FindSet(Node *x)
{
    Node *representative, *tmp;
    representative = x;
    while (representative != representative->p)
    {
        representative = representative->p;
    }
    while (x->p != representative)
    {
        tmp = x->p;
        x->p = representative;
        x = tmp;
    }
    return representative;
}

void Union(Node *x, Node *y)
{
    Link(FindSet(x), FindSet(y));
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

Node* FindSetWithoutChangingStructure(Node *x)
{
    Node *representative;
    representative = x;
    while (representative != representative->p)
    {
        representative = representative->p;
    }
    return representative;
}

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
    Node *representative, *representative_curr;
    std::unordered_map<int, Node*> node_map;
    for (Node *node : nodes)
    {
        node_map[node->key] = node;
    }
    for (const std::list<int>& set : require_collection)
    {
        representative = nullptr;
        for (int key : set)
        {
            assert(node_map.count(key));
            representative_curr = FindSetWithoutChangingStructure(node_map[key]);
            if (representative)
                assert(representative == representative_curr);
            else
                representative = representative_curr;
            node_map.erase(key);
        }
    }
    assert(node_map.empty() == true);
}

void CheckNode(Node *node, int key, Node *p, int rank)
{
    assert(node->key == key);
    assert(node->p == p);
    assert(node->rank == rank);
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
    REQUIRE(FindSetWithoutChangingStructure(b) == FindSetWithoutChangingStructure(c));
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

TEST_CASE("21.3-1")
{
    int i;
    std::cout << "-------------------\n";
    Node *x[17];
    for (i = 1; i <= 16; ++i)
    {
        x[i] = AllocateNodeAndMakeSet(i);
    }
    Check({ x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], 
        x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16] }, 
        { { 1 }, { 2 }, { 3 }, { 4 }, { 5 }, { 6 }, { 7 }, { 8 }, 
        { 9 }, { 10 }, { 11 }, { 12 }, { 13 }, { 14 }, { 15 }, { 16 } });
    for (i = 1; i <= 15; i += 2)
    {
        Union(x[i], x[i + 1]);
    }
    Check({ x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], 
        x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16] }, 
        { { 1, 2 }, { 3, 4 }, { 5, 6 }, { 7, 8 }, 
        { 9, 10 }, { 11, 12 }, { 13, 14 }, { 15, 16 } });
    for (i = 1; i <= 13; i += 4)
    {
        Union(x[i], x[i + 2]);
    }
    Check({ x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], 
        x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16] }, 
        { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, 
        { 9, 10, 11, 12 }, { 13, 14, 15, 16 } });
    Union(x[1], x[5]);
    Check({ x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], 
        x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16] }, 
        { { 1, 2, 3, 4, 5, 6, 7, 8 }, 
        { 9, 10, 11, 12 }, { 13, 14, 15, 16 } });
    Union(x[11], x[13]);
    Check({ x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], 
        x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16] }, 
        { { 1, 2, 3, 4, 5, 6, 7, 8 }, 
        { 9, 10, 11, 12, 13, 14, 15, 16 } });
    Union(x[1], x[10]);
    Check({ x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], 
        x[9], x[10], x[11], x[12], x[13], x[14], x[15], x[16] }, 
        { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 } });
    Node *r2 = FindSet(x[2]);
    Node *r9 = FindSet(x[9]);
    REQUIRE(r2 == r9);
    REQUIRE(r2 == x[16]);
    CheckNode(x[1], 1, x[8], 0);
    CheckNode(x[2], 2, x[16], 1);
    CheckNode(x[3], 3, x[4], 0);
    CheckNode(x[4], 4, x[16], 2);
    CheckNode(x[5], 5, x[8], 0);
    CheckNode(x[6], 6, x[8], 1);
    CheckNode(x[7], 7, x[8], 0);
    CheckNode(x[8], 8, x[16], 3);
    CheckNode(x[9], 9, x[16], 0);
    CheckNode(x[10], 10, x[16], 1);
    CheckNode(x[11], 11, x[12], 0);
    CheckNode(x[12], 12, x[16], 2);
    CheckNode(x[13], 13, x[16], 0);
    CheckNode(x[14], 14, x[16], 1);
    CheckNode(x[15], 15, x[16], 0);
    CheckNode(x[16], 16, x[16], 4);
    for (i = 1; i <= 16; ++i)
    {
        delete x[i];
    }
}
