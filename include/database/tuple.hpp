#ifndef DATABASE_TUPLE_HPP_
#define DATABASE_TUPLE_HPP_

#include "database/types.hpp"
#include "database/value.hpp"
#include "database/schema.hpp"
#include <vector>
#include <optional>

namespace database {

/**
 * @brief TupleHeader - MVCC metadata for a tuple
 * 
 * Contains transaction IDs and version chaining information:
 * - xmin: Transaction ID that created this version
 * - xmax: Transaction ID that deleted/updated this version (0 if not set)
 * - ctid: Pointer to next version in chain (for version chaining)
 * - deleted: Flag indicating if tuple is deleted
 */
class TupleHeader {
public:
  explicit TupleHeader(TransactionId xmin);
  
  [[nodiscard]] TransactionId getXmin() const noexcept { return xmin_; }
  [[nodiscard]] TransactionId getXmax() const noexcept { return xmax_; }
  [[nodiscard]] TupleId getCtid() const noexcept { return ctid_; }
  [[nodiscard]] bool isDeleted() const noexcept { return deleted_; }
  
  void setXmax(TransactionId xmax) noexcept { xmax_ = xmax; }
  void setCtid(TupleId ctid) noexcept { ctid_ = ctid; }
  void setDeleted(bool deleted) noexcept { deleted_ = deleted; }

private:
  TransactionId xmin_;
  TransactionId xmax_;
  TupleId ctid_;  // Next version in chain (default: (0, 0))
  bool deleted_;
};

/**
 * @brief Tuple - represents a single row of data
 * 
 * A tuple contains:
 * - A schema defining its structure
 * - Values for each column
 * - A header with MVCC metadata
 */
class Tuple {
public:
  Tuple(const Schema& schema, const std::vector<Value>& values, TransactionId xmin);
  
  [[nodiscard]] size_t getColumnCount() const noexcept { return values_.size(); }
  [[nodiscard]] TransactionId getXmin() const noexcept { return header_.getXmin(); }
  [[nodiscard]] const TupleHeader& getHeader() const noexcept { return header_; }
  [[nodiscard]] TupleHeader& getHeader() noexcept { return header_; }
  
  [[nodiscard]] std::optional<Value> getValue(ColumnId column_id) const;
  [[nodiscard]] size_t getSize() const;  // Approximate size in bytes

private:
  const Schema& schema_;
  std::vector<Value> values_;
  TupleHeader header_;
};

}  // namespace database

#endif  // DATABASE_TUPLE_HPP_

