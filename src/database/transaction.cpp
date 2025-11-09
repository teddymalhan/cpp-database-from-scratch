#include "database/transaction.hpp"
#include <chrono>

namespace database {

Transaction::Transaction(TransactionId txn_id, IsolationLevel isolation_level)
    : txn_id_(txn_id),
      isolation_level_(isolation_level),
      state_(TransactionState::ACTIVE),
      start_time_(std::chrono::steady_clock::now()) {
}

void Transaction::addOperation(const Operation& op) {
  if (state_ == TransactionState::ACTIVE) {
    operations_.push_back(op);
  }
}

bool Transaction::commit() {
  if (state_ != TransactionState::ACTIVE) {
    return false;  // Already committed or aborted
  }
  
  state_ = TransactionState::IN_COMMIT;
  // In a real implementation, we'd perform commit operations here
  state_ = TransactionState::COMMITTED;
  
  return true;
}

void Transaction::rollback() {
  if (state_ == TransactionState::COMMITTED) {
    return;  // Cannot rollback committed transaction
  }
  
  if (state_ == TransactionState::ACTIVE || state_ == TransactionState::IN_COMMIT) {
    // In a real implementation, we'd undo operations here
    state_ = TransactionState::ABORTED;
  }
}

}  // namespace database

