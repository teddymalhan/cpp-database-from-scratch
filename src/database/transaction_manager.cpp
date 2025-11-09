#include "database/transaction_manager.hpp"

namespace database {

TransactionManager::TransactionManager()
    : next_txn_id_(1) {
}

TransactionId TransactionManager::beginTransaction(IsolationLevel isolation_level) {
  std::lock_guard<std::mutex> lock(mutex_);
  
  TransactionId txn_id = next_txn_id_++;
  auto txn = createTransaction(txn_id, isolation_level);
  active_transactions_[txn_id] = std::move(txn);
  
  return txn_id;
}

bool TransactionManager::commitTransaction(TransactionId txn_id) {
  std::lock_guard<std::mutex> lock(mutex_);
  
  auto it = active_transactions_.find(txn_id);
  if (it == active_transactions_.end()) {
    return false;  // Transaction not found
  }
  
  bool committed = it->second->commit();
  // Keep transaction in map even after commit (for testing/debugging)
  // In a real implementation, we might move it to a committed list
  
  return committed;
}

bool TransactionManager::rollbackTransaction(TransactionId txn_id) {
  std::lock_guard<std::mutex> lock(mutex_);
  
  auto it = active_transactions_.find(txn_id);
  if (it == active_transactions_.end()) {
    return false;  // Transaction not found
  }
  
  it->second->rollback();
  // Keep transaction in map even after rollback (for testing/debugging)
  // In a real implementation, we might move it to an aborted list
  
  return true;
}

Transaction* TransactionManager::getTransaction(TransactionId txn_id) {
  std::lock_guard<std::mutex> lock(mutex_);
  
  auto it = active_transactions_.find(txn_id);
  if (it == active_transactions_.end()) {
    return nullptr;
  }
  
  return it->second.get();
}

bool TransactionManager::isTransactionActive(TransactionId txn_id) const {
  std::lock_guard<std::mutex> lock(mutex_);
  
  auto it = active_transactions_.find(txn_id);
  return it != active_transactions_.end() && 
         it->second->getState() == TransactionState::ACTIVE;
}

std::vector<TransactionId> TransactionManager::getActiveTransactionIds() const {
  std::lock_guard<std::mutex> lock(mutex_);
  
  std::vector<TransactionId> active_ids;
  for (const auto& [txn_id, txn] : active_transactions_) {
    if (txn->getState() == TransactionState::ACTIVE) {
      active_ids.push_back(txn_id);
    }
  }
  
  return active_ids;
}

std::unique_ptr<Transaction> TransactionManager::createTransaction(TransactionId txn_id, IsolationLevel isolation_level) {
  return std::make_unique<Transaction>(txn_id, isolation_level);
}

}  // namespace database

