/// \file data_loader_service.h
/// \brief Class responsible for loading data into DB.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.4
/// \date 26.02.2019

#ifndef CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
#define CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_

#include <cstdint>
#include <string>

#include "socket_connection/unix_connection.h"

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
  
  /// \brief Set path to datatbase.
  /// \param[in] path Path to database.
  void SetPath(const std::string &path);
  void SetPath(std::string &&path);

  /// \brief Get path to database.
  /// \return Path to database.
  inline std::string GetPath() const noexcept {
    return path_;
  }

 private:
  /// \brief Daemonize process.
  void Daemonize() const noexcept;

  void LockFile() const noexcept;

  /// \brief Check if tmp file is loked.
  /// \return Status of the tmp file.
  inline bool IsLockerClose() const noexcept {
    return is_locker_closed_;
  }
  
  //socket_communication::UnixConnection unix_connect_{};
  std::string data_{};
  std::string path_{};
  bool is_locker_closed_{};
};
}  // namespace client
}  // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
