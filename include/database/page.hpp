#ifndef DATABASE_PAGE_HPP_
#define DATABASE_PAGE_HPP_

#include "database/types.hpp"
#include "database/tuple.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

namespace database {

/**
 * @brief Page - fixed-size storage unit containing tuples
 * 
 * A page is a fixed-size storage unit (typically 8KB) that contains
 * multiple tuple slots. Pages manage tuple insertion, updates, and deletes
 * within their storage space.
 */
class Page {
public:
  Page(PageId page_id, size_t page_size);
  ~Page() = default;
  
  // Disable copy (pages are unique)
  Page(const Page&) = delete;
  Page& operator=(const Page&) = delete;
  
  // Allow move
  Page(Page&&) = default;
  Page& operator=(Page&&) = default;
  
  [[nodiscard]] PageId getPageId() const noexcept { return page_id_; }
  [[nodiscard]] size_t getFreeSpace() const noexcept { return free_space_; }
  
  /**
   * @brief Insert a tuple into the page
   * @return TupleId (page_id, slot_number) if successful, nullptr otherwise
   */
  std::unique_ptr<TupleId> insertTuple(const Tuple& tuple);
  
  /**
   * @brief Get a tuple from the page
   * @return Pointer to tuple if found, nullptr if deleted or not found
   */
  const Tuple* getTuple(const TupleId& tuple_id) const;
  
  /**
   * @brief Update a tuple in the page
   * @return true if successful, false otherwise
   */
  bool updateTuple(const TupleId& tuple_id, const Tuple& new_tuple);
  
  /**
   * @brief Delete a tuple from the page
   */
  void deleteTuple(const TupleId& tuple_id);
  
  /**
   * @brief Check if page has enough free space for a tuple
   */
  [[nodiscard]] bool hasFreeSpace(size_t required_size) const noexcept;

private:
  PageId page_id_;
  size_t page_size_;
  size_t free_space_;
  
  // Store tuples by slot number
  std::unordered_map<uint16_t, std::unique_ptr<Tuple>> slots_;
  uint16_t next_slot_;
  
  size_t calculateSlotSize(const Tuple& tuple) const;
};

}  // namespace database

#endif  // DATABASE_PAGE_HPP_

