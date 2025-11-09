#include "database/types.hpp"

#include <gtest/gtest.h>
#include <cstdint>

TEST(TypesTest, TransactionIdIsUint64)
{
  database::TransactionId txn_id = 1;
  EXPECT_EQ(txn_id, 1ULL);
  EXPECT_GE(sizeof(txn_id), sizeof(uint64_t));
}

TEST(TypesTest, TableIdIsUint32)
{
  database::TableId table_id = 1;
  EXPECT_EQ(table_id, 1U);
  EXPECT_GE(sizeof(table_id), sizeof(uint32_t));
}

TEST(TypesTest, PageIdIsUint64)
{
  database::PageId page_id = 1;
  EXPECT_EQ(page_id, 1ULL);
  EXPECT_GE(sizeof(page_id), sizeof(uint64_t));
}

TEST(TypesTest, TupleIdIsPairOfPageIdAndSlot)
{
  database::PageId page_id = 5;
  uint16_t slot = 10;
  database::TupleId tuple_id = std::make_pair(page_id, slot);
  
  EXPECT_EQ(tuple_id.first, 5ULL);
  EXPECT_EQ(tuple_id.second, 10);
}

TEST(TypesTest, ColumnIdIsUint16)
{
  database::ColumnId column_id = 1;
  EXPECT_EQ(column_id, 1);
  EXPECT_GE(sizeof(column_id), sizeof(uint16_t));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

