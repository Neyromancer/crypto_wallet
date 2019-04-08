/// \file data_loader.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.4
/// \date 09.04.2019

#include "data_loader.h"

extern "C" {
#include <signal.h>
#include <sqlite3.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
}

#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "boost/format.hpp"

namespace crypto_wallet {
namespace client {

struct DataLoader::PimplDBHandler {
 public:
  /// \brief Evaluate database size.
  /// \return Database size.
  uint32_t GetDataBaseSize() const;

  //sqlite3 *database_{nullptr};
  std::unique_ptr<sqlite3 *> database_ = std::make_unique<sqlite3 *>();
  std::string db_name_;
  std::string db_table_name_;
  std::string sql_script_;
  bool is_data_table_exist_{false};
};

#define DB_SIZE_LIMIT 1000 // MB

DataLoader::DataLoader(std::string &&path) 
    : pimpl_db_handler_{std::make_unique<DataLoader::PimplDBHandler>()} {
  SetDataBaseName(path);
  if (SQLITE_OK != sqlite3_open((pimpl_db_handler_->db_name_).c_str(),
                                ((pimpl_db_handler_->database_).get())))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
  //  std::cout << "database was not created" << std::endl;
}

DataLoader::DataLoader(const std::string &path) 
    : pimpl_db_handler_{std::make_unique<DataLoader::PimplDBHandler>()} {
  SetDataBaseName(path);
  if (SQLITE_OK != sqlite3_open((pimpl_db_handler_->db_name_).c_str(), 
                                (pimpl_db_handler_->database_).get()))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
}

sqlite3 *DataLoader::GetDataBase() const noexcept {
  return *((pimpl_db_handler_->database_).get());
}

uint32_t DataLoader::RunSqlScript() {
  char *errMsg;
  auto res = sqlite3_exec(*((pimpl_db_handler_->database_).get()), 
                          (pimpl_db_handler_->sql_script_).c_str(), NULL, 0, &errMsg);
  if (res != SQLITE_OK) {
    // throw exception here
    std::cout << "Script evaluation results in error: " << std::string(errMsg) << std::endl;
    sqlite3_free(errMsg);
  }
  return res;
}

void DataLoader::SetDataBaseName(const std::string &db_name) {
  pimpl_db_handler_->db_name_ = db_name;
}

void DataLoader::SetDataBaseName(std::string &&db_name) {
  pimpl_db_handler_->db_name_ = db_name;
}

std::string DataLoader::GetDataBaseName() const noexcept {
  return pimpl_db_handler_->db_name_;
}

void DataLoader::SetDataBaseTableName(const std::string &db_table_name) {
  pimpl_db_handler_->db_table_name_ = db_table_name;
}

std::string DataLoader::GetDataBaseTableName() const noexcept {
  return pimpl_db_handler_->db_table_name_;
}

void DataLoader::SetSqlScript(const std::string &sql_scrpt) {
  pimpl_db_handler_->sql_script_ = boost::str(boost::format{sql_scrpt} % 
                                             pimpl_db_handler_->db_table_name_);
}

std::string DataLoader::GetSqlScript() const noexcept {
  return pimpl_db_handler_->sql_script_;
}

bool DataLoader::IsDataBaseSizeLimitReached() {
  return DB_SIZE_LIMIT <= pimpl_db_handler_->GetDataBaseSize();
}

uint32_t DataLoader::PimplDBHandler::GetDataBaseSize() const {
  struct stat sb{};
  if (stat(db_name_.c_str(), &sb) == -1) {
    // throw exception here
    std::cout << "Error occured while retrieving database size " 
              << std::strerror(errno) << std::endl;
  }

  return (sb.st_size / 1000) / 1000;
}

bool DataLoader::IsDataBaseTableExist(const std::string &table_name) {
  if (!(pimpl_db_handler_->is_data_table_exist_)) {
    std::string sql_script = "SELECT COUNT(type) from sqlite_master WHERE type = "
                            "'table' AND name = " + table_name;
    SetSqlScript(sql_script);
    pimpl_db_handler_->is_data_table_exist_ = (RunSqlScript() == 1);
  }

  // set to false after backup.
  return pimpl_db_handler_->is_data_table_exist_;
}

bool DataLoader::IsDataBaseTableExist(std::string &&table_name) {
  if (!pimpl_db_handler_->is_data_table_exist_) {
  std::string sql_script = "SELECT COUNT(type) from sqlite_master WHERE type = "
                            "'table' AND name = " + table_name;
  SetSqlScript(sql_script);
  pimpl_db_handler_->is_data_table_exist_ = (RunSqlScript() == 1);
}

  // set to false after backup.
  return pimpl_db_handler_->is_data_table_exist_;
}

bool DataLoader::IsDataBaseTableExist() {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string sql_script = "SELECT COUNT(type) from sqlite_master WHERE type = "
                            "'table' AND name = " + GetDataBaseTableName();
    SetSqlScript(sql_script);
    pimpl_db_handler_->is_data_table_exist_ = (RunSqlScript() == 1);
  }

  // set to false after backup.
  return pimpl_db_handler_->is_data_table_exist_;
}

DataLoader::~DataLoader() {
  sqlite3_close(*((pimpl_db_handler_->database_).get()));
}

} // namespace client
} // namespace crypt_wallet
