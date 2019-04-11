/// \file data_loader.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.6
/// \date 11.04.2019

#include "data_loader.h"

extern "C" {
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
}

#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>

#include "boost/format.hpp"

namespace crypto_wallet {
namespace client {

#define DB_SIZE_LIMIT 1000 // MB

namespace {
  // Name containing ONLY alphanumeric symbols is valid.
  bool IsDBNameValid(const std::string &db_name) {
    const std::string rex_str{"[a-zA-Z0-9_.-]"};
    return std::regex_match(db_name, rex_str);
  }

  std::string init_list_to_string(const std::initializer_list<std::string> &init_lst) {
    std::string res = "";
    auto sz = init_lst.size();
    for (const auto &el : init_lst) {
      res += el;
      if (--sz >= 1)
        res += ", ";
    }
    return res;
  }
}

struct DataLoader::PimplDBHandler {
 public:
  /// \brief Evaluate database size.
  /// \return Database size.
  int32_t GetDataBaseSize() const;

  //sqlite3 *database_{nullptr};
  std::unique_ptr<sqlite3 *> database_ = std::make_unique<sqlite3 *>();
  std::string db_name_{"file::memory:?cache=shared"};
  std::string db_table_name_;
  std::initializer_list<std::string> db_table_struct_template_;
  bool is_data_table_exist_{false};
};


DataLoader::DataLoader(std::string &&path = nullptr) 
    : pimpl_db_handler_{std::make_unique<DataLoader::PimplDBHandler>()} {
  SetDataBaseName(path);
  if (SQLITE_OK != sqlite3_open((pimpl_db_handler_->db_name_).c_str(),
                                ((pimpl_db_handler_->database_).get())))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
}

DataLoader::DataLoader(const std::string &path = nullptr) 
    : pimpl_db_handler_{std::make_unique<DataLoader::PimplDBHandler>()} {
  SetDataBaseName(path);
  if (SQLITE_OK != sqlite3_open((pimpl_db_handler_->db_name_).c_str(), 
                                (pimpl_db_handler_->database_).get()))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
}

int32_t DataLoader::RunSqlScript(const std::string &sql_script) {
  char *errMsg;
  auto res = sqlite3_exec(*((pimpl_db_handler_->database_).get()), 
                          sql_script.c_str(), NULL, 0, &errMsg);
  if (res != SQLITE_OK) {
    // throw exception here
    std::cout << "Script evaluation results in error: " << std::string(errMsg) << std::endl;
    sqlite3_free(errMsg);
  }
  return res;
}

/// TODO implement IsDBNameValid
void DataLoader::SetDataBaseName(const std::string &db_name) {
  if (db_name && IsDBNameValid(db_name))
    pimpl_db_handler_->db_name_ = db_name;
}

/// TODO implement IsDBNameValid
void DataLoader::SetDataBaseName(std::string &&db_name) {
  if (db_name && IsDBNameValid(db_name))
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

void DataLoader::SetDBTableStructTemplate(const std::initializer_list<std::string> &db_struct_template) {
  db_table_struct_template_ = db_struct_template;
}

std::string DataLoader::GetDBTableStructTemplate() const noexcept {
  return init_list_to_string(db_table_struct_template_);
}

/// Valid insert request ex: INSERT INTO table_name (col1, ... colN) 
/// VALUES (val1 ... valN);
int32_t DataLoader::InsertIntoTable(const std::initializer_list<std::string> &val_lst) const noexcept {
  int32_t res{SQLITE_MISMATCH};
  if (val_lst.size() != db_table_struct_template_.size())
    return res;

  std::string insert_template = "INSERT INTO %1% (%2%) VALUES (%3%);"
  auto sql_script = boost::str(boost::format{insert_template} % pimpl_db_handler_->db_table_name_ %
                              GetDBTableStructTemplate() % init_list_to_string(val_lst));
  res = RunSqlScript(sql_scrpt);

  return res;
}

/// TODO implement IsSelectValValid(const std::string &val)
/// valid select request ex: SELECT col1, col2, ... colN FROM table_bane;
int32_t DataLoader::SelectFromTable(const std::initializer_list<std::string> &val_lst) const noexcept {
  int32_t res{0};
  std::string select_template = "SELECT %1% FROM %2%;";
  auto sql_script = boost::str(boost::format{select_template} % init_list_to_string(val_lst) % 
                              pimpl_db_handler_->db_table_name);
  res = RunSqlScript(sql_script);

  return res;
}

bool DataLoader::IsInMemoryUse() const noexcept {
 return (pimpl_db_handler_->db_name_).find("memory") != std::string::npos;
}

bool DataLoader::IsDataBaseSizeLimitReached() {
  return DB_SIZE_LIMIT <= pimpl_db_handler_->GetDataBaseSize();
}

int32_t DataLoader::PimplDBHandler::GetDataBaseSize() const {
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
    std::string sql_script{"SELECT COUNT(type) from sqlite_master WHERE type = "
                           "'table' AND name = " + table_name};
    pimpl_db_handler_->is_data_table_exist_ = (RunSqlScript(sql_script) == 1);
  }

  // set to false after backup.
  return pimpl_db_handler_->is_data_table_exist_;
}

bool DataLoader::IsDataBaseTableExist(std::string &&table_name) {
  if (!pimpl_db_handler_->is_data_table_exist_) {
  std::string sql_script{"SELECT COUNT(type) from sqlite_master WHERE type = "
                         "'table' AND name = " + table_name};
  pimpl_db_handler_->is_data_table_exist_ = (RunSqlScript(sql_script) == 1);
}

  // set to false after backup.
  return pimpl_db_handler_->is_data_table_exist_;
}

bool DataLoader::IsDataBaseTableExist() {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string sql_script{"SELECT COUNT(type) from sqlite_master WHERE type = "
                           "'table' AND name = " + GetDataBaseTableName()};
    pimpl_db_handler_->is_data_table_exist_ = (RunSqlScript(sql_script) == 1);
  }

  // set to false after backup.
  return pimpl_db_handler_->is_data_table_exist_;
}

DataLoader::~DataLoader() {
  sqlite3_close(*((pimpl_db_handler_->database_).get()));
}

} // namespace client
} // namespace crypt_wallet
