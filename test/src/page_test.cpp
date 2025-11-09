#include "database/page.hpp"
#include "database/tuple.hpp"
#include "database/schema.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(PageTest, CanCreatePage)
{
  database::PageId page_id = 1;
  size_t page_size = 8192;  // 8KB
  database::Page page(page_id, page_size);
  
  EXPECT_EQ(page.getPageId(), 1);
  EXPECT_GE(page.getFreeSpace(), page_size - sizeof(database::Page));  // Some space used by page header
}

TEST(PageTest, CanInsertTuple)
{
  database::PageId page_id = 1;
  database::Page page(page_id, 8192);
  
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  auto slot = page.insertTuple(tuple);
  ASSERT_NE(slot, nullptr);
  EXPECT_EQ(slot->first, page_id);
  EXPECT_GE(slot->second, 0);
}

TEST(PageTest, CanGetTuple)
{
  database::PageId page_id = 1;
  database::Page page(page_id, 8192);
  
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  auto slot = page.insertTuple(tuple);
  ASSERT_NE(slot, nullptr);
  
  auto retrieved = page.getTuple(*slot);
  ASSERT_NE(retrieved, nullptr);
  EXPECT_EQ(retrieved->getXmin(), 100);
  
  auto value = retrieved->getValue(0);
  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(std::get<int64_t>(value.value()), 42);
}

TEST(PageTest, CanDeleteTuple)
{
  database::PageId page_id = 1;
  database::Page page(page_id, 8192);
  
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  auto slot = page.insertTuple(tuple);
  ASSERT_NE(slot, nullptr);
  
  page.deleteTuple(*slot);
  
  auto retrieved = page.getTuple(*slot);
  EXPECT_EQ(retrieved, nullptr);  // Should be deleted
}

TEST(PageTest, CanUpdateTuple)
{
  database::PageId page_id = 1;
  database::Page page(page_id, 8192);
  
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  std::vector<database::Value> values1 = {database::Value{42LL}};
  database::Tuple tuple1(schema, values1, 100);
  
  auto slot = page.insertTuple(tuple1);
  ASSERT_NE(slot, nullptr);
  
  std::vector<database::Value> values2 = {database::Value{43LL}};
  database::Tuple tuple2(schema, values2, 200);
  
  bool updated = page.updateTuple(*slot, tuple2);
  EXPECT_TRUE(updated);
  
  auto retrieved = page.getTuple(*slot);
  ASSERT_NE(retrieved, nullptr);
  auto value = retrieved->getValue(0);
  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(std::get<int64_t>(value.value()), 43);
}

TEST(PageTest, HasFreeSpace)
{
  database::PageId page_id = 1;
  database::Page page(page_id, 8192);
  
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  size_t tuple_size = tuple.getSize();
  EXPECT_TRUE(page.hasFreeSpace(tuple_size));
}

TEST(PageTest, ReturnsFalseWhenNoFreeSpace)
{
  database::PageId page_id = 1;
  database::Page page(page_id, 100);  // Very small page
  
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  std::vector<database::Value> values = {database::Value{42LL}};
  database::Tuple tuple(schema, values, 100);
  
  // Try to insert a tuple larger than the page
  size_t huge_size = 10000;
  EXPECT_FALSE(page.hasFreeSpace(huge_size));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

