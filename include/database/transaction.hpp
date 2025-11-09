#ifndef DATABASE_TRANSACTION_HPP_
#define DATABASE_TRANSACTION_HPP_

#include "database/types.hpp"
#include <vector>
#include <chrono>

namespace database {

/**
 * @brief Isolation level enumeration
 */
enum class IsolationLevel {
  READ_UNCOMMITTED,
  READ_COMMITTED,
  REPEATABLE_READ,
  SERIALIZABLE
};

/**
 * @brief Transaction state enumeration (State pattern)
 */
enum class TransactionState {
  ACTIVE,
  COMMITTED,
  ABORTED,
  IN_COMMIT
};

/**
 * @brief Operation - represents an operation performed in a transaction
 * 
 * This is a placeholder for now. Will be expanded later with specific
 * operation types (INSERT, UPDATE, DELETE, etc.)
 */
struct Operation {
  // Will be expanded later
};

/**
 * @brief Transaction - represents a single database transaction
 * 
 * A transaction maintains:
 * - Transaction ID
 * - Isolation level
 * - State (ACTIVE, COMMITTED, ABORTED, IN_COMMIT)
 * - Operations performed
 * - Start time
 * 
 * Uses State pattern for transaction state management.
 */
class Transaction {
public:
  Transaction(TransactionId txn_id, IsolationLevel isolation_level);
  ~Transaction() = default;
  
  // Disable copy (transactions are unique)
  Transaction(const Transaction&) = delete;
  Transaction& operator=(const Transaction&) = delete;
  
  // Allow move
  Transaction(Transaction&&) = default;
  Transaction& operator=(Transaction&&) = default;
  
  [[nodiscard]] TransactionId getTransactionId() const noexcept { return txn_id_; }
  [[nodiscard]] IsolationLevel getIsolationLevel() const noexcept { return isolation_level_; }
  [[nodiscard]] TransactionState getState() const noexcept { return state_; }
  [[nodiscard]] const std::vector<Operation>& getOperations() const noexcept { return operations_; }
  [[nodiscard]] std::chrono::time_point<std::chrono::steady_clock> getStartTime() const noexcept { return start_time_; }
  
  /**
   * @brief Add an operation to the transaction
   */
  void addOperation(const Operation& op);
  
  /**
   * @brief Commit the transaction
   * @return true if successful, false if already committed/aborted
   */
  bool commit();
  
  /**
   * @brief Rollback the transaction
   */
  void rollback();

private:
  TransactionId txn_id_;
  IsolationLevel isolation_level_;
  TransactionState state_;
  std::vector<Operation> operations_;
  std::chrono::time_point<std::chrono::steady_clock> start_time_;
};

}  // namespace database

#endif  // DATABASE_TRANSACTION_HPP_

