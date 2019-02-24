/// \file data_loader_service.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.2
/// \date 24.02.2019


#include "data_loader_service.h"

#include <iostream>

#include "boost/format.hpp"

namespace crypto_wallet {
namespace client {

//std::unique_ptr<DataLoaderService> DataLoaderService::data_loader_;

DataLoaderService::DataLoaderService(std::string &&path) {
  if (SQLITE_OK != sqlite3_open(path.c_str(), &database_))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
  //  std::cout << "database was not created" << std::endl;
}

DataLoaderService::DataLoaderService(const std::string &path) {
  if (SQLITE_OK != sqlite3_open(path.c_str(), &database_))
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

void DataLoaderService::SetDataBaseTableName(const std::string db_table_name) {
  db_table_name = db_table_name_;
}

void DataLoaderService::SetSqlScript(const std::string &sql_scrpt) {
  sql_script_ = boost::str(boost::format{sql_scrpt} % db_table_name_);
}

}
}
