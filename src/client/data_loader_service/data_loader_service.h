/// \file data_loader_service.h
/// \brief Class responsible for loading data into DB.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.2
/// \date 24.02.2019

#ifndef CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
#define CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_

extern "C" {
#include <sqlite3.h>
}

#include <memory>
#include <cstdint>
#include <string>

//#include "socket_connection/unix_connection.h"

/// \namespace crypto_wallet.
/// \brief Project namespace.
namespace crypto_wallet {
/// \namespace client
/// \brief Client namespace.
namespace client {
/// \class DataLoaderService data_loader_service.h.
/// \brief Class responsible for loading data into DB.
class DataLoaderService {
 public:
  /// \brief Class DataLoaderService copy constructor.
  /// \param[in] data_loader_service Class DataLoaderService object.
  //DataLoaderService(const DataLoaderService &data_loader_service) = delete;

  /// \brief Class DataLoaderService copy assignment.
  /// \param[in] data_loader_service Class DataLoaderService object.
  /// \return DataLoaderService object.
  //DataLoaderService &operator=(const DataLoaderService &data_loader_service) =
//delete;

  /// \brief DataLoaderService destructor.
  ~DataLoaderService() {
    sqlite3_close(database_);
  } 

  /// \brief Get instance of class DataLoaderService..
  /// \param[in] path Path to the DB.
  /// \return Instance of class DataLoaderService.
  inline static DataLoaderService &GetDataLoaderServiceInstance(std::string &&path) 
  {
    static DataLoaderService data_loader{path};
    return data_loader;
  }

  /// \brief Get instance of class DataLoaderService.
  /// \param[in] path Path to the DB.
  /// \return Instance of class DataLoaderService.
  inline static DataLoaderService &GetDataLoaderServiceInstance(const std::string &path) 
  {
    static DataLoaderService data_loader{path};
    return data_loader;
  }

  /// \brief Creates current database's backup.
  /// \param[in] backup_pat Path to put backuped database.
  void CreateDataBaseBackUp(const std::string &backup_path);
  void CreateDataBaseBackUp(std::string &&backup_path);

  /// \brief Run service as a daemon.
  void RunAsDaemon();

  /// \brief Set and look after the interprocess connection.
  void SetAndProcessConnection();

  /// \brief Set data.
  /// \param[in] data Data.
  void SetData(const std::string &data);
  void SetData(std::string &&data);

  /// \brief Get data.
  /// \return Data.
  inline std::string GetData() const noexcept {
    return data_;
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

  /// \brief Create table.
  /// \param[in] table_name Table name.
  ///void CreateTable(const std::string &table_name);
  ///void CreateTable(std::string &&table_name);

 private:
  /// \brief DataLoaderService constructor.
  /// \param[in] path Path.
  DataLoaderService(std::string &&path);
  DataLoaderService(const std::string &path);

  /// \brief Class DataLoaderService move constructor.
  /// \param[in] data_loader_service Class DataLoaderService object.
  DataLoaderService(DataLoaderService &&data_loader_service) = default;

  /// \brief Class DataLoaderService move assignment.
  /// \param[in] data_loader_service Class DataLoaderService object.
  /// \return DataLoaderService object.
  DataLoaderService &operator=(DataLoaderService &&data_loader_service) =
default;

  /// \brief Daemonize process.
  void Daemonize() const noexcept;
  
  /// \brief Evaluate database size.
  /// \return Database size.
  uint32_t GetDataBaseSize();

  //socket_communication::UnixConnection unix_connect_{};
  std::string data_{};
  std::string db_name_{};
  sqlite3 *database_{nullptr};
  std::string db_table_name_{};
  std::string sql_script_{};
  // static std::shared_ptr<DataLoaderService> data_loader_{};
  //static std::unique_ptr<DataLoaderService> data_loader_;
};
}  // namespace client
}  // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
