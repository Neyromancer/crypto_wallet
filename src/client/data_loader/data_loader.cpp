/// \file data_loader.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.8
/// \date 20.04.2019

#include "data_loader.h"

extern "C" {
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sqlite3.h>
}

#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>
#include <tuple>
#include <vector>

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
    std::cout << "init_list_to_string res is " << res << std::endl;
    return res;
  }
  static std::pair<std::string, std::vector<std::string>> columns_;
}

struct DataLoader::PimplDBHandler {
 public:
  /// \brief Set database table name.
  /// \param[in] db_table_name Database table name.
  void SetDataBaseTableName(const std::string &db_table_name);

  /// \brief Evaluate database size.
  /// \return Database size.
  int32_t GetDataBaseSize() const noexcept;

  /// \brief Execute passed SQL script.
  /// \return Result of execution.
  int32_t RunSqlScript(const std::string &sql_script);
  
  /// \brief Display returned database value.
  /// \param[in] Unused parameter.
  /// \param[in] Number of columns to select.
  /// \param[out] Data from the columns.
  /// \param[out] Names of the columns.
  /// \return Result of calling the method.
  static int retrieve_db_value(void *, int cols, char **col_data, char **col_names);

  //sqlite3 *database_{nullptr};
  std::unique_ptr<sqlite3 *> database_ = std::make_unique<sqlite3 *>();
  std::string db_name_{"file::memory:?cache=shared"};
  std::string db_table_name_;
  std::string db_table_struct_template_;
  bool is_data_table_exist_{false};
  bool is_select_{false};
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
  return pimpl_db_handler_->RunSqlScript(sql_script);
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
  return pimpl_db_handler_->RunSqlScript(sql_script);
}

/// TODO implement IsSelectValValid(const std::string &val)
/// valid select request ex: SELECT col1, col2, ... colN FROM table_bane;
int32_t DataLoader::SelectColumnFromTable(const std::string &column_name,
                                          std::pair<std::string, 
                                                    std::vector<std::string>> &column_data) {
  if (column_name.empty())
    return SQLITE_MISMATCH;

  if (!IsDataBaseTableExist())
    return SQLITE_INTERNAL;

  pimpl_db_handler_->is_select_ = true;
  std::cout << "select method called | " << pimpl_db_handler_->is_select_ << std::endl;
  std::string select_template = "SELECT %1% FROM %2%;";

  auto sql_script = boost::str(boost::format{select_template} % column_name % 
                              pimpl_db_handler_->db_table_name_);
  auto res = pimpl_db_handler_->RunSqlScript(sql_script);
  column_data = columns_;

  return res;
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
    return pimpl_db_handler_->is_data_table_exist_ = (pimpl_db_handler_->RunSqlScript(sql_script) 
                                                      == SQLITE_OK);
  }
  // set to false after backup pimpl_db_handler_->is_data_table_exist_;

}

bool DataLoader::IsDataBaseTableExist(std::string &&table_name) {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string db_template = "SELECT COUNT(type) from sqlite_master WHERE type = 'table' AND name" 
                              " = '%1%'";
    std::string sql_script = boost::str(boost::format{db_template} % table_name);
    return pimpl_db_handler_->is_data_table_exist_ = (pimpl_db_handler_->RunSqlScript(sql_script) 
                                                      == SQLITE_OK);
  }
  // set to false after backup pimpl_db_handler_->is_data_table_exist_;
}

bool DataLoader::IsDataBaseTableExist() {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string db_template = "SELECT COUNT(type) from sqlite_master WHERE type = 'table' AND name" 
                              " = '%1%'";
    std::string sql_script = boost::str(boost::format{db_template} % GetDataBaseTableName());
    return pimpl_db_handler_->is_data_table_exist_ = (pimpl_db_handler_->RunSqlScript(sql_script) 
                                                      == SQLITE_OK);
  }
  // set to false after backup pimpl_db_handler_->is_data_table_exist_;
}

DataLoader::~DataLoader() {
  sqlite3_close(*((pimpl_db_handler_->database_).get()));
}

int32_t DataLoader::PimplDBHandler::RunSqlScript(const std::string &sql_script) {
  char *errMsg;
  std::cout << "run sql script method is called | select " << is_select_ << std::endl;
  auto callback = is_select_ ? DataLoader::PimplDBHandler::retrieve_db_value : NULL;
  auto res = sqlite3_exec(*(database_.get()), 
                          sql_script.c_str(), DataLoader::PimplDBHandler::retrieve_db_value, 0, &errMsg);
  is_select_ = false;

  if (res != SQLITE_OK) {
    // throw exception here
    std::cout << "Script evaluation results in error: " << std::string(errMsg) << std::endl;
    sqlite3_free(errMsg);
  }

  return res;
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

int DataLoader::PimplDBHandler::retrieve_db_value(void *, int col, char **col_data, 
                                                  char **col_name) {
  std::vector<std::string> res;
  std::cout << "retrieve_db_value method is called | col " << col << std::endl;
  for (auto i = 0; i < col; ++i) {
    std::cout << std::string(col_data[i]) << std::endl;
    res.push_back(std::move(std::string(col_data[i])));
  }

  std::cout << col_name[0] << std::endl;
  columns_ = std::make_pair(std::string(col_name[0]), res);

  return 0;
}

} // namespace client
} // namespace crypt_wallet
