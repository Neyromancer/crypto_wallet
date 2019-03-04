/// \file daemonizer.cpp
/// \brief Source file which contains implementaion of class Daemonizer.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.0
/// \date 04.03.2019

#include "daemonizer.h"

extern "C" {
#include <limits.h>
#include <signal.h>
#include <unistd.h>
}

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <iostream>


namespace crypto_wallet {
namespace client {

void Daemonizer::Daemonize() const noexcept {
  pid_t pid{};
  pid = fork();
  
  if (pid < 0)
    exit(EXIT_FAILURE);

  if (pid > 0)
    exit(EXIT_SUCCESS);

  if (setsid() < 0)
    exit(EXIT_FAILURE);

  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  pid = fork();

  if (pid < 0)
    exit(EXIT_FAILURE);

  if (pid > 0)
    exit(EXIT_SUCCESS);

  umask(0);

  chdir("/");

  for (auto i = sysconf(_SC_OPEN_MAX); i >= 0; --i)
    close(i);
}

void Daemonizer::RunAsDaemon() const noexcept {
  Daemonize();

  while (true)
    sleep(1);
}

void Daemonizer::SignalCatcher() const noexcept {
  struct sigaction sig_handler;
  memset(&sig_handler, 0, sizeof(sig_handler));

  sig_handler.sa_handler = &SignlaHandler;
  if (sigaction(SIGINT, sig_handler) < 0)
  if (sigaction(SIGHUP, ) < 0)
}

void SignalHandler() const noexcept {}


} // namespace client
} // namespace crypt_wallet
