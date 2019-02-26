/// \file data_loader_service.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.4
/// \date 26.02.2019

#include "data_loader_service.h"

extern "C" {
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
}

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "data_loader/data_loader.h"

namespace crypto_wallet {
namespace client {

#define DB_SIZE_LIMIT 1000 // MB
#define DB_TABLE_NAME "main_table"
#define DB_BACKUP_NAME "test_db.backup"
#define CREATE_TBALE
#define INSERT_DATA
#define FILE_LOCKER "file_locker"

DataLoaderService::DataLoaderService(std::string &&path) : path_{path} {}

DataLoaderService::DataLoaderService(const std::string &path) : path_{path} {}

void DataLoaderService::RunAsDaemon() {
  Daemonize();
  SetAndProcessConnection();
  using DataLoader::GetDataLoaderInstance;
  DataLoader::GetDataLoaderInstance(path_).SetDataBaseTableName(DB_TABLE_NAME);
  DataLoader::GetDataLoaderInstance(path_).SetDataBaseSizeLimit(DB_SIZE_LIMIT);
  while (true) {
    if (DataLoader::GetDataLoaderInstance(path_).IsDataBaseSizeLimitReached()) {
      CreateDataBaseBackUp(DB_BACKUP_NAME);
      sleep(10);
      DataLoader::GetDataLoaderInstance(path_).SetDataBaseTableName(DB_TABLE_NAME);
      DataLoader::GetDataLoaderInstance(path_).SetSqlScript(CREATE_TBALE);
    }
    DataLoader::GetDataLoaderInstance(path_).SetSqlScript(CREATE_TBALE)
    DataLoader::GetDataLoaderInstance(path_).SetSqlScript(INSERT_DATA);
    SetData("recieved data");
    sleep(20);
  }
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

void DataLoaderService::LockFile() const noexcept {
  auto fd = open(FILE_LOCKER, O_CREATE|O_EXCL); 
  if (fd < 0) {
    exit(EXIT_FAILURE);
  }
}

} // namespace client
} // namespace crypt_wallet
