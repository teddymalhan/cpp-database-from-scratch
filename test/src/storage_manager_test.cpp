#include "database/storage_manager.hpp"
#include "database/schema.hpp"

#include <gtest/gtest.h>

TEST(StorageManagerTest, CanCreateStorageManager)
{
  database::StorageManager storage;
  EXPECT_NE(&storage, nullptr);
}

TEST(StorageManagerTest, CanCreateTable)
{
  database::StorageManager storage;
  
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  schema.addColumn(database::Column(1, "name", database::DataType::TEXT, true, false));
  
  auto table_id = storage.createTable("users", schema);
  EXPECT_GT(table_id, 0);
}

TEST(StorageManagerTest, CanGetTable)
{
  database::StorageManager storage;
  
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  auto table_id = storage.createTable("users", schema);
  auto* table = storage.getTable(table_id);
  
  ASSERT_NE(table, nullptr);
  EXPECT_EQ(table->getTableId(), table_id);
}

TEST(StorageManagerTest, GetTableReturnsNullForInvalidId)
{
  database::StorageManager storage;
  
  auto* table = storage.getTable(999);
  EXPECT_EQ(table, nullptr);
}

TEST(StorageManagerTest, CanCreateMultipleTables)
{
  database::StorageManager storage;
  
  database::Schema schema1;
  schema1.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  database::Schema schema2;
  schema2.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  auto table_id1 = storage.createTable("users", schema1);
  auto table_id2 = storage.createTable("posts", schema2);
  
  EXPECT_NE(table_id1, table_id2);
  
  auto* table1 = storage.getTable(table_id1);
  auto* table2 = storage.getTable(table_id2);
  
  ASSERT_NE(table1, nullptr);
  ASSERT_NE(table2, nullptr);
  EXPECT_NE(table1, table2);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

