#include "database/tuple.hpp"

namespace database {

TupleHeader::TupleHeader(TransactionId xmin)
    : xmin_(xmin),
      xmax_(0),
      ctid_(std::make_pair(0, 0)),
      deleted_(false) {
}

Tuple::Tuple(const Schema& schema, const std::vector<Value>& values, TransactionId xmin)
    : schema_(schema),
      values_(values),
      header_(xmin) {
}

std::optional<Value> Tuple::getValue(ColumnId column_id) const {
  if (column_id >= values_.size()) {
    return std::nullopt;
  }
  return values_[column_id];
}

size_t Tuple::getSize() const {
  size_t size = sizeof(TupleHeader);
  
  for (const auto& value : values_) {
    size += sizeof(Value);  // Base variant size
    
    // Add size for string values
    if (std::holds_alternative<std::string>(value)) {
      size += std::get<std::string>(value).size();
    }
  }
  
  return size;
}

}  // namespace database

