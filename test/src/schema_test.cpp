#include "database/schema.hpp"

#include <gtest/gtest.h>
#include <string>

TEST(SchemaTest, CanCreateEmptySchema)
{
  database::Schema schema;
  EXPECT_EQ(schema.getColumnCount(), 0);
}

TEST(SchemaTest, CanAddColumn)
{
  database::Schema schema;
  database::Column col(0, "id", database::DataType::INTEGER, false, true);
  schema.addColumn(col);
  
  EXPECT_EQ(schema.getColumnCount(), 1);
  auto* retrieved = schema.getColumn(0);
  ASSERT_NE(retrieved, nullptr);
  EXPECT_EQ(retrieved->getName(), "id");
  EXPECT_EQ(retrieved->getDataType(), database::DataType::INTEGER);
  EXPECT_FALSE(retrieved->isNullable());
  EXPECT_TRUE(retrieved->isPrimaryKey());
}

TEST(SchemaTest, CanAddMultipleColumns)
{
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  schema.addColumn(database::Column(1, "name", database::DataType::TEXT, true, false));
  schema.addColumn(database::Column(2, "age", database::DataType::INTEGER, true, false));
  
  EXPECT_EQ(schema.getColumnCount(), 3);
  EXPECT_EQ(schema.getColumn(0)->getName(), "id");
  EXPECT_EQ(schema.getColumn(1)->getName(), "name");
  EXPECT_EQ(schema.getColumn(2)->getName(), "age");
}

TEST(SchemaTest, GetColumnReturnsNullForInvalidId)
{
  database::Schema schema;
  schema.addColumn(database::Column(0, "id", database::DataType::INTEGER, false, true));
  
  EXPECT_EQ(schema.getColumn(1), nullptr);
  EXPECT_EQ(schema.getColumn(999), nullptr);
}

TEST(ColumnTest, CanCreateColumn)
{
  database::Column col(0, "name", database::DataType::TEXT, true, false);
  
  EXPECT_EQ(col.getColumnId(), 0);
  EXPECT_EQ(col.getName(), "name");
  EXPECT_EQ(col.getDataType(), database::DataType::TEXT);
  EXPECT_TRUE(col.isNullable());
  EXPECT_FALSE(col.isPrimaryKey());
}

TEST(ColumnTest, PrimaryKeyColumn)
{
  database::Column col(0, "id", database::DataType::INTEGER, false, true);
  
  EXPECT_TRUE(col.isPrimaryKey());
  EXPECT_FALSE(col.isNullable());
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

