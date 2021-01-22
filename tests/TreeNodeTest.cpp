#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

#include "TreeNode/TreeNode.h"

using ::testing::InSequence;

struct Mock : public TreeNode<Mock>
{
    explicit Mock(Mock *parent) : TreeNode(parent){};

    MOCK_METHOD(void, toCall, ());
};

TEST(TreeNodeTest, Instantiation)
{
    Mock root(nullptr),
        child1(&root),
        subChild1(&child1),
        child2(&root);

    EXPECT_TRUE(root.isRoot());
    EXPECT_FALSE(child1.isRoot());
    EXPECT_FALSE(child2.isRoot());

    EXPECT_EQ(root.size(), 4);
    EXPECT_EQ(child1.size(), 2);
    EXPECT_EQ(subChild1.size(), 1);
}

TEST(TreeNodeTest, Iterator)
{
    Mock root(nullptr),
        child1(&root),
        subChild1_1(&child1),
        subChild1_2(&child1),
        child2(&root),
        subChild2_1(&child2);

    {
        InSequence seq;
        EXPECT_CALL(root, toCall()).Times(1);
        EXPECT_CALL(child1, toCall()).Times(1);
        EXPECT_CALL(subChild1_1, toCall()).Times(1);
        EXPECT_CALL(subChild1_2, toCall()).Times(1);
        EXPECT_CALL(child2, toCall()).Times(1);
        EXPECT_CALL(subChild2_1, toCall()).Times(1);
    }

    // From root
    for (auto it = root.begin(); it != root.end(); ++it)
    {
        it->toCall();
    }

    {
        InSequence seq;
        EXPECT_CALL(child1, toCall()).Times(1);
        EXPECT_CALL(subChild1_1, toCall()).Times(1);
        EXPECT_CALL(subChild1_2, toCall()).Times(1);
    }

    // From sub node
    for (auto &node : child1)
    {
        node.toCall();
    }
}

TEST(TreeNodeTest, DeleteNode)
{
    Mock *root, *child1, *child2, *child3;
    root = new Mock(nullptr);
    child1 = new Mock(root);
    child2 = new Mock(root);
    child3 = new Mock(root);

    delete child2;
    EXPECT_EQ(root->size(), 3);

    delete child1;
    EXPECT_EQ(root->size(), 2);

    delete child3;
    EXPECT_EQ(root->size(), 1);

    delete root;
}
