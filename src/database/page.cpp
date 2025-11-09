#include "database/page.hpp"
#include <algorithm>
#include <cstring>

namespace database {

Page::Page(PageId page_id, size_t page_size)
    : page_id_(page_id),
      page_size_(page_size),
      free_space_(page_size - sizeof(Page)),  // Approximate: subtract page overhead
      next_slot_(0) {
}

std::unique_ptr<TupleId> Page::insertTuple(const Tuple& tuple) {
  size_t required_size = calculateSlotSize(tuple);
  
  if (!hasFreeSpace(required_size)) {
    return nullptr;
  }
  
  uint16_t slot = next_slot_++;
  auto tuple_copy = std::make_unique<Tuple>(tuple);
  slots_[slot] = std::move(tuple_copy);
  
  free_space_ -= required_size;
  
  return std::make_unique<TupleId>(std::make_pair(page_id_, slot));
}

const Tuple* Page::getTuple(const TupleId& tuple_id) const {
  // Verify this tuple belongs to this page
  if (tuple_id.first != page_id_) {
    return nullptr;
  }
  
  uint16_t slot = tuple_id.second;
  auto it = slots_.find(slot);
  if (it == slots_.end()) {
    return nullptr;
  }
  
  // Check if tuple is deleted
  if (it->second->getHeader().isDeleted()) {
    return nullptr;
  }
  
  return it->second.get();
}

bool Page::updateTuple(const TupleId& tuple_id, const Tuple& new_tuple) {
  // Verify this tuple belongs to this page
  if (tuple_id.first != page_id_) {
    return false;
  }
  
  uint16_t slot = tuple_id.second;
  auto it = slots_.find(slot);
  if (it == slots_.end()) {
    return false;
  }
  
  // Check if tuple is deleted
  if (it->second->getHeader().isDeleted()) {
    return false;
  }
  
  // Calculate size difference
  size_t old_size = calculateSlotSize(*it->second);
  size_t new_size = calculateSlotSize(new_tuple);
  size_t size_diff = new_size - old_size;
  
  if (size_diff > 0 && !hasFreeSpace(size_diff)) {
    return false;  // Not enough space for update
  }
  
  // Update the tuple by replacing it
  it->second = std::make_unique<Tuple>(new_tuple);
  free_space_ -= size_diff;
  
  return true;
}

void Page::deleteTuple(const TupleId& tuple_id) {
  // Verify this tuple belongs to this page
  if (tuple_id.first != page_id_) {
    return;
  }
  
  uint16_t slot = tuple_id.second;
  auto it = slots_.find(slot);
  if (it == slots_.end()) {
    return;
  }
  
  // Mark as deleted
  it->second->getHeader().setDeleted(true);
  
  // Reclaim space (we could do this more efficiently, but for now just mark as deleted)
  // In a real implementation, we might want to actually remove it or mark the slot as free
}

bool Page::hasFreeSpace(size_t required_size) const noexcept {
  return free_space_ >= required_size;
}

size_t Page::calculateSlotSize(const Tuple& tuple) const {
  // Approximate size: tuple size + some overhead for slot management
  return tuple.getSize() + sizeof(uint16_t) + sizeof(void*);  // slot number + pointer overhead
}

}  // namespace database

