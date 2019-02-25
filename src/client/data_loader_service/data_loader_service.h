/// \file data_loader_service.h
/// \brief Class responsible for loading data into DB.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.3
/// \date 25.02.2019

#ifndef CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
#define CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_

#include <cstdint>
#include <string>

#include "data_loader/data_loader.h"

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
  /// \brief DataLoaderService constructor.
  /// \param[in] path Path.
  DataLoaderService(std::string &&path);
  DataLoaderService(const std::string &path);

  /// \brief Class DataLoaderService copy constructor.
  /// \param[in] data_loader_service Class DataLoaderService object.
  DataLoaderService(const DataLoaderService &data_loader_service) = delete;

  /// \brief Class DataLoaderService move constructor.
  /// \param[in] data_loader_service Class DataLoaderService object.
  DataLoaderService(DataLoaderService &&data_loader_service) = default;

  /// \brief Class DataLoaderService copy assignment.
  /// \param[in] data_loader_service Class DataLoaderService object.
  /// \return DataLoaderService object.
  DataLoaderService &operator=(const DataLoaderService &data_loader_service) = delete;

  /// \brief Class DataLoaderService move assignment.
  /// \param[in] data_loader_service Class DataLoaderService object.
  /// \return DataLoaderService object.
  DataLoaderService &operator=(DataLoaderService &&data_loader_service) =
default;

  /// \brief DataLoaderService destructor.
  ~DataLoaderService() = default; 

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
 
 private:
  /// \brief Daemonize process.
  void Daemonize() const noexcept;
  
  //socket_communication::UnixConnection unix_connect_{};
  std::string data_{};
};
}  // namespace client
}  // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
