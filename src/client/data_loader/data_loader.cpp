/// \file data_loader.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.0
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

#include "boost/format.hpp"

namespace crypto_wallet {
namespace client {

#define DB_SIZE_LIMIT 1000 // MB

DataLoader::DataLoader(std::string &&path) {
  SetDataBaseName(path);
  if (SQLITE_OK != sqlite3_open(GetDataBaseName().c_str(), &database_))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
  //  std::cout << "database was not created" << std::endl;
}

DataLoader::DataLoader(const std::string &path) {
  SetDataBaseName(path);
  if (SQLITE_OK != sqlite3_open(GetDataBaseName().c_str(), &database_))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
  //  std::cout << "database was not created" << std::endl;
}

void DataLoader::SetDataBaseName(const std::string &db_name) {
  db_name_ = db_name;
}

void DataLoader::SetDataBaseName(std::string &&db_name) {
  db_name_ = db_name;
}

void DataLoader::RunSqlScript() {
  char *errMsg;
  auto res = sqlite3_exec(database_, GetSqlScript().c_str(), NULL, 0, &errMsg);
  if (res != SQLITE_OK) {
    // throw exception here
    std::cout << "Script evaluation results in error: " << std::string(errMsg) << std::endl;
    sqlite3_free(errMsg);
  }
}

void DataLoader::SetDataBaseTableName(const std::string &db_table_name) {
  db_table_name_ = db_table_name;
}

void DataLoader::SetSqlScript(const std::string &sql_scrpt) {
  sql_script_ = boost::str(boost::format{sql_scrpt} % db_table_name_);
}

bool DataLoader::IsDataBaseSizeLimitReached() {
  return DB_SIZE_LIMIT <= GetDataBaseSize();
}

uint32_t DataLoader::GetDataBaseSize() {
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
