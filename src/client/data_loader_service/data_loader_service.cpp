/// \file data_loader_service.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.2
/// \date 24.02.2019

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

#include "boost/format.hpp"

namespace crypto_wallet {
namespace client {

#define DB_SIZE_LIMIT 1000 // MB

//std::unique_ptr<DataLoaderService> DataLoaderService::data_loader_;

DataLoaderService::DataLoaderService(std::string &&path) {
  SetDataBaseName(path);
  if (SQLITE_OK != sqlite3_open(GetDataBaseName().c_str(), &database_))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
  //  std::cout << "database was not created" << std::endl;
}

DataLoaderService::DataLoaderService(const std::string &path) {
  SetDataBaseName(path);
  if (SQLITE_OK != sqlite3_open(GetDataBaseName().c_str(), &database_))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
  //  std::cout << "database was not created" << std::endl;
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

void DataLoaderService::SetDataBaseName(const std::string &db_name) {
  db_name_ = db_name;
}

void DataLoaderService::SetDataBaseName(std::string &&db_name) {
  db_name_ = db_name;
}

void DataLoaderService::RunSqlScript() {
  char *errMsg;
  auto res = sqlite3_exec(database_, GetSqlScript().c_str(), NULL, 0, &errMsg);
  if (res != SQLITE_OK) {
    // throw exception here
    std::cout << "Script evaluation results in error: " << std::string(errMsg) << std::endl;
    sqlite3_free(errMsg);
  }
}

void DataLoaderService::SetDataBaseTableName(const std::string &db_table_name) {
  db_table_name_ = db_table_name;
}

void DataLoaderService::SetSqlScript(const std::string &sql_scrpt) {
  sql_script_ = boost::str(boost::format{sql_scrpt} % db_table_name_);
}

bool DataLoaderService::IsDataBaseSizeLimitReached() {
  return DB_SIZE_LIMIT <= GetDataBaseSize();
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

uint32_t DataLoaderService::GetDataBaseSize() {
  struct stat sb{};
  if (stat(GetDataBaseName().c_str(), &sb) == -1) {
    // throw exception here
    std::cout << "Error occured while retrieving database size " 
              << std::strerror(errno) << std::endl;
  }
  return (sb.st_size / 1000) / 1000;
}

} // namespace client
} // namespace crypt_wallet
