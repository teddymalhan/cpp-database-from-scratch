#ifndef DATABASE_HEAP_FILE_HPP_
#define DATABASE_HEAP_FILE_HPP_

#include "database/types.hpp"
#include "database/tuple.hpp"
#include "database/schema.hpp"
#include "database/page.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

namespace database {

/**
 * @brief HeapFile - represents a single table
 * 
 * A heap file manages pages containing tuples. It handles:
 * - Tuple insertion, updates, and deletes
 * - Page allocation and management
 * - Tuple version chains (for MVCC)
 */
class HeapFile {
public:
  HeapFile(TableId table_id, const Schema& schema);
  ~HeapFile() = default;
  
  // Disable copy (heap files are unique)
  HeapFile(const HeapFile&) = delete;
  HeapFile& operator=(const HeapFile&) = delete;
  
  // Allow move (but not move assignment due to const reference member)
  HeapFile(HeapFile&&) = default;
  HeapFile& operator=(HeapFile&&) = delete;  // Cannot move assign due to const reference
  
  [[nodiscard]] TableId getTableId() const noexcept { return table_id_; }
  [[nodiscard]] const Schema& getSchema() const noexcept { return schema_; }
  [[nodiscard]] size_t getPageCount() const noexcept { return pages_.size(); }
  
  /**
   * @brief Insert a tuple into the heap file
   * @return TupleId if successful, nullptr otherwise
   */
  std::unique_ptr<TupleId> insertTuple(const Tuple& tuple, TransactionId txn_id);
  
  /**
   * @brief Update a tuple in the heap file
   * @return New TupleId if successful (for version chaining), nullptr otherwise
   */
  std::unique_ptr<TupleId> updateTuple(const TupleId& tuple_id, const Tuple& new_tuple, TransactionId txn_id);
  
  /**
   * @brief Delete a tuple from the heap file
   */
  void deleteTuple(const TupleId& tuple_id, TransactionId txn_id);
  
  /**
   * @brief Get a tuple from the heap file
   * @return Pointer to tuple if found, nullptr if deleted or not found
   */
  const Tuple* getTuple(const TupleId& tuple_id) const;
  
  /**
   * @brief Get all tuples (for testing/debugging)
   */
  std::vector<Tuple> getAllTuples() const;

private:
  TableId table_id_;
  const Schema& schema_;
  std::vector<std::unique_ptr<Page>> pages_;
  PageId next_page_id_;
  
  /**
   * @brief Find or create a page with enough free space
   */
  Page* findOrCreatePage(size_t required_size);
  
  /**
   * @brief Get the page containing a tuple
   */
  Page* getPage(PageId page_id) const;
};

}  // namespace database

#endif  // DATABASE_HEAP_FILE_HPP_

