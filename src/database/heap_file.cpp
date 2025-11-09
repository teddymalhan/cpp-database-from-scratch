#include "database/heap_file.hpp"
#include <algorithm>

namespace database {

constexpr size_t DEFAULT_PAGE_SIZE = 8192;  // 8KB

HeapFile::HeapFile(TableId table_id, const Schema& schema)
    : table_id_(table_id),
      schema_(schema),
      next_page_id_(1) {
}

std::unique_ptr<TupleId> HeapFile::insertTuple(const Tuple& tuple, TransactionId txn_id) {
  size_t required_size = tuple.getSize();
  
  // Find or create a page with enough space
  Page* page = findOrCreatePage(required_size);
  if (!page) {
    return nullptr;
  }
  
  // Insert tuple into page
  return page->insertTuple(tuple);
}

std::unique_ptr<TupleId> HeapFile::updateTuple(const TupleId& tuple_id, const Tuple& new_tuple, TransactionId txn_id) {
  // Get the page containing the tuple
  Page* page = getPage(tuple_id.first);
  if (!page) {
    return nullptr;
  }
  
  // Try to update in place first
  if (page->updateTuple(tuple_id, new_tuple)) {
    // Update successful, return same tuple_id
    return std::make_unique<TupleId>(tuple_id);
  }
  
  // Update failed (not enough space), create new version
  // For now, just insert as new tuple (proper version chaining will come later)
  return insertTuple(new_tuple, txn_id);
}

void HeapFile::deleteTuple(const TupleId& tuple_id, TransactionId txn_id) {
  // Get the page containing the tuple
  Page* page = getPage(tuple_id.first);
  if (!page) {
    return;
  }
  
  // Delete tuple from page
  page->deleteTuple(tuple_id);
}

const Tuple* HeapFile::getTuple(const TupleId& tuple_id) const {
  // Get the page containing the tuple
  const Page* page = getPage(tuple_id.first);
  if (!page) {
    return nullptr;
  }
  
  // Get tuple from page
  return page->getTuple(tuple_id);
}

std::vector<Tuple> HeapFile::getAllTuples() const {
  std::vector<Tuple> all_tuples;
  
  for (const auto& page : pages_) {
    // This is a simplified version - in a real implementation,
    // we'd need to iterate through all slots in each page
    // For now, this is just a placeholder
  }
  
  return all_tuples;
}

Page* HeapFile::findOrCreatePage(size_t required_size) {
  // Try to find an existing page with enough space
  for (auto& page : pages_) {
    if (page->hasFreeSpace(required_size)) {
      return page.get();
    }
  }
  
  // No page with enough space, create a new one
  PageId new_page_id = next_page_id_++;
  auto new_page = std::make_unique<Page>(new_page_id, DEFAULT_PAGE_SIZE);
  Page* page_ptr = new_page.get();
  pages_.push_back(std::move(new_page));
  
  return page_ptr;
}

Page* HeapFile::getPage(PageId page_id) const {
  for (const auto& page : pages_) {
    if (page->getPageId() == page_id) {
      return page.get();
    }
  }
  return nullptr;
}

}  // namespace database

