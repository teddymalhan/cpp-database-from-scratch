#ifndef DATABASE_SCHEMA_HPP_
#define DATABASE_SCHEMA_HPP_

#include "database/types.hpp"
#include "database/value.hpp"
#include <string>
#include <vector>
#include <memory>

namespace database {

/**
 * @brief Data type enumeration
 */
enum class DataType {
  INTEGER,
  DOUBLE,
  TEXT,
  BOOLEAN
};

/**
 * @brief Column - represents a column in a table schema
 */
class Column {
public:
  Column(ColumnId column_id, const std::string& name, DataType data_type, 
         bool nullable, bool primary_key);
  
  [[nodiscard]] ColumnId getColumnId() const noexcept { return column_id_; }
  [[nodiscard]] const std::string& getName() const noexcept { return name_; }
  [[nodiscard]] DataType getDataType() const noexcept { return data_type_; }
  [[nodiscard]] bool isNullable() const noexcept { return nullable_; }
  [[nodiscard]] bool isPrimaryKey() const noexcept { return primary_key_; }

private:
  ColumnId column_id_;
  std::string name_;
  DataType data_type_;
  bool nullable_;
  bool primary_key_;
};

/**
 * @brief Schema - represents a table schema with columns
 */
class Schema {
public:
  Schema() = default;
  ~Schema() = default;
  
  // Disable copy and move for now (can be enabled later if needed)
  Schema(const Schema&) = delete;
  Schema& operator=(const Schema&) = delete;
  Schema(Schema&&) = default;
  Schema& operator=(Schema&&) = default;
  
  void addColumn(const Column& column);
  [[nodiscard]] const Column* getColumn(ColumnId column_id) const;
  [[nodiscard]] size_t getColumnCount() const noexcept { return columns_.size(); }

private:
  std::vector<Column> columns_;
};

}  // namespace database

#endif  // DATABASE_SCHEMA_HPP_

