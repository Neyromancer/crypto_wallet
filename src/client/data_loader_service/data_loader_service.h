/// \file data_loader_service.h
/// \brief Class responsible for loading data into DB.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.0
/// \data 07.02.2019

#ifndef CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
#define CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_

extern "C" {
#include <sqlite3.h>
}

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
  /// \brief Daemonize process.
  void Daemonize() const noexcept;

  /// \brief Set and look after the connection.
  void SetAndProcessConnection();

  /// \brief Set data.
  /// \param[in] data Data.
  void SetData(std::string data);

  /// \brief Return data.
  /// \return Data.
  inline std::string GetData() const noexcept {
    return data_;
  }
  
 private:
  socket_communication::UnixConnection unix_connect_{};
  std::string data_{};
};
}  // namespace client
}  // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_DATA_LOADER_SERVICE_H_
