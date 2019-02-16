/// \file data_loader_service.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.1
/// \date 16.02.2019

#include <iostream>

#include "data_loader_service.h"

namespace crypto_wallet {
namespace client {

DataLoaderService *DataLoaderService::data_loader_ = nullptr;

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

}
}
