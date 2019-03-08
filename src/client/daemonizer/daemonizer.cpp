/// \file daemonizer.cpp
/// \brief Source file which contains implementaion of class Daemonizer.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.2
/// \date 11.03.2019

#include "daemonizer.h"

extern "C" {
#include <limits.h>
#include <signal.h>
#include <unistd.h>
}

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

#include "boost/format.hpp"

namespace crypto_wallet {
namespace client {

// TODO set appropriate default data for uid 
// TODO and gid.
Daemonizer::Daemonizer(std::string &&path, int uid, int gid) 
    : daemon_working_path_{path},
      user_id_{uid}, 
      group_id_{gid} {}

Deamonizer::Daemonizer(const std::string &path, int uid, int gid)
    : daemon_working_path_{path},
      user_id_{uid},
      group_id_{gid} {}

void Daemonizer::Daemonize() const noexcept {
  pid_t dpid = fork();
  
  if (dpid < 0)
    exit(EXIT_FAILURE);

  if (dpid > 0)
    exit(EXIT_SUCCESS);

  if (setsid() < 0)
    exit(EXIT_FAILURE);

  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  dpid = fork();

  if (dpid < 0)
    exit(EXIT_FAILURE);

  if (dpid > 0)
    exit(EXIT_SUCCESS);

  umask(0);

  chdir("/");

  for (auto i = sysconf(_SC_OPEN_MAX); i >= 0; --i)
    close(i);
}

void Daemonizer::RunAsDaemon() const noexcept {
  Daemonize();

  while (is_daemon_running_)
    sleep(1);
}

void Daemonizer::SignalCatcher() const noexcept {
  struct sigaction sig_handler;
  memset(&sig_handler, 0, sizeof(sig_handler));

  sig_handler.sa_handler = &SignlaHandler;
  sigemptyset(&sig_handler.sa_mask);
  sig_handler.sa_mask = SA_RESTART;

  if (sigaction(SIGINT, &sig_handler, NULL) < 0) {
    // TODO add writing to log
    throw std::invalid_argument(std::strerror(errno));
  }

//  if (sigaction(SIGHUP, &sig_handler, NULL) < 0) {
    // TODO add writing to log
    //throw std::invalid_argument(std::strerror(errno));
    //}

  if (sigaction(SIGTERM, &sig_handler, NULL) < 0) {
    // TODO add writing to log
    throw std::invalid_argument(std::strerror(errno));
  }
}

void SignalHandler(int signal) const noexcept {
  switch (signal) {
  case SIGINT:
    is_daemon_running_ = true;
    break;
  case SIGTERM: // reread log files
    is_daemon_running_ = false;
    break;
  case SIGHUP:
    is_daemon_running_ = false;
    break;
  default:
    break;
  }
}

} // namespace client
} // namespace crypt_wallet
