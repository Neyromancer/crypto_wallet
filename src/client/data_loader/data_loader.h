/// \file data_loader.h
/// \brief Class responsible for loading data into DB.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.5
/// \date 11.04.2019

#ifndef CRYPTO_WALLET_CLIENT_DATA_LOADER_H_
#define CRYPTO_WALLET_CLIENT_DATA_LOADER_H_

extern "C" {
#include <sqlite3.h>
}

#include <cstdint>
#include <memory>
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
  /// \brief DataLoader destructor.
  ~DataLoader();

  /// \brief Get instance of class DataLoader.
  /// \param[in] path Path to the DB.
  /// \return Instance of class DataLoader.
  inline static DataLoader &GetDataLoaderInstance(std::string &&path) {
    static DataLoader data_loader{path};
    return data_loader;
  }

  /// \brief Get instance of class DataLoader.
  /// \param[in] path Path to the DB.
  /// \return Instance of class DataLoader.
  inline static DataLoader &GetDataLoaderInstance(const std::string &path) {
    static DataLoader data_loader{path};
    return data_loader;
  }

  /// \brief Execute passed SQL script.
  /// \return Result of executing passed script.
  uint32_t RunSqlScript(const std::string &sql_script);

  /// \Set database name.
  /// \param[in] db_name Database bame.
  void SetDataBaseName(const std::string &db_name);
  void SetDataBaseName(std::string &&db_name);

  /// \Get database name.
  /// \return Database name.
  std::string GetDataBaseName() const noexcept; 

  /// \brief Set database table name.
  /// \param[in] db_table_name Database table name.
  void SetDataBaseTableName(const std::string &db_table_name);

  /// \brief Get database table name.
  /// \return Database table name.
  std::string GetDataBaseTableName() const noexcept;

  /// \brief Insert data into database.
  /// \param[in] val_lst List of values.
  uint32_t InsertIntoTable(const std::string &insert_val) const noexcept;

  /// \brief Select values for the passed columns.
  /// \param[in] val_lst List of values.
  uint32_t SelectFromTable(const std::string &select_val) const noexcept;

  /// \brief Is current database used as in-memory database.
  /// \return Result of checking wether the current DB is in memory.
  bool IsInMemoryUse() const noexcept;

  /// \brief Check if database size limit reached.
  /// \return State of the check if database size limit reached.
  bool IsDataBaseSizeLimitReached();

  /// \brief Check if table with passed name is already exist in db.
  /// \param[in] table_name Table name.
  /// \return Result of the check if the table with specified name is already
  /// \return Status of the passed db table.
  bool IsDataBaseTableExist(const std::string &table_name) const noexcept;
  bool IsDataBaseTableExist(std::string &&table_name) const noexcept;

  /// brief Check if current database table exist.
  /// \return Status of the passed db table.
  bool IsDataBaseTableExist() const noexcept;

 private:
  /// \brief DataLoader constructor.
  /// \param[in] path Path.
  DataLoader(std::string &&path);
  DataLoader(const std::string &path);

  /// \brief Class DataLoader move constructor.
  /// \param[in] data_loader Class DataLoader object.
  DataLoader(DataLoader &&data_loader) = default;

  /// \brief Class DataLoader copy constructor.
  /// \param[in] data_loader Class DataLoader object.
  DataLoader(const DataLoader &data_loader) = delete;

  /// \brief Class DataLoader move assignment.
  /// \param[in] data_loader Class DataLoader object.
  /// \return DataLoader object.
  DataLoader &operator=(DataLoader &&data_loader) = default;

  /// brief Class Dataloader copy assignment.
  /// param[in] data_loader Class DataLoader object.
  /// \return DataLoader object.
  DataLoader &operator=(const DataLoader &data_loader) = delete;

  struct PimplDBHandler;
  std::unique_ptr<PimplDBHandler> pimpl_db_handler_;
};
}  // namespace client
}  // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_DATA_LOADER_H_
