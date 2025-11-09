#include "database/transaction_manager.hpp"

#include <gtest/gtest.h>

TEST(TransactionManagerTest, CanCreateTransactionManager)
{
  database::TransactionManager txn_manager;
  EXPECT_NE(&txn_manager, nullptr);
}

TEST(TransactionManagerTest, CanBeginTransaction)
{
  database::TransactionManager txn_manager;
  
  auto txn_id = txn_manager.beginTransaction(database::IsolationLevel::READ_COMMITTED);
  EXPECT_GT(txn_id, 0);
}

TEST(TransactionManagerTest, CanGetTransaction)
{
  database::TransactionManager txn_manager;
  
  auto txn_id = txn_manager.beginTransaction(database::IsolationLevel::READ_COMMITTED);
  auto* txn = txn_manager.getTransaction(txn_id);
  
  ASSERT_NE(txn, nullptr);
  EXPECT_EQ(txn->getTransactionId(), txn_id);
  EXPECT_EQ(txn->getIsolationLevel(), database::IsolationLevel::READ_COMMITTED);
}

TEST(TransactionManagerTest, CanCommitTransaction)
{
  database::TransactionManager txn_manager;
  
  auto txn_id = txn_manager.beginTransaction(database::IsolationLevel::READ_COMMITTED);
  bool committed = txn_manager.commitTransaction(txn_id);
  
  EXPECT_TRUE(committed);
  
  auto* txn = txn_manager.getTransaction(txn_id);
  ASSERT_NE(txn, nullptr);
  EXPECT_EQ(txn->getState(), database::TransactionState::COMMITTED);
}

TEST(TransactionManagerTest, CanRollbackTransaction)
{
  database::TransactionManager txn_manager;
  
  auto txn_id = txn_manager.beginTransaction(database::IsolationLevel::READ_COMMITTED);
  bool rolled_back = txn_manager.rollbackTransaction(txn_id);
  
  EXPECT_TRUE(rolled_back);
  
  auto* txn = txn_manager.getTransaction(txn_id);
  ASSERT_NE(txn, nullptr);
  EXPECT_EQ(txn->getState(), database::TransactionState::ABORTED);
}

TEST(TransactionManagerTest, IsTransactionActive)
{
  database::TransactionManager txn_manager;
  
  auto txn_id = txn_manager.beginTransaction(database::IsolationLevel::READ_COMMITTED);
  EXPECT_TRUE(txn_manager.isTransactionActive(txn_id));
  
  txn_manager.commitTransaction(txn_id);
  EXPECT_FALSE(txn_manager.isTransactionActive(txn_id));
}

TEST(TransactionManagerTest, GetActiveTransactionIds)
{
  database::TransactionManager txn_manager;
  
  auto txn_id1 = txn_manager.beginTransaction(database::IsolationLevel::READ_COMMITTED);
  auto txn_id2 = txn_manager.beginTransaction(database::IsolationLevel::REPEATABLE_READ);
  
  auto active_txns = txn_manager.getActiveTransactionIds();
  EXPECT_EQ(active_txns.size(), 2);
  
  txn_manager.commitTransaction(txn_id1);
  active_txns = txn_manager.getActiveTransactionIds();
  EXPECT_EQ(active_txns.size(), 1);
  EXPECT_EQ(active_txns[0], txn_id2);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

