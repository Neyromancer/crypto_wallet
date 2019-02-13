#include "data_loader_service.h"

namespace crypto_wallet {
namespace client {

DataLoaderService::DataLoaderService(std::string &&path) {
  if (SQLITE_OK != sqlite3_open(path, &database))
    // throw error becasuse connection was not set
  if (!*database)
    // throw error because database was not created
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
