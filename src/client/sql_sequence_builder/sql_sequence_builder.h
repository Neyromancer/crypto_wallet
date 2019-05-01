/// \file sql_sequence_builder.h
/// \brief Class responsible for creating sql sequences.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.0
/// \data 01.05.2019

#ifndef CRYPTO_WALLET_CLIENT_SQL_SEQUENCE_BUILDER_H_
#define CRYPTO_WALLET_CLIENT_SQL_SEQUENCE_BUILDER_H_

#include <initializer_list>
#include <string>
#include <vector>

/// \namespace crypto_wallet.
/// \brief Project namespace
namespace crypto_wallet {

/// \namesapce client.
/// \brief Client namesapce
namespace client {

/// \class SQLSequenceBuilder sql_sequence_builder.h
/// \brief Class responsible for creating sql sequences.
class SQLSequenceBuilder {
 public:
  /// \brief Return SQL sequence.
  /// \return SQL sequence.
  std::string GetSQLSequence() const noexcept {
    return sql_sequence_;
  }

  /// \brief Build valid SQL sequence.
  void BuildSQLSequence() noexcept;

  /// \brief Set value to build sequence for.
  /// \param[in] columns Columns to build sequence from.
  void SetColumns(const std::initializer_list<std::string> &columns) noexcept;

  /// \brief Set table name.
  /// \param[in] table_name Table name.
  void SetTableName(const std::string &table_name) noexcept;

 private:
  /// \brief Create sequence of columns appropriate for sql scripts.
  /// \return Column sequence.
  std::string ColumnSequence() const noexcept;
  
  std::string sql_sequence_;
  std::vector<std::string> columns_;
  std::string table_name_;
};

} // namespace client
} // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_SQL_SEQUENCE_BUILDER_H_
