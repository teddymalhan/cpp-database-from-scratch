#include "database/tuple.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(TupleHeaderTest, CanCreateTupleHeader)
{
  database::TransactionId xmin = 100;
  database::TupleHeader header(xmin);
  
  EXPECT_EQ(header.getXmin(), 100);
  EXPECT_EQ(header.getXmax(), 0);  // Default to 0 (not set)
  EXPECT_FALSE(header.isDeleted());
}

TEST(TupleHeaderTest, CanSetXmax)
{
  database::TupleHeader header(100);
  header.setXmax(200);
  
  EXPECT_EQ(header.getXmax(), 200);
}

TEST(TupleHeaderTest, CanMarkAsDeleted)
{
  database::TupleHeader header(100);
  header.setDeleted(true);
  
  EXPECT_TRUE(header.isDeleted());
}

TEST(TupleHeaderTest, CanSetCtid)
{
  database::TupleHeader header(100);
  database::PageId page_id = 5;
  uint16_t slot = 10;
  database::TupleId next_tuple_id = std::make_pair(page_id, slot);
  header.setCtid(next_tuple_id);
  
  EXPECT_EQ(header.getCtid(), next_tuple_id);
}

TEST(TupleTest, CanCreateTuple)
{
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  schema.addColumn(database::Column(1, "name", database::DataType::TEXT, true, false));
  
  std::vector<database::Value> values = {database::Value{42LL}, database::Value{std::string("test")}};
  database::TransactionId xmin = 100;
  
  database::Tuple tuple(schema, values, xmin);
  
  EXPECT_EQ(tuple.getColumnCount(), 2);
  EXPECT_EQ(tuple.getXmin(), 100);
}

TEST(TupleTest, CanGetValue)
{
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  auto value = tuple.getValue(0);
  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(std::get<int64_t>(value.value()), 42);
}

TEST(TupleTest, GetValueReturnsNulloptForInvalidColumn)
{
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  auto value = tuple.getValue(999);
  EXPECT_FALSE(value.has_value());
}

TEST(TupleTest, CanGetSize)
{
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  schema.addColumn(database::Column(1, "name", database::DataType::TEXT, true, false));
  
  std::vector<database::Value> values = {database::Value{42LL}, database::Value{std::string("test")}};
  database::Tuple tuple(schema, values, 100);
  
  size_t size = tuple.getSize();
  EXPECT_GT(size, 0);  // Should have some size
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

