#include "database/heap_file.hpp"
#include "database/tuple.hpp"
#include "database/schema.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(HeapFileTest, CanCreateHeapFile)
{
  database::TableId table_id = 1;
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  database::HeapFile heap_file(table_id, schema);
  
  EXPECT_EQ(heap_file.getTableId(), 1);
  EXPECT_EQ(heap_file.getPageCount(), 0);
}

TEST(HeapFileTest, CanInsertTuple)
{
  database::TableId table_id = 1;
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  database::HeapFile heap_file(table_id, schema);
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  auto tuple_id = heap_file.insertTuple(tuple, 100);
  ASSERT_NE(tuple_id, nullptr);
  EXPECT_EQ(tuple_id->first, 1);  // First page
  EXPECT_GE(tuple_id->second, 0);  // Some slot
}

TEST(HeapFileTest, CanGetTuple)
{
  database::TableId table_id = 1;
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  database::HeapFile heap_file(table_id, schema);
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  auto tuple_id = heap_file.insertTuple(tuple, 100);
  ASSERT_NE(tuple_id, nullptr);
  
  auto retrieved = heap_file.getTuple(*tuple_id);
  ASSERT_NE(retrieved, nullptr);
  EXPECT_EQ(retrieved->getXmin(), 100);
  
  auto value = retrieved->getValue(0);
  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(std::get<int64_t>(value.value()), 42);
}

TEST(HeapFileTest, CanUpdateTuple)
{
  database::TableId table_id = 1;
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  database::HeapFile heap_file(table_id, schema);
  
  std::vector<database::Value> values1 = {database::Value{42LL}};
  database::Tuple tuple1(schema, values1, 100);
  
  auto tuple_id = heap_file.insertTuple(tuple1, 100);
  ASSERT_NE(tuple_id, nullptr);
  
  std::vector<database::Value> values2 = {database::Value{43LL}};
  database::Tuple tuple2(schema, values2, 200);
  
  auto updated_id = heap_file.updateTuple(*tuple_id, tuple2, 200);
  ASSERT_NE(updated_id, nullptr);
  
  auto retrieved = heap_file.getTuple(*updated_id);
  ASSERT_NE(retrieved, nullptr);
  auto value = retrieved->getValue(0);
  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(std::get<int64_t>(value.value()), 43);
}

TEST(HeapFileTest, CanDeleteTuple)
{
  database::TableId table_id = 1;
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  database::HeapFile heap_file(table_id, schema);
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  auto tuple_id = heap_file.insertTuple(tuple, 100);
  ASSERT_NE(tuple_id, nullptr);
  
  heap_file.deleteTuple(*tuple_id, 200);
  
  auto retrieved = heap_file.getTuple(*tuple_id);
  EXPECT_EQ(retrieved, nullptr);  // Should be deleted
}

TEST(HeapFileTest, CreatesNewPageWhenNeeded)
{
  database::TableId table_id = 1;
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  database::HeapFile heap_file(table_id, schema);
  
  // Insert many tuples to fill up first page
  for (int i = 0; i < 100; ++i) {
    std::vector<database::Value> values = {database::Value{static_cast<int64_t>(i)}};
    database::Tuple tuple(schema, values, 100);
    auto tuple_id = heap_file.insertTuple(tuple, 100);
    ASSERT_NE(tuple_id, nullptr);
  }
  
  // Should have created at least one page
  EXPECT_GT(heap_file.getPageCount(), 0);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

