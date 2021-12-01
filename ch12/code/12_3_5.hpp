#include <iostream>

template <typename T1, typename T2>
struct BinaryTreeNode
{
    BinaryTreeNode<T1, T2>* succ;
    BinaryTreeNode<T1, T2>* left;
    BinaryTreeNode<T1, T2>* right;
    T1 data;
    T2 key;

    BinaryTreeNode(T2 key)
    {
        this->key = key;
        this->succ = nullptr;
        this->left = nullptr;
        this->right = nullptr;
    }

    void SetRelation(BinaryTreeNode<T1, T2>* succ,
        BinaryTreeNode<T1, T2>* left, 
        BinaryTreeNode<T1, T2>* right)
    {
        this->succ = succ;
        this->left = left;
        this->right = right;
    }
};

//12.3-5
template <typename T1, typename T2>
class BinarySearchTree
{
public:
    BinaryTreeNode<T1, T2>* root_node_;

    //O(h)
    //root_node cannot be nullptr
    BinaryTreeNode<T1, T2>* TreeMinimum(BinaryTreeNode<T1, T2>* root_node)
    {
        while (root_node->left)
            root_node = root_node->left;
        return root_node;
    }

    //O(h)
    //root_node cannot be nullptr
    BinaryTreeNode<T1, T2>* TreeMaximum(BinaryTreeNode<T1, T2>* root_node)
    {
        while (root_node->right)
            root_node = root_node->right;
        return root_node;
    }

    //O(h)
    //h: height (h >= lgn)
    //space complexity: O(h)
    BinaryTreeNode<T1, T2>* TreeSearchRecursive(BinaryTreeNode<T1, T2>* root_node, T2 key)
    {
        if (root_node == nullptr || root_node->key == key)
            return root_node;
        if (key < root_node->key)
            return TreeSearchRecursive(root_node->left, key);
        else
            return TreeSearchRecursive(root_node->right, key);
    }

    //O(h)
    //h: height (h >= lgn)
    //space complexity: O(h)
    BinaryTreeNode<T1, T2>* TreeSearchRecursive(T2 key)
    {
        return TreeSearchRecursive(root_node_, key);
    }

    //O(h)
    void TreeInsert(BinaryTreeNode<T1, T2>* to_insert)
    {
        BinaryTreeNode<T1, T2> **now_ptr, *pred;
        if (root_node_ == nullptr)
        {
            root_node_ = to_insert;
        }
        else
        {
            now_ptr = &root_node_;
            pred = nullptr;
            while (*now_ptr)
            {
                if (to_insert->key < (*now_ptr)->key)
                {
                    to_insert->succ = *now_ptr;
                    now_ptr = &((*now_ptr)->left);
                }
                else
                {
                    pred = *now_ptr;
                    now_ptr = &((*now_ptr)->right);
                }
            }
            *now_ptr = to_insert;
            if (pred)
                pred->succ = to_insert;
        }
    }

    //O(h)
    BinaryTreeNode<T1, T2>* GetParent(const BinaryTreeNode<T1, T2>* node, BinaryTreeNode<T1, T2>* root_node)
    {
        BinaryTreeNode<T1, T2>* parent;
        parent = nullptr;
        while (root_node != node)
        {
            parent = root_node;
            if (node->key < root_node->key)
                root_node = root_node->left;
            else
                root_node = root_node->right;
        }
        return parent;
    }

    //O(h)
    void GetParentAndPred(BinaryTreeNode<T1, T2>* node, BinaryTreeNode<T1, T2>* root_node, 
        BinaryTreeNode<T1, T2>** parent_ptr, BinaryTreeNode<T1, T2>** pred_ptr)
    {
        *pred_ptr = *parent_ptr = nullptr;
        while (root_node != node)
        {
            *parent_ptr = root_node;
            if (node->key < root_node->key)
            {
                root_node = root_node->left;
            }
            else
            {
                *pred_ptr = root_node;
                root_node = root_node->right;
            }
        }
        if (node->left)
            *pred_ptr = TreeMaximum(node->left);
    }

    //replace subtree rooted at node u with subtree rooted at node v
    //u will be removed from u_pred->succ
    //u cannot be nullptr
    //O(h)
    void Transplant(BinaryTreeNode<T1, T2>* u, BinaryTreeNode<T1, T2>* v)
    {
        BinaryTreeNode<T1, T2> *u_parent, *u_pred;
        GetParentAndPred(u, root_node_, &u_parent, &u_pred);
        if (u_parent == nullptr)
            root_node_ = v;
        else if (u == u_parent->left)
            u_parent->left = v;
        else
            u_parent->right = v;
        if (u_pred != nullptr)
            u_pred->succ = u->succ;
    }

    //O(h)
    //to_delete cannot be nullptr
    void TreeDelete(BinaryTreeNode<T1, T2>* to_delete)
    {
        BinaryTreeNode<T1, T2> *will_replace, *will_replace_2, 
            *to_delete_parent, *to_delete_pred, *will_replace_parent;
        if (to_delete->left == nullptr)
        {
            //replace to_delete with to_delete->right
            Transplant(to_delete, to_delete->right);
        }
        else if (to_delete->right == nullptr)
        {
            //replace to_delete with to_delete->left
            Transplant(to_delete, to_delete->left);
        }
        else
        {
            will_replace = to_delete->succ;//pred of will_replace is to_delete
            if (will_replace != to_delete->right)
            {
                //replace will_replace with will_replace_2
                //then replace to_delete with will_replace
                will_replace_2 = will_replace->right;
                GetParentAndPred(to_delete, root_node_, &to_delete_parent, &to_delete_pred);
                will_replace_parent = GetParent(will_replace, to_delete);
                if (to_delete_parent == nullptr)
                    root_node_ = will_replace;
                else if (to_delete == to_delete_parent->left)
                    to_delete_parent->left = will_replace;
                else
                    to_delete_parent->right = will_replace;
                if (will_replace == will_replace_parent->left)
                    will_replace_parent->left = will_replace_2;
                else
                    will_replace_parent->right = will_replace_2;
                if (to_delete_pred != nullptr)
                    to_delete_pred->succ = to_delete->succ;
                will_replace->left = to_delete->left;
                will_replace->right = to_delete->right;
            }
            else
            {
                //replace to_delete with will_replace
                Transplant(to_delete, will_replace);
                will_replace->left = to_delete->left;
            }
        }
    }

    //O(n)
    void InorderTreeWalkRecursive(BinaryTreeNode<T1, T2>* x)
    {
        if (x != nullptr)
        {
            InorderTreeWalkRecursive(x->left);
            std::cout << x->key << std::endl;
            InorderTreeWalkRecursive(x->right);
        }
    }

    //O(n)
    void InorderTreeWalkRecursive()
    {
        InorderTreeWalkRecursive(root_node_);
    }

    //O(n)
    void InorderTreeWalkNonRecursive()
    {
        BinaryTreeNode<T1, T2>* now;
        now = TreeMinimum(root_node_);
        while (now)
        {
            std::cout << now->key << std::endl;
            now = now->succ;
        }
    }

};
