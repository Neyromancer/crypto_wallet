/// \file daemonizer.h
/// \brief General class for creating daemon/service.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.0
/// \date 04.03.2019

#ifndef CRYPTO_WALLET_CLIENT_DAEMONIZER_H_
#define CRYPTO_WALLET_CLIENT_DAEMONIZER_H_

#include <memory>
#include <cstdint>
#include <string>

/// \namespace crypto_wallet.
/// \brief Project namespace.
namespace crypto_wallet {
/// \namespace client
/// \brief Client namespace.
namespace client {
/// \class Daemonizer daemonizer.h.
/// \brief General class for creating daemon/service.
class Daemonizer {
 public:
  /// \brief Daemonizer constructor.
  /// \param[in] path Path.
  Daemonizer(std::string &&path);
  Daemonizer(const std::string &path);

  /// \brief DataLoaderService destructor.
  ~Daemonizer() = default;

  /// \brief Class Daemonizer move constructor.
  /// \param[in] daemon Class DataLoaderService object.
  Daemonizer(Daemonizer &&daemon) = default;

  /// \brief Class Daemonizer move assignment.
  /// \param[in] daemon Class Daemonizer object.
  /// \return Daemonizer object.
  Daemonizer &operator=(Daemonizer &&daemon) = default;

  /// \brief Class Daemonizer copy constructor.
  /// \param[in] daemon Class Daemonizer object.
  Daemonizer(const Daemonizer &daemon) = delete;

  /// \brief Class Daemonizer copy assignment.
  /// \param[in] daemon Class Daemonizer object.
  /// \return Daemonizer object.
  Daemonizer &operator=(const Daemonizer &daemon) = delete;

  /// \brief Daemonize process.
  void Daemonize() const noexcept;

  /// \brief Run as daemon.
  void RunAsDaemon() const noexcept = 0;

  /// \Catch system signals.
  void SignalCatcher() const noexcept;

  bool IsDaemonRunning() const noexcept {
    return is_daemon_running_;
  }

 private:
  /// \brief Handle system signals.
  void SignalHandler() const noexcept;

  bool is_daemon_running_{false};
  
};
}  // namespace client
}  // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_DAEMONIZER_H_
