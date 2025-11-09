#ifndef DATABASE_TRANSACTION_MANAGER_HPP_
#define DATABASE_TRANSACTION_MANAGER_HPP_

#include "database/types.hpp"
#include "database/transaction.hpp"
#include <map>
#include <memory>
#include <vector>
#include <atomic>
#include <mutex>

namespace database {

/**
 * @brief TransactionManager - manages all transactions
 * 
 * The TransactionManager is responsible for:
 * - Assigning unique transaction IDs (monotonically increasing)
 * - Creating and tracking active transactions
 * - Handling commit/rollback
 * - Providing transaction lookup
 * 
 * Uses Factory pattern for transaction creation.
 */
class TransactionManager {
public:
  TransactionManager();
  ~TransactionManager() = default;
  
  // Disable copy (transaction manager is unique)
  TransactionManager(const TransactionManager&) = delete;
  TransactionManager& operator=(const TransactionManager&) = delete;
  
  // Disable move (due to atomic and mutex members)
  TransactionManager(TransactionManager&&) = delete;
  TransactionManager& operator=(TransactionManager&&) = delete;
  
  /**
   * @brief Begin a new transaction
   * @return TransactionId for the new transaction
   */
  TransactionId beginTransaction(IsolationLevel isolation_level);
  
  /**
   * @brief Commit a transaction
   * @return true if successful, false otherwise
   */
  bool commitTransaction(TransactionId txn_id);
  
  /**
   * @brief Rollback a transaction
   * @return true if successful, false otherwise
   */
  bool rollbackTransaction(TransactionId txn_id);
  
  /**
   * @brief Get a transaction by ID
   * @return Pointer to Transaction if found, nullptr otherwise
   */
  Transaction* getTransaction(TransactionId txn_id);
  
  /**
   * @brief Check if a transaction is active
   */
  [[nodiscard]] bool isTransactionActive(TransactionId txn_id) const;
  
  /**
   * @brief Get all active transaction IDs
   */
  [[nodiscard]] std::vector<TransactionId> getActiveTransactionIds() const;

private:
  std::atomic<TransactionId> next_txn_id_;
  std::map<TransactionId, std::unique_ptr<Transaction>> active_transactions_;
  mutable std::mutex mutex_;  // For thread safety
  
  /**
   * @brief Create a new transaction (Factory pattern)
   */
  std::unique_ptr<Transaction> createTransaction(TransactionId txn_id, IsolationLevel isolation_level);
};

}  // namespace database

#endif  // DATABASE_TRANSACTION_MANAGER_HPP_

