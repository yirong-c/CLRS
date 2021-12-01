#include "12_3_5.hpp"

int main()
{
    //build test tree
    typedef BinaryTreeNode<int, int> TestNode;
    typedef TestNode* TestNodePtr;
    BinarySearchTree<int, int> bst;
    TestNode n1(1), n2(2), n3(3), n4(4), n6(6), n7(7), 
        n9(9), n12(12), n13(13), n15(15), n17(17), n18(18), n19(19), n20(20);
    n1.SetRelation(&n2, nullptr, &n15);
    n2.SetRelation(&n3, nullptr, nullptr);
    n3.SetRelation(&n4, &n2, &n4);
    n4.SetRelation(&n6, nullptr, nullptr);
    n6.SetRelation(&n7, &n3, &n7);
    n7.SetRelation(&n9, nullptr, &n13);
    n9.SetRelation(&n13, nullptr, nullptr);
    n13.SetRelation(&n15, &n9, nullptr);
    n15.SetRelation(&n17, &n6, &n19);
    n17.SetRelation(&n19, nullptr, nullptr);
    n19.SetRelation(&n20, &n17, &n20);
    n20.SetRelation(nullptr, nullptr, nullptr);

    bst.root_node_ = &n1;

    bst.InorderTreeWalkRecursive();
    std::cout << "-------------" << std::endl;

    bst.TreeInsert(&n12);

    bst.InorderTreeWalkRecursive();
    std::cout << "-------------" << std::endl;

    bst.TreeDelete(&n12);
    bst.TreeDelete(&n6);

    bst.InorderTreeWalkRecursive();
    std::cout << "-------------" << std::endl;

    bst.TreeInsert(&n18);

    bst.InorderTreeWalkRecursive();
    std::cout << "-------------" << std::endl;

    bst.TreeDelete(&n15);

    bst.InorderTreeWalkRecursive();
    std::cout << "-------------" << std::endl;

    bst.InorderTreeWalkNonRecursive();
    std::cout << "-------------" << std::endl;

    return 0;
}
