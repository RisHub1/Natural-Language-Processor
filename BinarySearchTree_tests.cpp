// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;

TEST(test_default) {
    BinarySearchTree<int> tree;

    ASSERT_TRUE(tree.empty());
    ASSERT_EQUAL(tree.size(), 0);
    ASSERT_EQUAL(tree.height(), 0);

    ostringstream preorder;
    ostringstream inorder;
    tree.traverse_inorder(inorder);
    tree.traverse_preorder(preorder);
    ASSERT_EQUAL(inorder.str(), "");
    ASSERT_EQUAL(preorder.str(), "");
}

TEST(test_insert) {
    BinarySearchTree<int> tree;

    BinarySearchTree<int>::Iterator i;
    i = tree.insert(13);
    ASSERT_EQUAL(*i, 13)
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 1);
    ASSERT_EQUAL(tree.height(), 1);
    

    i = tree.insert(7);
    ASSERT_EQUAL(*i, 7)
    tree.insert(4);
    tree.insert(10);
    tree.insert(19);
    i = tree.insert(16);
    ASSERT_EQUAL(*i, 16)
    tree.insert(22);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 7);

    ostringstream preorder;
    ostringstream inorder;
    tree.traverse_inorder(inorder);
    tree.traverse_preorder(preorder);
    ASSERT_EQUAL(inorder.str(), "4 7 10 13 16 19 22 ");
    ASSERT_EQUAL(preorder.str(), "13 7 4 10 19 16 22 ");
}

TEST(test_find)
{
    BinarySearchTree<int> tree;

    tree.insert(13);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 1);

    BinarySearchTree<int>::Iterator i;
    i = tree.find(13);
    ASSERT_EQUAL(*i, 13);
    i = tree.find(15);
    ASSERT_EQUAL(i, tree.end());

    tree.insert(7);
    ASSERT_EQUAL(tree.height(), 2);
    tree.insert(4);
    ASSERT_EQUAL(tree.height(), 3);
    tree.insert(10);
    ostringstream preorder;
    ostringstream inorder;
    tree.traverse_inorder(inorder);
    tree.traverse_preorder(preorder);
    ASSERT_EQUAL(inorder.str(), "4 7 10 13 ");
    ASSERT_EQUAL(preorder.str(), "13 7 4 10 ");
    tree.insert(19);
    tree.insert(16);
    tree.insert(22);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 7);

    i = tree.find(4);
    ASSERT_EQUAL(*i, 4);
    i = tree.find(10);
    ASSERT_EQUAL(*i, 10);
    i = tree.find(16);
    ASSERT_EQUAL(*i, 16);
    i = tree.find(22);
    ASSERT_EQUAL(*i, 22);
    i = tree.find(15);
    ASSERT_EQUAL(i, tree.end());
}

TEST(test_max_min)
{
    BinarySearchTree<int> tree;
    ASSERT_EQUAL(tree.max_element(), tree.min_element());
    ASSERT_EQUAL(tree.min_element(), tree.end());

    tree.insert(13);
    ASSERT_EQUAL(tree.max_element(), tree.min_element());
    ASSERT_EQUAL(*(tree.max_element()), 13);
    
    tree.insert(7);
    ASSERT_EQUAL(*(tree.min_element()), 7);
    tree.insert(4);
    ASSERT_EQUAL(*(tree.min_element()), 4);
    tree.insert(10);
    ASSERT_EQUAL(*(tree.min_element()), 4);
    ASSERT_EQUAL(*(tree.max_element()), 13);
    tree.insert(19);
    ASSERT_EQUAL(*(tree.max_element()), 19);
    tree.insert(16);
    ASSERT_EQUAL(*(tree.max_element()), 19);
    ASSERT_EQUAL(*(tree.min_element()), 4);
    tree.insert(22);
    ASSERT_EQUAL(*(tree.max_element()), 22);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 7);
}

TEST(test_min_greater_than)
{
    BinarySearchTree<int> tree;

    tree.insert(13);
    tree.insert(19);
    tree.insert(16);
    tree.insert(22);
    tree.insert(7);
    ostringstream preorder;
    ostringstream inorder;
    tree.traverse_inorder(inorder);
    tree.traverse_preorder(preorder);
    ASSERT_EQUAL(inorder.str(), "7 13 16 19 22 ");
    ASSERT_EQUAL(preorder.str(), "13 7 19 16 22 ");
    tree.insert(4);
    tree.insert(10);

    ASSERT_EQUAL(*(tree.min_greater_than(13)), 16);
    ASSERT_EQUAL(*(tree.min_greater_than(9)), 10);
    ASSERT_EQUAL(tree.min_greater_than(25), tree.end());
    ASSERT_EQUAL(*(tree.min_greater_than(2)), 4);
    ASSERT_EQUAL(*(tree.min_greater_than(21)), 22);
    ASSERT_EQUAL(*(tree.min_greater_than(12)), 13);
    ASSERT_EQUAL(*(tree.min_greater_than(6)), 7);
    ASSERT_EQUAL(*(tree.min_greater_than(18)), 19);
    ASSERT_EQUAL(*(tree.min_greater_than(7)), 10);
}

TEST(test_invariants_greater)
{
    BinarySearchTree<int> tree;

    tree.insert(13);
    tree.insert(19);
    tree.insert(16);
    tree.insert(22);
    tree.insert(7);
    tree.insert(4);
    tree.insert(10);
    
    BinarySearchTree<int>::Iterator i;
    i = tree.find(10);
    *i = 15;
    ASSERT_FALSE(tree.check_sorting_invariant());
}

TEST(test_invariants_less)
{
    BinarySearchTree<int> tree;

    tree.insert(13);
    tree.insert(19);
    tree.insert(16);
    tree.insert(22);
    tree.insert(7);
    tree.insert(4);
    tree.insert(10);
    
    BinarySearchTree<int>::Iterator i;
    i = tree.find(16);
    *i = 12;
    ASSERT_FALSE(tree.check_sorting_invariant());
}

TEST(test_invariants_duplicate)
{
    BinarySearchTree<int> tree;

    tree.insert(13);
    tree.insert(19);
    tree.insert(16);
    tree.insert(22);
    tree.insert(7);
    tree.insert(4);
    tree.insert(10);
    
    BinarySearchTree<int>::Iterator i;
    i = tree.find(19);
    *i = 13;
    ASSERT_FALSE(tree.check_sorting_invariant());
}

TEST_MAIN()
