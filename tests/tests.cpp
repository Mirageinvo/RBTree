#include "gtest/gtest.h"
#include "rbtree_impl.hpp"

const size_t kNum = 1e5;

TEST(RBTree, InsertNumOfLessMthStatisticTest) {
    Trees::RBTree<size_t> tmp_1;
    for (size_t i = 1; i <= kNum; ++i) {
        tmp_1.insert(i);
        ASSERT_TRUE(tmp_1.mth_statistic(i) == i);
        ASSERT_TRUE(tmp_1.num_of_less(i + 1) == i);
    }
    Trees::RBTree<size_t> tmp_2;
    for (size_t i = kNum; i >= 1; --i) {
        tmp_2.insert(i);
        ASSERT_TRUE(tmp_2.mth_statistic(1) == i);
        ASSERT_TRUE(tmp_2.num_of_less(kNum + 1) == kNum - i + 1);
    }
}

TEST(RBTree, CopyConstructorTest) {
    auto tmp_1 = new Trees::RBTree<size_t>;
    for (size_t i = 1; i <= kNum; ++i) {
        tmp_1->insert(i);
        ASSERT_TRUE(tmp_1->mth_statistic(i) == i);
        ASSERT_TRUE(tmp_1->num_of_less(i + 1) == i);
    }
    Trees::RBTree<size_t> tmp_2(*tmp_1);
    delete tmp_1;
    for (size_t i = 1; i <= kNum; ++i) {
        ASSERT_TRUE(tmp_2.mth_statistic(i) == i);
        ASSERT_TRUE(tmp_2.num_of_less(i + 1) == i);
    }
}

TEST(RBTree, MoveConstructorTest) {
    Trees::RBTree<size_t> tmp_1;
    for (size_t i = 1; i <= kNum; ++i) {
        tmp_1.insert(i);
        ASSERT_TRUE(tmp_1.mth_statistic(i) == i);
        ASSERT_TRUE(tmp_1.num_of_less(i + 1) == i);
    }
    Trees::RBTree<size_t> tmp_2(std::move(tmp_1));
    for (size_t i = 1; i <= kNum; ++i) {
        ASSERT_TRUE(tmp_2.mth_statistic(i) == i);
        ASSERT_TRUE(tmp_2.num_of_less(i + 1) == i);
    }
}

TEST(RBTree, MoveAssignOperatorTest) {
    Trees::RBTree<size_t> tmp_1;
    for (size_t i = 1; i <= kNum; ++i) {
        tmp_1.insert(i);
        ASSERT_TRUE(tmp_1.mth_statistic(i) == i);
        ASSERT_TRUE(tmp_1.num_of_less(i + 1) == i);
    }
    Trees::RBTree<size_t> tmp_2;
    tmp_2 = std::move(tmp_1);
    for (size_t i = 1; i <= kNum; ++i) {
        ASSERT_TRUE(tmp_2.mth_statistic(i) == i);
        ASSERT_TRUE(tmp_2.num_of_less(i + 1) == i);
    }
}
















int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}