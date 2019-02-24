/// \file data_loader_service.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.2
/// \date 24.02.2019

#include "data_loader_service.h"

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
}

#include <cerrno>
#include <cstring>
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

void DataLoaderService::Daemonize() const noexcept {

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
