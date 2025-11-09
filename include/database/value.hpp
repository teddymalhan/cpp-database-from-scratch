#ifndef DATABASE_VALUE_HPP_
#define DATABASE_VALUE_HPP_

#include <cstdint>
#include <string>
#include <variant>

namespace database {

/**
 * @brief Value - variant type for different data types
 * 
 * Represents a value that can hold different data types:
 * - int64_t for integers
 * - double for floating point numbers
 * - std::string for text
 * - bool for boolean values
 * - nullptr_t for NULL values
 */
using Value = std::variant<int64_t, double, std::string, bool, std::nullptr_t>;

}  // namespace database

#endif  // DATABASE_VALUE_HPP_

