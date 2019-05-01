#include "sql_sequence_builder.h"

#include <boost/format.hpp>

namespace crypto_wallet {

namespace client {

void SQLSequenceBuilder::BuildSQLSequence() noexcept {
  std::string sql_template = "SELECT %1% FROM %2%;";
  sql_sequence_ = boost::str(boost::format{sql_template} %
                             ColumnSequence() % table_name_);
}

void SQLSequenceBuilder::SetColumns(const std::initializer_list<std::string> &columns) noexcept {
  for (const auto &el : columns)
    columns_.push_back(el);
}

void SQLSequenceBuilder::SetTableName(const std::string &table_name) noexcept {
  table_name_ = table_name;
}

std::string SQLSequenceBuilder::ColumnSequence() const noexcept {
  std::string result;
  auto size = columns_.size();
  for (std::size_t i = 0; i < size; ++i) {
    result += columns_.at(i);
    if (i < size - 1)
      result += ", ";
  }

  return result;
}

} // namespace client

} // namespace crypto_wallet
