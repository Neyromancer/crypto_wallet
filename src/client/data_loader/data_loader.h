/// \file data_loader.h
/// \brief Class responsible for loading data into DB.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.0
/// \date 25.02.2019

#ifndef CRYPTO_WALLET_CLIENT_DATA_LOADER_H_
#define CRYPTO_WALLET_CLIENT_DATA_LOADER_H_

extern "C" {
#include <sqlite3.h>
}

#include <cstdint>
#include <string>

/// \namespace crypto_wallet.
/// \brief Project namespace.
namespace crypto_wallet {
/// \namespace client
/// \brief Client namespace.
namespace client {
/// \class DataLoaderService data_loader_service.h.
/// \brief Class responsible for loading data into DB.
class DataLoader {
 public:
  /// \brief Class DataLoader copy constructor.
  /// \param[in] data_loader_ Class DataLoader object.
  //DataLoader(const DataLoader &data_loader) = delete;

  /// \brief Class DataLoader copy assignment.
  /// \param[in] data_loader Class DataLoader object.
  /// \return DataLoader object.
  //DataLoader &operator=(const DataLoader &data_loader) = delete;

  /// \brief DataLoader destructor.
  ~DataLoader() {
    sqlite3_close(database_);
  } 

  /// \brief Get instance of class DataLoader.
  /// \param[in] path Path to the DB.
  /// \return Instance of class DataLoader.
  inline static DataLoader &GetDataLoaderInstance(std::string &&path) 
  {
    static DataLoader data_loader{path};
    return data_loader;
  }

  /// \brief Get instance of class DataLoader.
  /// \param[in] path Path to the DB.
  /// \return Instance of class DataLoader.
  inline static DataLoader &GetDataLoaderInstance(const std::string &path) 
  {
    static DataLoader data_loader{path};
    return data_loader;
  }

  /// \brief Get DB.
  /// \return DB.
  inline sqlite3 &GetDataBase() const noexcept {
    return *database_;
  }
 
  /// \brief Create database table.
  void RunSqlScript();

  /// \Set database name.
  /// \param[in] db_name Database bame.
  void SetDataBaseName(const std::string &db_name);
  void SetDataBaseName(std::string &&db_name);

  /// \Get database name.
  /// \return Database name.
  std::string GetDataBaseName() const noexcept {
    return db_name_;
  }

  /// \brief Set database table name.
  /// \param[in] db_table_name Database table name.
  void SetDataBaseTableName(const std::string &db_table_name);

  /// \brief Get database table name.
  /// \return Database table name.
  std::string GetDataBaseTableName() const noexcept {
    return db_table_name_;
  }

  /// \brief Set SQL script.
  /// \param[in] sql_scrpt SQL script.
  void SetSqlScript(const std::string &sql_scrpt);

  /// \brief Get SQL script.
  /// \return SQL script.
  std::string GetSqlScript() const noexcept {
    return sql_script_;
  }

  /// \brief Check if database size limit reached.
  /// \return State of the check if database size limit reached.
  bool IsDataBaseSizeLimitReached();

 private:
  /// \brief DataLoader constructor.
  /// \param[in] path Path.
  DataLoader(std::string &&path);
  DataLoader(const std::string &path);

  /// \brief Class DataLoader move constructor.
  /// \param[in] data_loader Class DataLoader object.
  DataLoader(DataLoader &&data_loader) = default;

  /// \brief Class DataLoader move assignment.
  /// \param[in] data_loader Class DataLoader object.
  /// \return DataLoader object.
  DataLoader &operator=(DataLoader &&data_loader) = default;

  /// \brief Evaluate database size.
  /// \return Database size.
  uint32_t GetDataBaseSize();

  std::string db_name_{};
  sqlite3 *database_{nullptr};
  std::string db_table_name_{};
  std::string sql_script_{};
};
}  // namespace client
}  // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_DATA_LOADER_H_
