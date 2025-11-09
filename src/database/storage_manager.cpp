#include "database/storage_manager.hpp"

namespace database {

StorageManager::StorageManager()
    : next_table_id_(1) {
}

TableId StorageManager::createTable(const std::string& name, const Schema& schema) {
  TableId table_id = next_table_id_++;
  auto heap_file = std::make_unique<HeapFile>(table_id, schema);
  heap_files_[table_id] = std::move(heap_file);
  return table_id;
}

HeapFile* StorageManager::getTable(TableId table_id) {
  auto it = heap_files_.find(table_id);
  if (it == heap_files_.end()) {
    return nullptr;
  }
  return it->second.get();
}

}  // namespace database

