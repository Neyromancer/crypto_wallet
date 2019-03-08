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
  /// \param[in] path Path to daemon working directory.
  /// \param[in] uid User ID.
  /// \param[in] gid Group ID.
  Daemonizer(std::string &&path, int uid, int gid);
  Daemonizer(const std::string &path, int uid, int gid);

  /// \brief DataLoaderService destructor.
  ~Daemonizer() = default;

  // TODO try to make these funcitons constexpr and if needed.
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

  /// \brief Set user and group id for the deamon to use.
  /// \param[in] user_id User ID.
  /// \param[in] group_id Group ID.
  void SetUserAndGroupID(int user_id, int group_id);
  
  /// \brief Get user ID.
  /// \return User ID.
  constexpr int GetUserID() const noexcept {
    return user_id_;
  }

  constexpr int GetGroupID() const noexcept {
    return group_id_;
  }

  /// \brief Run as daemon.
  void RunAsDaemon() const noexcept = 0;

  /// \brief Catch system signals.
  void SignalCatcher() const noexcept;

  /// \brief Change root directory.
  /// \param[in] change Set whether to change root directory.
  void ChangeRootDirectory(bool change);

  /// \brief Return status of whether root directory changed.
  /// \return Status of whether root directory changed.
  constexpr bool IsRootDirectoryChanged() const noexcept {
    return is_chroot_;
  }

  /// \brief Return result of weather the daemon is active or not.
  /// \return Weather deamon is running or not.
  constexpr bool IsDaemonRunning() const noexcept {
    return is_daemon_running_;
  }

 private:
  /// \brief Handle system signals.
  void SignalHandler() const noexcept;

  std::string daemon_working_directory_;
  int user_id_; 
  int group_id_;
  bool is_chroot_{true};
  bool is_daemon_running_{false};
};
}  // namespace client
}  // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_DAEMONIZER_H_
