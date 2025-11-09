#include "database/transaction.hpp"

#include <gtest/gtest.h>

TEST(TransactionTest, CanCreateTransaction)
{
  database::TransactionId txn_id = 100;
  database::IsolationLevel level = database::IsolationLevel::READ_COMMITTED;
  
  database::Transaction txn(txn_id, level);
  
  EXPECT_EQ(txn.getTransactionId(), 100);
  EXPECT_EQ(txn.getIsolationLevel(), level);
  EXPECT_EQ(txn.getState(), database::TransactionState::ACTIVE);
}

TEST(TransactionTest, CanCommitTransaction)
{
  database::TransactionId txn_id = 100;
  database::Transaction txn(txn_id, database::IsolationLevel::READ_COMMITTED);
  
  bool committed = txn.commit();
  EXPECT_TRUE(committed);
  EXPECT_EQ(txn.getState(), database::TransactionState::COMMITTED);
}

TEST(TransactionTest, CanRollbackTransaction)
{
  database::TransactionId txn_id = 100;
  database::Transaction txn(txn_id, database::IsolationLevel::READ_COMMITTED);
  
  txn.rollback();
  EXPECT_EQ(txn.getState(), database::TransactionState::ABORTED);
}

TEST(TransactionTest, CannotCommitTwice)
{
  database::TransactionId txn_id = 100;
  database::Transaction txn(txn_id, database::IsolationLevel::READ_COMMITTED);
  
  bool committed1 = txn.commit();
  EXPECT_TRUE(committed1);
  
  bool committed2 = txn.commit();
  EXPECT_FALSE(committed2);  // Already committed
}

TEST(TransactionTest, CannotRollbackAfterCommit)
{
  database::TransactionId txn_id = 100;
  database::Transaction txn(txn_id, database::IsolationLevel::READ_COMMITTED);
  
  txn.commit();
  EXPECT_EQ(txn.getState(), database::TransactionState::COMMITTED);
  
  txn.rollback();  // Should be a no-op
  EXPECT_EQ(txn.getState(), database::TransactionState::COMMITTED);  // Still committed
}

TEST(TransactionTest, CanAddOperation)
{
  database::TransactionId txn_id = 100;
  database::Transaction txn(txn_id, database::IsolationLevel::READ_COMMITTED);
  
  // For now, we'll just test that we can add operations
  // The Operation type will be defined later
  // This is a placeholder test
  EXPECT_EQ(txn.getState(), database::TransactionState::ACTIVE);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

