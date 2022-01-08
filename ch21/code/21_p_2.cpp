struct Node
{
    Node *p;
    int rank;
    int d;
};

void MakeTree(Node *v)
{
    v->p = v;
    v->rank = 0;
    v->d = 0;
}

// after the function return, v->p is the root of the tree in the disjoint sets
void Compression(Node *v)
{
    if (v->p != v->p->p)
    {
        Compression(v->p);
        v->d += v->p->d;
        v->p = v->p->p;
    }
}

// after the function return, v->p is the root of the tree in the disjoint sets
int FindDepth(Node *v)
{
    Compression(v);
    return (v == v->p) ? v->d : (v->d + v->p->d);
}

void Graft(Node *r, Node *v)
{
    Node *r_set, *v_set;
    Compression(r);
    // r_set is the root node of the disjoint set contains r
    r_set = r->p;
    // add depths of all nodes in the tree contains node r by the depth of node v
    // correctness: disjoint set contains r is 
    //              exactly the set contains all elements in the tree contains r
    r_set->d += (FindDepth(v) + 1);
    // v_set is the root node of the disjoint set contains v
    v_set = v->p;
    if (r_set->rank > v_set->rank)
    {
        v_set->p = r_set;
        // since r_set becomes parent of v_set in the disjoint set,
        // we need to subtract pseudodistance of v_set by that of r_set
        v_set->d -= r_set->d;
    }
    else
    {
        r_set->p = v_set;
        // since v_set becomes parent of r_set in the disjoint set,
        // we need to subtract pseudodistance of r_set by that of v_set
        r_set->d -= v_set->d;
        if (r_set->rank == v_set->rank)
            ++v_set->rank;
    }
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("1")
{
    Node *a, *b, *c, *d, *e, *f, *g, *h;

    a = new Node;
    b = new Node;
    c = new Node;
    d = new Node;
    e = new Node;
    f = new Node;
    g = new Node;
    h = new Node;

    MakeTree(a);
    MakeTree(b);
    MakeTree(c);
    MakeTree(d);
    MakeTree(e);
    MakeTree(f);
    MakeTree(g);
    MakeTree(h);

    Graft(b, a);
    Graft(h, c);
    Graft(c, a);
    Graft(e, d);
    Graft(g, f);
    REQUIRE(a->d == 0);
    REQUIRE(b->d == 1);
    REQUIRE(c->d == 1);
    REQUIRE(h->d == 1);
    REQUIRE(d->d == 0);
    REQUIRE(e->d == 1);
    REQUIRE(f->d == 0);
    REQUIRE(g->d == 1);
    REQUIRE(a->p == a);
    REQUIRE(b->p == a);
    REQUIRE(c->p == a);
    REQUIRE(h->p == c);
    REQUIRE(d->p == d);
    REQUIRE(e->p == d);
    REQUIRE(f->p == f);
    REQUIRE(g->p == f);

    Graft(a, e);
    REQUIRE(a->d == 2);
    REQUIRE(b->d == 1);
    REQUIRE(c->d == 1);
    REQUIRE(h->d == 1);
    REQUIRE(d->d == -2);
    REQUIRE(e->d == 1);
    REQUIRE(f->d == 0);
    REQUIRE(g->d == 1);
    REQUIRE(a->p == a);
    REQUIRE(b->p == a);
    REQUIRE(c->p == a);
    REQUIRE(h->p == c);
    REQUIRE(d->p == a);
    REQUIRE(e->p == d);
    REQUIRE(f->p == f);
    REQUIRE(g->p == f);

    Graft(d, g);
    REQUIRE(a->d == 4);
    REQUIRE(b->d == 1);
    REQUIRE(c->d == 1);
    REQUIRE(h->d == 1);
    REQUIRE(d->d == -2);
    REQUIRE(e->d == 1);
    REQUIRE(f->d == -4);
    REQUIRE(g->d == 1);
    REQUIRE(a->p == a);
    REQUIRE(b->p == a);
    REQUIRE(c->p == a);
    REQUIRE(h->p == c);
    REQUIRE(d->p == a);
    REQUIRE(e->p == d);
    REQUIRE(f->p == a);
    REQUIRE(g->p == f);

    REQUIRE(FindDepth(f) == 0);
    REQUIRE(FindDepth(g) == 1);
    REQUIRE(FindDepth(d) == 2);
    REQUIRE(FindDepth(e) == 3);
    REQUIRE(FindDepth(a) == 4);
    REQUIRE(FindDepth(b) == 5);
    REQUIRE(FindDepth(c) == 5);
    REQUIRE(FindDepth(h) == 6);

    REQUIRE(a->p == a);
    REQUIRE(b->p == a);
    REQUIRE(c->p == a);
    REQUIRE(d->p == a);
    REQUIRE(e->p == a);
    REQUIRE(f->p == a);
    REQUIRE(g->p == a);
    REQUIRE(h->p == a);

    REQUIRE(FindDepth(f) == 0);
    REQUIRE(FindDepth(g) == 1);
    REQUIRE(FindDepth(d) == 2);
    REQUIRE(FindDepth(e) == 3);
    REQUIRE(FindDepth(a) == 4);
    REQUIRE(FindDepth(b) == 5);
    REQUIRE(FindDepth(c) == 5);
    REQUIRE(FindDepth(h) == 6);

    REQUIRE(a->p == a);
    REQUIRE(b->p == a);
    REQUIRE(c->p == a);
    REQUIRE(d->p == a);
    REQUIRE(e->p == a);
    REQUIRE(f->p == a);
    REQUIRE(g->p == a);
    REQUIRE(h->p == a);

    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    delete g;
    delete h;
}
