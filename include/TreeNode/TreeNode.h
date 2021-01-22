#pragma once

#include <cstddef>
#include <type_traits>

/**
 * @brief A class that can be inherited to provide a tree behaviour to a
 * hierarchy of objects.
 *
 * The main goal of this class is to provide an in-place tree graph management
 * without dynamic allocation. This means that all information relative to the
 * graph are embedded into the object. The belonging of the objet to a hierarchy
 * is considered to be an intrinsic property.
 *
 * This exposes some limitations:
 *  - An instance can only belong to one graph at a time.
 *  - We expect that children lifetime is always shorter than parents (i.e an
 *    instance should not be destructed before all of its children are
 *    destroyed).
 *
 * Implementation details:
 * Each nodes holds a pointer to its parent, its first child and its first sibling.
 */
template <typename T>
class TreeNode
{
public:
    /**
     * @brief Creates a new node
     */
    explicit TreeNode(TreeNode *parent) : mParent(parent), mChild(nullptr), mSibling(nullptr)
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
            // Find last sibling of parent and become its sibling
            TreeNode *node = mParent->mChild;
            while (node->mSibling != nullptr)
            {
                node = node->mSibling;
            }
            node->mSibling = this;
        }
    }

    virtual ~TreeNode() = 0; // To enforce pure virtual, implementation is after the class declaration

    TreeNode(const TreeNode &) = delete;
    TreeNode(TreeNode &&) = delete;

    /**
     * Wether this node is the root of a tree
     */
    bool isRoot() { return mParent == nullptr; }

    /**
     * Returns number of node in this node tree (including this one)
     */
    std::size_t size()
    {
        std::size_t size = 0;
        for (auto it = this->begin(); it != this->end(); ++it)
        {
            ++size;
        }
        return size;
    }

    /**
     * Iterator that does a depth-first path of the tree
     */
    class depthFirstIt
    {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T &reference;
        typedef T *pointer;
        typedef std::forward_iterator_tag iterator_category;

        depthFirstIt(const depthFirstIt &) = default;
        ~depthFirstIt() = default;
        reference operator*() { return static_cast<reference>(*mNode); }
        pointer operator->() { return static_cast<pointer>(mNode); }
        friend void swap(depthFirstIt &lhs, depthFirstIt &rhs) { std::swap(lhs, rhs); }

        depthFirstIt &operator++()
        {
            if (mNode->mChild)
            {
                mNode = mNode->mChild;
                return *this;
            }
            if (mNode->mSibling)
            {
                mNode = mNode->mSibling;
                return *this;
            }
            // If we created the iterator on a leaf we can already be the root
            if (mNode == mRoot)
            {
                mIsEnd = true;
                return *this;
            }

            // Climb up the tree until end or next eligible node (first ancestor sibling)
            while (true)
            {
                mNode = mNode->mParent;
                if (mNode == mRoot)
                {
                    mIsEnd = true;
                    return *this;
                }
                if (mNode->mSibling)
                {
                    mNode = mNode->mSibling;
                    return *this;
                }
            }
        }

        depthFirstIt operator++(int)
        {
            depthFirstIt copy(*this);
            ++(*this);
            return copy;
        }

        friend bool operator==(const depthFirstIt &lhs, const depthFirstIt &rhs)
        {
            return lhs.mNode == rhs.mNode &&
                   lhs.mRoot == rhs.mRoot &&
                   lhs.mIsEnd == rhs.mIsEnd;
        }
        friend bool operator!=(const depthFirstIt &lhs, const depthFirstIt &rhs)
        {
            return !(lhs == rhs);
        }

    private:
        friend TreeNode;
        depthFirstIt(TreeNode *node, bool isEnd) : mNode(node), mRoot(node), mIsEnd(isEnd) {}

        TreeNode *mNode; // Current node
        TreeNode *mRoot; // Root of the iteration
        bool mIsEnd;     // Used to detect end, set to true when coming back to root of iteration
    };

    /**
     * Returns a depth-first iterator
     */
    depthFirstIt begin() { return depthFirstIt(this, false); }

    /**
     * Returns the end iterator for the depth-first iterator
     */
    depthFirstIt end() { return depthFirstIt(this, true); }

private:
    TreeNode *mParent;
    TreeNode *mChild;
    TreeNode *mSibling;
};

template <typename T>
TreeNode<T>::~TreeNode()
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
    // We are not the first child and have a sibling, we need to link together
    // the remaining nodes
    else if (mSibling != nullptr)
    {
        // Find our leftSibling
        TreeNode *node = mParent->mChild;
        while (node->mSibling != this)
        {
            node = node->mSibling;
        }
        node->mSibling = mSibling;
    }
}
