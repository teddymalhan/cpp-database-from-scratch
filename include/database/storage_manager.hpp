#ifndef DATABASE_STORAGE_MANAGER_HPP_
#define DATABASE_STORAGE_MANAGER_HPP_

#include "database/types.hpp"
#include "database/schema.hpp"
#include "database/heap_file.hpp"
#include <string>
#include <map>
#include <memory>

namespace database {

/**
 * @brief IStorageManager - interface for storage management (Dependency Inversion)
 */
class IStorageManager {
public:
  virtual ~IStorageManager() = default;
  virtual TableId createTable(const std::string& name, const Schema& schema) = 0;
  virtual HeapFile* getTable(TableId table_id) = 0;
};

/**
 * @brief StorageManager - manages all heap files (tables)
 * 
 * The StorageManager is responsible for:
 * - Creating and managing tables (heap files)
 * - Providing access to tables by ID
 * - Allocating table IDs
 */
class StorageManager : public IStorageManager {
public:
  StorageManager();
  ~StorageManager() = default;
  
  // Disable copy (storage manager is unique)
  StorageManager(const StorageManager&) = delete;
  StorageManager& operator=(const StorageManager&) = delete;
  
  // Allow move
  StorageManager(StorageManager&&) = default;
  StorageManager& operator=(StorageManager&&) = default;
  
  /**
   * @brief Create a new table
   * @return TableId for the created table
   */
  TableId createTable(const std::string& name, const Schema& schema) override;
  
  /**
   * @brief Get a table by ID
   * @return Pointer to HeapFile if found, nullptr otherwise
   */
  HeapFile* getTable(TableId table_id) override;

private:
  std::map<TableId, std::unique_ptr<HeapFile>> heap_files_;
  TableId next_table_id_;
};

}  // namespace database

#endif  // DATABASE_STORAGE_MANAGER_HPP_

