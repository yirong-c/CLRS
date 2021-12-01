#include "binary_search_trees.hpp"

int main()
{
    //build test tree
    typedef BinaryTreeNode<int, int> TestNode;
    typedef TestNode* TestNodePtr;
    BinarySearchTree<int, int> bst;
    TestNode n1(1), n2(2), n3(3), n4(4), n5(5), n6(6), n7(7), 
        n8(8), n9(9), n10(10), n11(11), n12(12), n13(13), 
        n14(14), n15(15), n16(16);
    n1.SetRelation(&n5, nullptr, &n3);
    n2.SetRelation(&n3, nullptr, nullptr);
    n3.SetRelation(&n1, &n2, &n4);
    n4.SetRelation(&n3, nullptr, nullptr);
    n5.SetRelation(&n10, &n1, &n6);
    n6.SetRelation(&n5, nullptr, &n9);
    n7.SetRelation(nullptr, nullptr, nullptr);
    n8.SetRelation(&n9, /*&n7*/nullptr, nullptr);
    n9.SetRelation(&n6, &n8, nullptr);
    n10.SetRelation(nullptr, &n5, &n12);
    n11.SetRelation(&n12, nullptr, nullptr);
    n12.SetRelation(&n10, &n11, &n15);
    n13.SetRelation(&n15, nullptr, &n14);
    n14.SetRelation(&n13, nullptr, nullptr);
    n15.SetRelation(&n12, &n13, &n16);
    n16.SetRelation(&n15, nullptr, nullptr);

    bst.root_node_ = &n10;

    //test functions
    bst.InorderTreeWalkRecursive();
    std::cout << "---" << std::endl;
    bst.InorderTreeWalkNonRecursive();
    std::cout << "---" << std::endl;
    bst.InorderTreeWalkNonRecursiveNonStack();
    std::cout << "---" << std::endl;
    
    bst.PreorderTreeWalkRecursive();
    std::cout << "---" << std::endl;
    bst.PostorderTreeWalkRecursive(&n10);
    std::cout << "---" << std::endl;

    std::cout << "TreeMinimum(&n10)->key: " << bst.TreeMinimum(&n10)->key << std::endl;
    std::cout << "TreeMaximum(&n10)->key: " << bst.TreeMaximum(&n10)->key << std::endl;

    std::cout << "TreeSuccessor(&n4)->key: " << bst.TreeSuccessor(&n4)->key << std::endl;
    std::cout << "TreeSuccessor(&n10)->key: " << bst.TreeSuccessor(&n10)->key << std::endl;
    std::cout << "TreeSuccessor(&n11)->key: " << bst.TreeSuccessor(&n11)->key << std::endl;
    std::cout << "TreeSuccessor(&n16) == nullptr: " << (bst.TreeSuccessor(&n16) == nullptr) << std::endl;

    std::cout << "TreeMinimumRecursive(&n10)->key: " << bst.TreeMinimumRecursive(&n10)->key << std::endl;
    std::cout << "TreeMaximumRecursive(&n10)->key: " << bst.TreeMaximumRecursive(&n10)->key << std::endl;

    std::cout << "TreePredecessor(&n1) == nullptr: " << (bst.TreePredecessor(&n1) == nullptr) << std::endl;
    std::cout << "TreePredecessor(&n2)->key: " << bst.TreePredecessor(&n2)->key << std::endl;
    std::cout << "TreePredecessor(&n4)->key: " << bst.TreePredecessor(&n4)->key << std::endl;
    //std::cout << "TreePredecessor(&n7)->key: " << TreePredecessor(&n7)->key << std::endl;
    std::cout << "TreePredecessor(&n10)->key: " << bst.TreePredecessor(&n10)->key << std::endl;
    std::cout << "TreePredecessor(&n11)->key: " << bst.TreePredecessor(&n11)->key << std::endl;
    std::cout << "TreePredecessor(&n16)->key: " << bst.TreePredecessor(&n16)->key << std::endl;

    bst.TreeInsert(&n7);

    bst.TreeDeleteReplaceWithRandomSuccPred(&n12);
    bst.TreeDeleteReplaceWithRandomSuccPred(&n3);
    bst.TreeDeleteReplaceWithRandomSuccPred(&n10);
    bst.InorderTreeWalkNonRecursiveNonStack();

    return 0;
}
