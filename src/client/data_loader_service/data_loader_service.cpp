/// \file data_loader_service.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.3
/// \date 25.02.2019

#include "data_loader_service.h"

extern "C" {
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
}

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <iostream>

namespace crypto_wallet {
namespace client {

#define DB_SIZE_LIMIT 1000 // MB

DataLoaderService::DataLoaderService(std::string &&path) {
}

DataLoaderService::DataLoaderService(const std::string &path) {
}

void DataLoaderService::RunAsDaemon() {
  Daemonize();

  while (true)
    sleep(20);
}

void DataLoaderService::SetAndProcessConnection() {
}

void DataLoaderService::SetData(const std::string &data) {
  data_ = data;
}

void DataLoaderService::SetData(std::string &&data) {
  data_ = std::move(data);
}

void DataLoaderService::Daemonize() const noexcept {
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

} // namespace client
} // namespace crypt_wallet
