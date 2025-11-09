#include "database/schema.hpp"

namespace database {

Column::Column(ColumnId column_id, const std::string& name, DataType data_type,
               bool nullable, bool primary_key)
    : column_id_(column_id),
      name_(name),
      data_type_(data_type),
      nullable_(nullable),
      primary_key_(primary_key) {
}

void Schema::addColumn(const Column& column) {
  columns_.push_back(column);
}

const Column* Schema::getColumn(ColumnId column_id) const {
  for (const auto& col : columns_) {
    if (col.getColumnId() == column_id) {
      return &col;
    }
  }
  return nullptr;
}

}  // namespace database

