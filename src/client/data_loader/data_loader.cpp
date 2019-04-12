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
    bool res{false};
    if (!db_name.empty()) {
      const std::regex rex_str{"\\w+_?+.?+-?"};
      res = std::regex_match(db_name, rex_str);
    }

    return res;
  }

  bool IsTableNameValid(const std::string &db_table_name) {
    bool res{false};
    if (!db_table_name.empty()) {
      const std::regex rex_str{"\\w+_?+-?"};
      res = std::regex_match(db_table_name, rex_str);
    }

    return res;
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
  /// \brief Set database table name.
  /// \param[in] db_table_name Database table name.
  void SetDataBaseTableName(const std::string &db_table_name);

  /// \brief Evaluate database size.
  /// \return Database size.
  int32_t GetDataBaseSize() const noexcept;

  //sqlite3 *database_{nullptr};
  std::unique_ptr<sqlite3 *> database_ = std::make_unique<sqlite3 *>();
  std::string db_name_{"file::memory:?cache=shared"};
  std::string db_table_name_;
  std::string db_table_struct_template_;
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
  if (IsDBNameValid(db_name))
    pimpl_db_handler_->db_name_ = db_name;
}

/// TODO implement IsDBNameValid
void DataLoader::SetDataBaseName(std::string &&db_name) {
  if (IsDBNameValid(db_name))
    pimpl_db_handler_->db_name_ = db_name;
}

std::string DataLoader::GetDataBaseName() const noexcept {
  return pimpl_db_handler_->db_name_;
}



/// leave publi as it can be usefule to be able to check table name
std::string DataLoader::GetDataBaseTableName() const noexcept {
  return pimpl_db_handler_->db_table_name_;
}

void DataLoader::SetDBTableStructTemplate(const std::initializer_list<std::string> 
                                          &db_struct_template) {
  pimpl_db_handler_->db_table_struct_template_ = init_list_to_string(db_struct_template);
}

std::string DataLoader::GetDBTableStructTemplate() const noexcept {
  return pimpl_db_handler_->db_table_struct_template_;
}

int32_t DataLoader::CreateDBTable(const std::string &table_name) {
  pimpl_db_handler_->SetDataBaseTableName(table_name);
  std::string create_table_template = "CREATE TABLE %1% (%2%)";
  auto sql_script = boost::str(boost::format{create_table_template} %
                               pimpl_db_handler_->db_table_name_  % GetDBTableStructTemplate());
  return RunSqlScript(sql_script);
}

/// Valid insert request ex: INSERT INTO table_name (col1, ... colN) 
/// VALUES (val1 ... valN);
int32_t DataLoader::InsertIntoTable(const std::initializer_list<std::string> &val_lst) {
  if (val_lst.size() != pimpl_db_handler_->db_table_struct_template_.size())
    return SQLITE_MISMATCH;

  if (!IsDataBaseTableExist())
    return SQLITE_INTERNAL;

  std::string insert_template = "INSERT INTO %1% (%2%) VALUES (%3%);";
  auto sql_script = boost::str(boost::format{insert_template} % pimpl_db_handler_->db_table_name_ %
                              GetDBTableStructTemplate() % init_list_to_string(val_lst));
  return RunSqlScript(sql_script);
}

/// TODO implement IsSelectValValid(const std::string &val)
/// valid select request ex: SELECT col1, col2, ... colN FROM table_bane;
int32_t DataLoader::SelectFromTable(const std::initializer_list<std::string> &val_lst) {
  if (val_lst.size() == 0)
    return SQLITE_MISMATCH;

  if (!IsDataBaseTableExist())
    return SQLITE_INTERNAL;

  std::string select_template = "SELECT %1% FROM %2%;";
  auto sql_script = boost::str(boost::format{select_template} % init_list_to_string(val_lst) % 
                              pimpl_db_handler_->db_table_name_);
  return RunSqlScript(sql_script);
}

bool DataLoader::IsInMemoryUse() const noexcept {
 return (pimpl_db_handler_->db_name_).find("memory") != std::string::npos;
}

bool DataLoader::IsDataBaseSizeLimitReached() const noexcept {
  return DB_SIZE_LIMIT <= pimpl_db_handler_->GetDataBaseSize();
}

bool DataLoader::IsDataBaseTableExist(const std::string &table_name) {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string db_template = "SELECT COUNT(type) from sqlite_master WHERE type = 'table' AND name" 
                              " = '%1%'";
    std::string sql_script = boost::str(boost::format{db_template} % table_name);
    return pimpl_db_handler_->is_data_table_exist_ = (RunSqlScript(sql_script) == SQLITE_OK);
  }
  // set to false after backup pimpl_db_handler_->is_data_table_exist_;

}

bool DataLoader::IsDataBaseTableExist(std::string &&table_name) {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string db_template = "SELECT COUNT(type) from sqlite_master WHERE type = 'table' AND name" 
                              " = '%1%'";
    std::string sql_script = boost::str(boost::format{db_template} % table_name);
    return pimpl_db_handler_->is_data_table_exist_ = (RunSqlScript(sql_script) == SQLITE_OK);
  }
  // set to false after backup pimpl_db_handler_->is_data_table_exist_;
}

bool DataLoader::IsDataBaseTableExist() {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string db_template = "SELECT COUNT(type) from sqlite_master WHERE type = 'table' AND name" 
                              " = '%1%'";
    std::string sql_script = boost::str(boost::format{db_template} % GetDataBaseTableName());
    return pimpl_db_handler_->is_data_table_exist_ = (RunSqlScript(sql_script) == SQLITE_OK);
  }
  // set to false after backup pimpl_db_handler_->is_data_table_exist_;
}

DataLoader::~DataLoader() {
  sqlite3_close(*((pimpl_db_handler_->database_).get()));
}

void DataLoader::PimplDBHandler::SetDataBaseTableName(const std::string &db_table_name) {
  if (IsTableNameValid(db_table_name))
    db_table_name_ = db_table_name;
}

int32_t DataLoader::PimplDBHandler::GetDataBaseSize() const noexcept {
  struct stat sb{};
  if (stat(db_name_.c_str(), &sb) == -1) {
    // throw exception here ??
    std::cout << "Error occured while retrieving database size " 
              << std::strerror(errno) << std::endl;
  }

  return (sb.st_size / 1000) / 1000;
}

} // namespace client
} // namespace crypt_wallet
