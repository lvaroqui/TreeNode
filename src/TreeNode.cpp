#include "TreeNode/TreeNode.h"

TreeNode::TreeNode(TreeNode *parent) : mParent(parent), mChild(nullptr), mSibling(nullptr)
{
    if (mParent == nullptr)
    {
        return; // If root node, do nothing
    }

    if (mParent->mChild == nullptr)
    {
        mParent->mChild = this;
    }
    else
    {
        // Take previous parent child as sibling and become new first child
        mSibling = mParent->mChild;
        mParent->mChild = this;
    }
}

TreeNode::~TreeNode()
{
    if (mParent == nullptr)
    {
        return; // If root node, we should be the last node
    }

    // If we were the first child, our sibling takes our place
    if (mParent->mChild == this)
    {
        mParent->mChild = mSibling;
    }
}
