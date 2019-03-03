/// \file data_loader_service.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.5
/// \date 03.03.2019

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
#define FILE_LOCKED "mandatory_file_locker"

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

// move this method to the main module of the programm.
void DataLoaderService::LockFile() const noexcept {
  auto fd = open(FILE_LOCKED, O_NONBLOCK|O_RDWR);
  if (fd < 0) {
    exit(EXIT_FAILURE);
  }

  auto res = fchmod(fd, S_ISGID & ~S_IXGRP);
  if (res < 0) {
    // throw exception here or return error code
    return EXIT_FAILURE;
  }

  static struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_start = 0;
  lock.l_whence = SEEK_SET;
  lock.l_len = 0;
  lock.l_pid = getpid();

  res = fcntl(fd, F_SETLKW, &lock);
  if (res < 0) {
    // throw exception here or return error code
    std::cout << "Function fcntl returned with error: " 
              << std::strerror(errno) << std::endl;
  }
}

// move this method to the main module of the programm.
bool DataLoaderService::IsLockerClosed() const noexcept {
  if (!is_locker_closed_) {
    struct stat sb{};
    if (stat(FILE_LOCKED, &sb) == -1) {
      // log this info later
      std::cout << std::strerror(errno) << std::endl;
      is_locker_closed_ = false;
    }

    if ((sb.st_mode & ~S_IFMT) != S_IXGRP && 
        (sb.st_mode & ~S_IFMT) == S_ISGID)
      is_locker_closed_ = true;
  }

  return is_locker_closed_;
}

} // namespace client
} // namespace crypt_wallet
