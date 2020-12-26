#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "TreeNode/TreeNode.h"

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
      child2(&root),
      subChild1(&child1),
      subChild2(&child1),
      subChild3(&child2);

  EXPECT_CALL(root, toCall()).Times(1);
  EXPECT_CALL(child1, toCall()).Times(1);
  EXPECT_CALL(child2, toCall()).Times(1);
  EXPECT_CALL(subChild1, toCall()).Times(1);
  EXPECT_CALL(subChild2, toCall()).Times(1);
  EXPECT_CALL(subChild3, toCall()).Times(1);

  // From root
  for (auto it = root.begin(); it != root.end(); ++it)
  {
    it->toCall();
  }

  EXPECT_CALL(child1, toCall()).Times(1);
  EXPECT_CALL(subChild1, toCall()).Times(1);
  EXPECT_CALL(subChild2, toCall()).Times(1);

  // From sub node
  for (auto &node : child1)
  {
    node.toCall();
  }
}
