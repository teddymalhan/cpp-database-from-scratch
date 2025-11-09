#include "database/value.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(ValueTest, CanHoldInteger)
{
  database::Value v = database::Value{42};
  EXPECT_TRUE(std::holds_alternative<int64_t>(v));
  EXPECT_EQ(std::get<int64_t>(v), 42);
}

TEST(ValueTest, CanHoldDouble)
{
  database::Value v = database::Value{3.14};
  EXPECT_TRUE(std::holds_alternative<double>(v));
  EXPECT_DOUBLE_EQ(std::get<double>(v), 3.14);
}

TEST(ValueTest, CanHoldString)
{
  database::Value v = database::Value{std::string("hello")};
  EXPECT_TRUE(std::holds_alternative<std::string>(v));
  EXPECT_EQ(std::get<std::string>(v), "hello");
}

TEST(ValueTest, CanHoldBoolean)
{
  database::Value v = database::Value{true};
  EXPECT_TRUE(std::holds_alternative<bool>(v));
  EXPECT_EQ(std::get<bool>(v), true);
}

TEST(ValueTest, CanHoldNull)
{
  database::Value v = database::Value{std::nullptr_t{}};
  EXPECT_TRUE(std::holds_alternative<std::nullptr_t>(v));
}

TEST(ValueTest, CanCompareValues)
{
  database::Value v1 = database::Value{42};
  database::Value v2 = database::Value{42};
  database::Value v3 = database::Value{43};
  
  EXPECT_EQ(v1, v2);
  EXPECT_NE(v1, v3);
}

TEST(ValueTest, DifferentTypesAreNotEqual)
{
  database::Value v1 = database::Value{42};
  database::Value v2 = database::Value{std::string("42")};
  
  EXPECT_NE(v1, v2);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

