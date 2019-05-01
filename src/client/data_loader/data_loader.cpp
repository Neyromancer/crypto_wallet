/// \file data_loader.cpp
/// \brief Source file containing class DataLoaderService methods definitions.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.9
/// \date 01.05.2019

#include "data_loader.h"

extern "C" {
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sqlite3.h>
}

#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>
#include <iostream>
#include <regex>
#include <tuple>
#include <vector>

#include "boost/format.hpp"
#include "boost/algorithm/string.hpp"

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

  std::string InitListToString(const std::initializer_list<std::string> &init_lst) {
    std::string res = "";
    auto sz = init_lst.size();

    for (const auto &el : init_lst) {
      res += el;
      if (--sz >= 1)
        res += ", ";
    }

    return res;
  }

  bool IsSQLType(const std::string &val) {
    std::vector<std::string> sql_types { "INT", "INTEGER", "TINYINT", "SMALLINT", "MEDIUMINT", 
                                         "BIGINT", "UNSIGNED BIG INT", "INT2", "INT8", "TEXT", 
                                         "CLOB", "BLOB", "REAL", "DOUBLE", "DOUBLE PRECISION", 
                                         "FLOAT", "NUMERIC", "BOOLEAN", "DATE", "DATETIME",
                                         "CHARACTER", "VARCHAR", "NCHAR", "DECIMAL" };

    for (const auto &el : sql_types)
      if (val == el)
        return true;

    return false;
  }

  /// TODO: this method should return information in case of errors.
  std::map<std::string, std::string> InitListToMap(const std::initializer_list<std::string> 
                                                      &init_list) {
  std::map<std::string, std::string> result;

  for (const auto &el : init_list) {
    std::vector<std::string> vc;
    boost::split(vc, el, boost::is_space());

    if (vc.size() == 2) {
      if (!IsSQLType(vc[0]) && IsSQLType(vc[1]))
        result[vc[0]] = vc[1];

      if (!IsSQLType(vc[1]))
        std::cout << vc[1] << "not supported yet" <<std::endl;
    }
  }

  return result;
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
  int32_t RunSQLScript(const std::string &sql_script);
  
  /// \brief Display returned database value.
  /// \param[in] Unused parameter.
  /// \param[in] Number of columns to select.
  /// \param[out] Data from the columns.
  /// \param[out] Names of the columns.
  /// \return Result of calling the method.
  static int RetrieveDBValue(void *, int cols, char **col_data, char **col_names);

  std::string GetDBTableStructTemplateWithTypes() const;

  bool IsValListValid(const std::initializer_list<std::string> &val_lst);

  //sqlite3 *database_{nullptr};
  std::unique_ptr<sqlite3 *> database_ = std::make_unique<sqlite3 *>();
  std::string db_name_{"file::memory:?cache=shared"};
  std::string db_table_name_;
  std::map<std::string, std::string> db_table_struct_template_;
  std::size_t db_table_struct_tamplate_size_{0};
  bool is_data_table_exist_{false};
  bool is_select_{false};
  bool is_db_table_template_exist_{false};
};

DataLoader::~DataLoader() {
  sqlite3_close(*((pimpl_db_handler_->database_).get()));
}

DataLoader::DataLoader(std::string &&path = nullptr) 
    : pimpl_db_handler_{std::make_unique<DataLoader::PimplDBHandler>()} {
  SetDBName(path);
  if (SQLITE_OK != sqlite3_open((pimpl_db_handler_->db_name_).c_str(),
                                ((pimpl_db_handler_->database_).get())))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
}

DataLoader::DataLoader(const std::string &path = nullptr) 
    : pimpl_db_handler_{std::make_unique<DataLoader::PimplDBHandler>()} {
  SetDBName(path);
  if (SQLITE_OK != sqlite3_open((pimpl_db_handler_->db_name_).c_str(), 
                                (pimpl_db_handler_->database_).get()))
    // throw error becasuse connection was not set
    std::cout << "connection was not established" << std::endl;
  //if (*database_ != NULL)
    // throw error because database was not created
}

/// TODO implement IsDBNameValid
void DataLoader::SetDBName(const std::string &db_name) {
  if (IsDBNameValid(db_name))
    pimpl_db_handler_->db_name_ = db_name;
}

/// TODO implement IsDBNameValid
void DataLoader::SetDBName(std::string &&db_name) {
  if (IsDBNameValid(db_name))
    pimpl_db_handler_->db_name_ = db_name;
}

std::string DataLoader::GetDBName() const noexcept {
  return pimpl_db_handler_->db_name_;
}

/// leave publi as it can be usefule to be able to check table name
std::string DataLoader::GetDBTableName() const noexcept {
  return pimpl_db_handler_->db_table_name_;
}

void DataLoader::SetDBTableStructTemplate(const std::initializer_list<std::string> 
                                          &db_struct_template) {
  pimpl_db_handler_->db_table_struct_tamplate_size_ = db_struct_template.size();
  pimpl_db_handler_->db_table_struct_template_ = InitListToMap(db_struct_template);
  pimpl_db_handler_->is_db_table_template_exist_ = true;
}

std::string DataLoader::GetDBTableStructTemplate() const noexcept {
  std::string result;
  std::size_t cnt{0};
  for (const auto &el : pimpl_db_handler_->db_table_struct_template_) {
    result += el.first;
    if (cnt < pimpl_db_handler_->db_table_struct_template_.size() - 1)
      result += ", ";
    ++cnt;
  }

  return result;
}

int32_t DataLoader::CreateDBTable(const std::string &table_name) {
  pimpl_db_handler_->SetDataBaseTableName(table_name);
  std::string create_table_template = "CREATE TABLE %1% (%2%)";
  auto sql_script = boost::str(boost::format{create_table_template} %
                               pimpl_db_handler_->db_table_name_  %
                               pimpl_db_handler_->GetDBTableStructTemplateWithTypes());

  return pimpl_db_handler_->RunSQLScript(sql_script);
}

/// Valid insert request ex: INSERT INTO table_name (col1, ... colN) 
/// VALUES (val1 ... valN);
int32_t DataLoader::InsertIntoTable(const std::initializer_list<std::string> &val_lst) {
  if (!(pimpl_db_handler_->IsValListValid(val_lst)))
    return SQLITE_MISMATCH;

  if (!IsDBTableExist())
    return SQLITE_INTERNAL;

  std::string insert_template = "INSERT INTO %1% (%2%) VALUES (%3%);";
  auto sql_script = boost::str(boost::format{insert_template} % pimpl_db_handler_->db_table_name_ %
                              GetDBTableStructTemplate() % InitListToString(val_lst));
  return pimpl_db_handler_->RunSQLScript(sql_script);
}

/// TODO implement IsSelectValValid(const std::string &val)
/// valid select request ex: SELECT col1, col2, ... colN FROM table_bane;
int32_t DataLoader::SelectColumnFromTable(const std::string &column_name,
                                          std::pair<std::string, 
                                                    std::vector<std::string>> &column_data) {
  if (column_name.empty())
    return SQLITE_MISMATCH;

  if (!IsDBTableExist())
    return SQLITE_INTERNAL;

  pimpl_db_handler_->is_select_ = true;
  std::string select_template = "SELECT %1% FROM %2%;";

  auto sql_script = boost::str(boost::format{select_template} % column_name % 
                              pimpl_db_handler_->db_table_name_);

  auto res = pimpl_db_handler_->RunSQLScript(sql_script);
  column_data = columns_;

  return res;
}

int32_t DataLoader::ExecuteDirectSQLSequence(const std::string &sql_script, 
                                             std::string &db_result_sequence) {
  pimpl_db_handler_->is_select_ = true;
  return pimpl_db_handler_->RunSQLScript(sql_script);

}

bool DataLoader::IsInMemoryUse() const noexcept {
 return (pimpl_db_handler_->db_name_).find("memory") != std::string::npos;
}

bool DataLoader::IsDBSizeLimitReached() const noexcept {
  return DB_SIZE_LIMIT <= pimpl_db_handler_->GetDataBaseSize();
}

bool DataLoader::IsDBTableExist(const std::string &table_name) {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string db_template = "SELECT COUNT(type) from sqlite_master WHERE type = 'table' AND name" 
                              " = '%1%'";
    std::string sql_script = boost::str(boost::format{db_template} % table_name);
    pimpl_db_handler_->is_data_table_exist_ = (pimpl_db_handler_->RunSQLScript(sql_script) 
                                                      == SQLITE_OK);
  }
  // TODO: set to false after backup pimpl_db_handler_->is_data_table_exist_;
  return pimpl_db_handler_->is_data_table_exist_;
}

bool DataLoader::IsDBTableExist(std::string &&table_name) {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string db_template = "SELECT COUNT(type) from sqlite_master WHERE type = 'table' AND name" 
                              " = '%1%'";
    std::string sql_script = boost::str(boost::format{db_template} % table_name);
    pimpl_db_handler_->is_data_table_exist_ = (pimpl_db_handler_->RunSQLScript(sql_script) 
                                                      == SQLITE_OK);
  }
  // TODO: set to false after backup pimpl_db_handler_->is_data_table_exist_;
  return pimpl_db_handler_->is_data_table_exist_;
}

bool DataLoader::IsDBTableExist() {
  if (!pimpl_db_handler_->is_data_table_exist_) {
    std::string db_template = "SELECT COUNT(type) from sqlite_master WHERE type = 'table' AND name" 
                              " = '%1%'";
    std::string sql_script = boost::str(boost::format{db_template} % GetDBTableName());
    pimpl_db_handler_->is_data_table_exist_ = (pimpl_db_handler_->RunSQLScript(sql_script) 
                                                      == SQLITE_OK);
  }
  //TODO: set to false after backup pimpl_db_handler_->is_data_table_exist_;
  return pimpl_db_handler_->is_data_table_exist_;
}

bool DataLoader::IsDBTableTemplateExist() {
  return pimpl_db_handler_->is_db_table_template_exist_;
}

int32_t DataLoader::PimplDBHandler::RunSQLScript(const std::string &sql_script) {
  char *errMsg;
  auto callback = is_select_ ? DataLoader::PimplDBHandler::RetrieveDBValue : NULL;
  auto res = sqlite3_exec(*(database_.get()), 
                          sql_script.c_str(), DataLoader::PimplDBHandler::RetrieveDBValue, 0, &errMsg);
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

int DataLoader::PimplDBHandler::RetrieveDBValue(void *, int col, char **col_data, 
                                                  char **col_name) {
  std::vector<std::string> res;

  for (auto i = 0; i < col; ++i)
    res.push_back(std::move(std::string(col_data[i])));

  columns_ = std::make_pair(std::string(col_name[0]), res);

  return 0;
}

std::string DataLoader::PimplDBHandler::GetDBTableStructTemplateWithTypes() const {
  std::string result;
  std::size_t cnt{0};
  for (const auto &el : db_table_struct_template_) {
    result += el.first + " " + el.second;
    if (cnt < db_table_struct_template_.size() - 1)
      result += ", ";
    ++cnt;
  }

  return result;
}

bool DataLoader::PimplDBHandler::IsValListValid(const std::initializer_list<std::string> &val_lst) {
  if (db_table_struct_tamplate_size_ != val_lst.size())
    return false;
  return true;
}

} // namespace client
} // namespace crypt_wallet
