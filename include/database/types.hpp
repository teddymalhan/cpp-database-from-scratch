#ifndef DATABASE_TYPES_HPP_
#define DATABASE_TYPES_HPP_

#include <cstdint>
#include <utility>

namespace database {

/**
 * @brief Transaction ID - unique identifier for each transaction
 * 
 * Transaction IDs are monotonically increasing and used in tuple headers
 * to track which transaction created or modified a tuple.
 */
using TransactionId = uint64_t;

/**
 * @brief Table ID - unique identifier for each table
 * 
 * Table IDs are assigned when tables are created and used to reference
 * tables throughout the database system.
 */
using TableId = uint32_t;

/**
 * @brief Page ID - unique identifier for each page
 * 
 * Pages are fixed-size storage units (typically 8KB) that contain tuples.
 * Page IDs are unique within a table.
 */
using PageId = uint64_t;

/**
 * @brief Tuple ID - unique identifier for a tuple
 * 
 * A tuple ID consists of a page ID and a slot number within that page.
 * This allows direct addressing of any tuple in the database.
 */
using TupleId = std::pair<PageId, uint16_t>;  // (page_id, slot_number)

/**
 * @brief Column ID - identifier for a column within a table
 * 
 * Column IDs are used to reference columns in a table's schema.
 */
using ColumnId = uint16_t;

}  // namespace database

#endif  // DATABASE_TYPES_HPP_

