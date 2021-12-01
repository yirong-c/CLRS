#include <limits.h>
#include <string>
#include <list>

struct Node
{
    Node *left_child = nullptr;
    Node *right_sibling = nullptr;
    std::string name;
    int conviviality_rating;
    int dp_attend_subtree_r_sum = INT_MIN;
    int dp_not_attend_subtree_r_sum = INT_MIN;
    bool dp_attend_if_parent_not_attend;

    Node() {}

    Node(const std::string& name, int conviviality_rating)
        : name(name), conviviality_rating(conviviality_rating) {}
};

void DpAuxRootNotAttend(Node *root);

void DpAuxRootAttend(Node *root)
{
    Node *child;
    root->dp_attend_subtree_r_sum = root->conviviality_rating;
    for (child = root->left_child; child; child = child->right_sibling)
    {
        if (child->dp_not_attend_subtree_r_sum < 0)
            DpAuxRootNotAttend(child);
        root->dp_attend_subtree_r_sum += child->dp_not_attend_subtree_r_sum;
    }
}

void DpAuxRootNotAttend(Node *root)
{
    Node *child;
    root->dp_not_attend_subtree_r_sum = 0;
    for (child = root->left_child; child; child = child->right_sibling)
    {
        if (child->dp_attend_subtree_r_sum < 0)
            DpAuxRootAttend(child);
        if (child->dp_not_attend_subtree_r_sum < 0)
            DpAuxRootNotAttend(child);
        if (child->dp_attend_subtree_r_sum > child->dp_not_attend_subtree_r_sum)
        {
            root->dp_not_attend_subtree_r_sum += child->dp_attend_subtree_r_sum;
            child->dp_attend_if_parent_not_attend = true;
        }
        else
        {
            root->dp_not_attend_subtree_r_sum += child->dp_not_attend_subtree_r_sum;
            child->dp_attend_if_parent_not_attend = false;
        }
    }
}

void OutputListRootNotAttend(Node *root, std::list<Node*>& result);

void OutputListRootAttend(Node *root, std::list<Node*>& result)
{
    Node *child;
    result.push_back(root);
    for (child = root->left_child; child; child = child->right_sibling)
    {
        OutputListRootNotAttend(child, result);
    }
}

void OutputListRootNotAttend(Node *root, std::list<Node*>& result)
{
    Node *child;
    for (child = root->left_child; child; child = child->right_sibling)
    {
        if (child->dp_attend_if_parent_not_attend)
            OutputListRootAttend(child, result);
        else
            OutputListRootNotAttend(child, result);
    }
}

std::list<Node*> PlanningCompanyParty(Node *root)
{
    std::list<Node*> result;
    DpAuxRootAttend(root);
    DpAuxRootNotAttend(root);
    if (root->dp_attend_subtree_r_sum > root->dp_not_attend_subtree_r_sum)
        OutputListRootAttend(root, result);
    else
        OutputListRootNotAttend(root, result);
    return result;
}

#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include <catch/catch.hpp>
#include <iostream>

TEST_CASE("1")
{
    std::cout << "------------------------" << std::endl;
    Node a("a", 50), b("b", 30), c("c", 40), d("d", 45),
        e("e", 20), f("f", 60), g("g", 30), h("h", 20),
        i("i", 25), j("j", 50), k("k", 70), l("l", 10),
        m("m", 5), n("n", 15);
    a.left_child = &b;
    b.right_sibling = &c;
    c.right_sibling = &d;
    b.left_child = &e;
    e.right_sibling = &f;
    c.left_child = &g;
    g.right_sibling = &h;
    h.right_sibling = &i;
    i.right_sibling = &j;
    d.left_child = &k;
    f.left_child = &l;
    i.left_child = &m;
    m.right_sibling = &n;
    clock_t start = clock();
    std::list<Node*> result = PlanningCompanyParty(&a);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (Node*& node : result)
    {
        std::cout << node->name << " ";
    }
    std::cout << std::endl;
    // REQUIRE(true);
}

TEST_CASE("2")
{
    std::cout << "------------------------" << std::endl;
    Node a("a", 50), b("b", 30), c("c", 100), d("d", 70),
        e("e", 20), f("f", 60), g("g", 10), h("h", 20),
        i("i", 25), j("j", 10), k("k", 70), l("l", 10),
        m("m", 5), n("n", 15);
    a.left_child = &b;
    b.right_sibling = &c;
    c.right_sibling = &d;
    b.left_child = &e;
    e.right_sibling = &f;
    c.left_child = &g;
    g.right_sibling = &h;
    h.right_sibling = &i;
    i.right_sibling = &j;
    d.left_child = &k;
    f.left_child = &l;
    i.left_child = &m;
    m.right_sibling = &n;
    clock_t start = clock();
    std::list<Node*> result = PlanningCompanyParty(&a);
    clock_t end = clock();
    std::cout << "running time: " << 
        (double)(end - start) << " clocks" << std::endl;
    std::cout << "running time: " << 
        (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
    std::cout << "result: ";
    for (Node*& node : result)
    {
        std::cout << node->name << " ";
    }
    std::cout << std::endl;
    // REQUIRE(true);
}
