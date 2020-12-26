#pragma once

/**
 * @brief A class that can be inherited to provide a tree behaviour to a
 * hierachy of objects.
 *
 * The main goal of this class is to provide an in-place tree graph management
 * without dynamic allocation. This means that all information relative to the
 * graph are embedded into the object. The belonging of the objet to a hiearchy
 * is considered to be an intrinsic property.
 *
 * This exposes some limitations:
 *  - An instance can only belong to one graph at a time.
 *  - We expect that children lifetime is always shorter than parents (i.e an
 *    instance should not be destructed before all of its children are
 *    destroyed).
 */
class TreeNode
{
public:
    TreeNode(TreeNode *parent);
    ~TreeNode();

    bool isRoot() { return mParent == nullptr; }

private:
    TreeNode *mParent;
    TreeNode *mChild;
    TreeNode *mSibling;
};
