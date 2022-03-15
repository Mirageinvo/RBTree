#include <fstream>

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

TEST(RBTree, CopyAssignOperatorTest) {
  Trees::RBTree<size_t> tmp_1;
  for (size_t i = 1; i <= kNum; ++i) {
    tmp_1.insert(i);
    ASSERT_TRUE(tmp_1.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_1.num_of_less(i + 1) == i);
  }
  Trees::RBTree<size_t> tmp_2;
  tmp_2 = tmp_1;
  for (size_t i = 1; i <= kNum; ++i) {
    ASSERT_TRUE(tmp_2.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_2.num_of_less(i + 1) == i);
  }
}

TEST(RBTree, EndToEndTestFirst) {
  Trees::RBTree<int> tmp;
  char sign;
  int num, ans;
  std::ifstream in, in_ans;
  in.open("../../tests/files_for_tests/1.txt");
  in_ans.open("../../tests/files_for_tests/1_ans.txt");
  assert(in.is_open() && in_ans.is_open());
  if (in.is_open() && in_ans.is_open()) {
    while (!in.eof() && !in_ans.eof()) {
      in >> sign >> num;
      if (sign == 'k') {
        tmp.insert(num);
      } else if (sign == 'm') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.mth_statistic(static_cast<size_t>(num)) == ans);
      } else if (sign == 'n') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.num_of_less(num) == static_cast<size_t>(ans));
      } else {
        ASSERT_TRUE(false);
      }
    }
  }
  in.close();
  in_ans.close();
}

TEST(RBTree, EndToEndTestSecond) {
  Trees::RBTree<int> tmp;
  char sign;
  int num, ans;
  std::ifstream in, in_ans;
  in.open("../../tests/files_for_tests/2.txt");
  in_ans.open("../../tests/files_for_tests/2_ans.txt");
  assert(in.is_open() && in_ans.is_open());
  if (in.is_open() && in_ans.is_open()) {
    while (!in.eof() && !in_ans.eof()) {
      in >> sign >> num;
      if (sign == 'k') {
        tmp.insert(num);
      } else if (sign == 'm') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.mth_statistic(static_cast<size_t>(num)) == ans);
      } else if (sign == 'n') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.num_of_less(num) == static_cast<size_t>(ans));
      } else {
        ASSERT_TRUE(false);
      }
    }
  }
  in.close();
  in_ans.close();
}

TEST(RBTree, EndToEndTestThird) {
  Trees::RBTree<int> tmp;
  char sign;
  int num, ans;
  std::ifstream in, in_ans;
  in.open("../../tests/files_for_tests/3.txt");
  in_ans.open("../../tests/files_for_tests/3_ans.txt");
  assert(in.is_open() && in_ans.is_open());
  if (in.is_open() && in_ans.is_open()) {
    while (!in.eof() && !in_ans.eof()) {
      in >> sign >> num;
      if (sign == 'k') {
        tmp.insert(num);
      } else if (sign == 'm') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.mth_statistic(static_cast<size_t>(num)) == ans);
      } else if (sign == 'n') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.num_of_less(num) == static_cast<size_t>(ans));
      } else {
        ASSERT_TRUE(false);
      }
    }
  }
  in.close();
  in_ans.close();
}

TEST(RBTree, EndToEndTestFourth) {
  Trees::RBTree<int> tmp;
  char sign;
  int num, ans;
  std::ifstream in, in_ans;
  in.open("../../tests/files_for_tests/4.txt");
  in_ans.open("../../tests/files_for_tests/4_ans.txt");
  assert(in.is_open() && in_ans.is_open());
  if (in.is_open() && in_ans.is_open()) {
    while (!in.eof() && !in_ans.eof()) {
      in >> sign >> num;
      if (sign == 'k') {
        tmp.insert(num);
      } else if (sign == 'm') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.mth_statistic(static_cast<size_t>(num)) == ans);
      } else if (sign == 'n') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.num_of_less(num) == static_cast<size_t>(ans));
      } else {
        ASSERT_TRUE(false);
      }
    }
  }
  in.close();
  in_ans.close();
}

TEST(RBTree, EndToEndTestFifth) {
  Trees::RBTree<int> tmp;
  char sign;
  int num, ans;
  std::ifstream in, in_ans;
  in.open("../../tests/files_for_tests/5.txt");
  in_ans.open("../../tests/files_for_tests/5_ans.txt");
  assert(in.is_open() && in_ans.is_open());
  if (in.is_open() && in_ans.is_open()) {
    while (!in.eof() && !in_ans.eof()) {
      in >> sign >> num;
      if (sign == 'k') {
        tmp.insert(num);
      } else if (sign == 'm') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.mth_statistic(static_cast<size_t>(num)) == ans);
      } else if (sign == 'n') {
        in_ans >> ans;
        ASSERT_TRUE(tmp.num_of_less(num) == static_cast<size_t>(ans));
      } else {
        ASSERT_TRUE(false);
      }
    }
  }
  in.close();
  in_ans.close();
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}