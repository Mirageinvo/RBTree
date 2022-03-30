#include <fstream>

#include "gtest/gtest.h"
#include "rbtree_impl.hpp"

const size_t kNum = 1e5;
const size_t kNumSmall = 5;

struct BadType {
  BadType() : val(0) {
    if (counter % 15 == 0) {
      throw std::runtime_error("Just an error");
    }
    ++counter;
  }
  BadType(size_t num) : val(num) {
    if (counter % 15 == 0) {
      throw std::runtime_error("Just an error");
    }
    ++counter;
  }
  BadType(const BadType& another) : val(another.val) {
    if (counter % 15 == 0) {
      throw std::runtime_error("Just an error");
    }
    ++counter;
  }
  size_t val;
  static size_t counter;
  bool operator>(const BadType& another) {
    ++counter;
    return val > another.val;
  }
  bool operator<(const BadType& another) {
    ++counter;
    return val < another.val;
  }
  bool operator==(const BadType& another) {
    ++counter;
    return val == another.val;
  }
  bool operator!=(const BadType& another) {
    ++counter;
    return !(*this == another);
  }
  BadType& operator=(const BadType& another) {
    if (this == &another) {
      return *this;
    }
    if (counter % 15 == 0) {
      throw std::runtime_error("Just an error");
    } else {
      ++counter;
      val = another.val;
    }
    return *this;
  }
};
size_t BadType::counter = 1;

TEST(RBTree, ExceptionsTest) {
  trees::RBTree<BadType> tmp_1;
  for (size_t i = 1; i <= kNum; ++i) {
    try {
      tmp_1.insert(BadType(i));
      ASSERT_TRUE(tmp_1.mth_statistic(i) == BadType(i));
      ASSERT_TRUE(tmp_1.num_of_less(BadType(i + 1)) == i);
    } catch (const std::runtime_error& e) {
      EXPECT_EQ(static_cast<std::string>(e.what()), "Just an error");
      break;
    } catch (...) {
      FAIL();
    }
  }
  trees::RBTree<BadType> tmp_2;
  for (size_t i = kNum; i >= 1; --i) {
    try {
      tmp_2.insert(i);
      ASSERT_TRUE(tmp_2.mth_statistic(1).val == i);
      ASSERT_TRUE(tmp_2.num_of_less(BadType(kNum + 1)) == kNum - i + 1);
    } catch (const std::runtime_error& e) {
      EXPECT_EQ(static_cast<std::string>(e.what()), "Just an error");
      break;
    } catch (...) {
      FAIL();
    }
  }
  trees::RBTree<BadType> tmp_3;
  for (size_t i = 0; i < kNumSmall; ++i) {
    try {
      BadType::counter = 1;
      tmp_3.insert(i);
    } catch (...) {
      FAIL();
    }
  }
  BadType::counter += 7;
  try {
    trees::RBTree<BadType> tmp_4(tmp_3);
  } catch (const std::runtime_error& e) {
    EXPECT_EQ(static_cast<std::string>(e.what()), "Just an error");
  } catch (...) {
    FAIL();
  }
}

TEST(RBTreeNode, DefaultAndCopyConstructorTest) {
  trees::RBTree<size_t>::node tmp;
  ASSERT_TRUE(tmp.color == trees::node_color::BLACK && tmp.num_of_less == 0 &&
              tmp.num_of_greater == 0);
  ASSERT_TRUE(tmp.left == nullptr && tmp.right == nullptr &&
              tmp.parent == nullptr && tmp.data == nullptr);
  tmp.data = new size_t;
  for (size_t i = 0; i <= kNum; ++i) {
    tmp.num_of_less = i;
    tmp.num_of_greater = i;
    ASSERT_TRUE(tmp.data);
    *tmp.data = i + 1;
    trees::RBTree<size_t>::node tmp_2 = tmp;
    ASSERT_TRUE(tmp.color == tmp_2.color &&
                tmp.num_of_less == tmp_2.num_of_less &&
                tmp.num_of_greater == tmp_2.num_of_greater);
    ASSERT_TRUE(tmp.left == tmp_2.left && tmp.right == tmp_2.right &&
                tmp.parent == tmp_2.parent && tmp.data && tmp_2.data);
    ASSERT_TRUE(*tmp.data == *tmp_2.data && tmp.data != tmp_2.data);
  }
}

TEST(RBTreeNode, AnotherConstructorFirstTest) {
  for (size_t i = 0; i <= kNum; ++i) {
    trees::RBTree<size_t>::node tmp(i, nullptr, nullptr, static_cast<int>(i));
    ASSERT_TRUE(tmp.data);
    ASSERT_TRUE(tmp.color == static_cast<int>(i) && tmp.num_of_less == 0 &&
                tmp.num_of_greater == 0);
    ASSERT_TRUE(tmp.left == nullptr && tmp.right == nullptr &&
                tmp.parent == nullptr && tmp.data);
    ASSERT_TRUE(*tmp.data == i);
  }
}

TEST(RBTreeNode, AnotherConstructorSecondTest) {
  for (size_t i = 0; i <= kNum; ++i) {
    trees::RBTree<size_t>::node tmp(i, nullptr, nullptr, static_cast<int>(i));
    trees::RBTree<size_t>::node tmp_2(&tmp, nullptr, nullptr);
    ASSERT_TRUE(tmp_2.data);
    ASSERT_TRUE(tmp_2.color == static_cast<int>(i) &&
                tmp_2.num_of_less == tmp.num_of_less &&
                tmp_2.num_of_greater == tmp.num_of_less);
    ASSERT_TRUE(tmp_2.left == nullptr && tmp_2.right == nullptr &&
                tmp_2.parent == nullptr && tmp.data && tmp_2.data);
    ASSERT_TRUE(*tmp.data == *tmp_2.data && tmp.data != tmp_2.data);
  }
}

TEST(RBTreeNode, MoveConstructorTest) {
  for (size_t i = 0; i <= kNum; ++i) {
    trees::RBTree<size_t>::node tmp(i, nullptr, nullptr, static_cast<int>(i));
    trees::RBTree<size_t>::node tmp_2 = std::move(tmp);
    ASSERT_TRUE(tmp_2.data);
    ASSERT_TRUE(tmp_2.color == static_cast<int>(i) && tmp_2.num_of_less == 0 &&
                tmp_2.num_of_greater == 0);
    ASSERT_TRUE(tmp_2.left == nullptr && tmp_2.right == nullptr &&
                tmp_2.parent == nullptr && tmp_2.data && tmp.data == nullptr);
    ASSERT_TRUE(*tmp_2.data == i);
  }
}

TEST(RBTreeNode, CopyAssignmentTest) {
  trees::RBTree<size_t>::node tmp;
  trees::RBTree<size_t>::node tmp_2;
  ASSERT_TRUE(tmp.color == trees::node_color::BLACK && tmp.num_of_less == 0 &&
              tmp.num_of_greater == 0);
  ASSERT_TRUE(tmp.left == nullptr && tmp.right == nullptr &&
              tmp.parent == nullptr && tmp.data == nullptr);
  tmp.data = new size_t;
  for (size_t i = 0; i <= kNum; ++i) {
    tmp.num_of_less = i;
    tmp.num_of_greater = i;
    ASSERT_TRUE(tmp.data);
    *tmp.data = i + 1;
    tmp_2 = tmp;
    ASSERT_TRUE(tmp.color == tmp_2.color &&
                tmp.num_of_less == tmp_2.num_of_less &&
                tmp.num_of_greater == tmp_2.num_of_greater);
    ASSERT_TRUE(tmp.left == tmp_2.left && tmp.right == tmp_2.right &&
                tmp.parent == tmp_2.parent && tmp.data && tmp_2.data);
    ASSERT_TRUE(*tmp.data == *tmp_2.data && tmp.data != tmp_2.data);
    delete tmp_2.data;
    tmp_2.data = nullptr;
  }
}

TEST(RBTreeNode, MoveAssignmentTest) {
  trees::RBTree<size_t>::node tmp;
  trees::RBTree<size_t>::node tmp_2;
  ASSERT_TRUE(tmp.color == trees::node_color::BLACK && tmp.num_of_less == 0 &&
              tmp.num_of_greater == 0);
  ASSERT_TRUE(tmp.left == nullptr && tmp.right == nullptr &&
              tmp.parent == nullptr && tmp.data == nullptr);
  for (size_t i = 0; i <= kNum; ++i) {
    tmp.data = new size_t;
    tmp.num_of_less = i;
    tmp.num_of_greater = i;
    ASSERT_TRUE(tmp.data);
    *tmp.data = i + 1;
    tmp_2 = std::move(tmp);
    ASSERT_TRUE(tmp_2.color == trees::node_color::BLACK &&
                tmp_2.num_of_less == i && tmp_2.num_of_greater == i);
    ASSERT_TRUE(tmp.left == nullptr && tmp_2.left == nullptr &&
                tmp.right == nullptr && tmp_2.right == nullptr);
    ASSERT_TRUE(tmp.parent == nullptr && tmp_2.parent == nullptr &&
                tmp.data == nullptr && tmp_2.data);
    ASSERT_TRUE(*tmp_2.data == i + 1);
    delete tmp_2.data;
    tmp_2.data = nullptr;
  }
}

TEST(RBTree, InsertNumOfLessMthStatisticTest) {
  trees::RBTree<size_t> tmp_1;
  for (size_t i = 1; i <= kNum; ++i) {
    tmp_1.insert(i);
    ASSERT_TRUE(tmp_1.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_1.num_of_less(i + 1) == i);
  }
  trees::RBTree<size_t> tmp_2;
  for (size_t i = kNum; i >= 1; --i) {
    tmp_2.insert(i);
    ASSERT_TRUE(tmp_2.mth_statistic(1) == i);
    ASSERT_TRUE(tmp_2.num_of_less(kNum + 1) == kNum - i + 1);
  }
}

TEST(RBTree, CopyConstructorTest) {
  auto tmp_1 = new trees::RBTree<size_t>;
  for (size_t i = 1; i <= kNum; ++i) {
    tmp_1->insert(i);
    ASSERT_TRUE(tmp_1->mth_statistic(i) == i);
    ASSERT_TRUE(tmp_1->num_of_less(i + 1) == i);
  }
  trees::RBTree<size_t> tmp_2(*tmp_1);
  delete tmp_1;
  for (size_t i = 1; i <= kNum; ++i) {
    ASSERT_TRUE(tmp_2.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_2.num_of_less(i + 1) == i);
  }
}

TEST(RBTree, MoveConstructorTest) {
  trees::RBTree<size_t> tmp_1;
  for (size_t i = 1; i <= kNum; ++i) {
    tmp_1.insert(i);
    ASSERT_TRUE(tmp_1.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_1.num_of_less(i + 1) == i);
  }
  trees::RBTree<size_t> tmp_2(std::move(tmp_1));
  for (size_t i = 1; i <= kNum; ++i) {
    ASSERT_TRUE(tmp_2.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_2.num_of_less(i + 1) == i);
  }
}

TEST(RBTree, MoveAssignOperatorTest) {
  trees::RBTree<size_t> tmp_1;
  for (size_t i = 1; i <= kNum; ++i) {
    tmp_1.insert(i);
    ASSERT_TRUE(tmp_1.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_1.num_of_less(i + 1) == i);
  }
  trees::RBTree<size_t> tmp_2;
  tmp_2 = std::move(tmp_1);
  for (size_t i = 1; i <= kNum; ++i) {
    ASSERT_TRUE(tmp_2.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_2.num_of_less(i + 1) == i);
  }
}

TEST(RBTree, CopyAssignOperatorTest) {
  trees::RBTree<size_t> tmp_1;
  for (size_t i = 1; i <= kNum; ++i) {
    tmp_1.insert(i);
    ASSERT_TRUE(tmp_1.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_1.num_of_less(i + 1) == i);
  }
  trees::RBTree<size_t> tmp_2;
  tmp_2 = tmp_1;
  for (size_t i = 1; i <= kNum; ++i) {
    ASSERT_TRUE(tmp_2.mth_statistic(i) == i);
    ASSERT_TRUE(tmp_2.num_of_less(i + 1) == i);
  }
}

TEST(RBTree, EndToEndTestFirst) {
  trees::RBTree<int> tmp;
  char sign;
  int num;
  int ans;
  std::ifstream in;
  std::ifstream in_ans;
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
  trees::RBTree<int> tmp;
  char sign;
  int num;
  int ans;
  std::ifstream in;
  std::ifstream in_ans;
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
  trees::RBTree<int> tmp;
  char sign;
  int num;
  int ans;
  std::ifstream in;
  std::ifstream in_ans;
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
  trees::RBTree<int> tmp;
  char sign;
  int num;
  int ans;
  std::ifstream in;
  std::ifstream in_ans;
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
  trees::RBTree<int> tmp;
  char sign;
  int num;
  int ans;
  std::ifstream in;
  std::ifstream in_ans;
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