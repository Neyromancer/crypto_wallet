/// \file data_loader_service.h
/// \brief Class responsible for loading data into DB.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.1
/// \data 13.02.2019

#ifndef CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
#define CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_

extern "C" {
#include <sqlite3.h>
}

#include <memory>
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
  DataLoaderService(const DataLoaderService &data_loader_service) = delete;

  /// \brief Class DataLoaderService copy assignment.
  /// \param[in] data_loader_service Class DataLoaderService object.
  /// \return DataLoaderService object.
  DataLoaderService &operator=(const DataLoaderService &data_loader_service) =
delete;
  
  /// \brief Connect to DB.
  /// \param[in] path Path to the DB.
  /// \return Smart pointer to object of class DataLoaderService.
  inline static std::shared_ptr<DataLoaderService> ConnectDataBase(std::string &&path) 
  const noexcept {
    return std::make_shared<DataLoaderService>(DataLoaderService(path));
  }

  /// \brief Connect to DB.
  /// \param[in] path Path to the DB.
  /// \return Smart pointer to object of class DataLoaderService.
  inline static std::shared_ptr<DataLoaderService> ConnectDataBase(const std::string &path) 
  const noexcept {
    return std::make_shared<DataLoaderService>(DataLoaderService(path));
  }

  /// \brief Daemonize process.
  void Daemonize() const noexcept;

  /// \brief Set and look after the connection.
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
    return database_;
  }
  
 private:
  /// \brief DataLoaderService constructor.
  /// \param[in] path Path.
  DataLoaderService(std::string &&path);

  /// \brief DataLoaderService destructor.
  ~DataLoaderService() {
    sqlite3_close(database_);
  }

  /// \brief Class DataLoaderService move constructor.
  /// \param[in] data_loader_service Class DataLoaderService object.
  DataLoaderService(DataLoaderService &&data_loader_service) = default;

  /// \brief Class DataLoaderService move assignment.
  /// \param[in] data_loader_service Class DataLoaderService object.
  /// \return DataLoaderService object.
  DataLoaderService &operator=(DataLoaderService &&data_loader_service) =
default; 

  //socket_communication::UnixConnection unix_connect_{};
  std::string data_{};
  sqlite3 *database_{nullptr};
};
}  // namespace client
}  // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
