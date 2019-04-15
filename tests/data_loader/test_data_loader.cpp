/// \file test_data_loader.cpp
/// \brief Unit tests for data_loader module.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.1
/// \date 13.04.2019

#include <string>

#include <gtest/gtest.h>

#include "client/data_loader/data_loader.h"

namespace crypto_wallet {
namespace client {

TEST (DataLoaderClassTest, NameDataBase) {
  std::string db_name{"test_db"};
  DataLoader::GetDataLoaderInstance("test_db.db").SetDataBaseName(db_name);

  EXPECT_EQ(db_name, DataLoader::GetDataLoaderInstance("test_db.db").GetDataBaseName());
}

TEST (DataLoaderClassTest, DBTableStructTemplate) {
  std::string test_db_template_struct = "name1 LOGNTEXT, name2 LONGTEXT";
  std::initializer_list<std::string> test_lst = {"name1 LOGNTEXT", "name2 LONGTEXT"};
  DataLoader::GetDataLoaderInstance("test_db.db").SetDBTableStructTemplate(test_lst);
  
  EXPECT_EQ(test_db_template_struct,
            DataLoader::GetDataLoaderInstance("test_db.db").GetDBTableStructTemplate());
}

TEST (DataLoaderClassTest, CreateDBTable) {
  std::initializer_list<std::string> test_lst = {"name1 LOGNTEXT", "name2 LONGTEXT"};
  DataLoader::GetDataLoaderInstance("test_db.db").SetDBTableStructTemplate(test_lst);

  std::string db_table_name = "test_db_table";
  DataLoader::GetDataLoaderInstance("test_db.db").CreateDBTable(db_table_name);
  
  EXPECT_EQ(db_table_name, DataLoader::GetDataLoaderInstance("test_db.db").GetDataBaseTableName());
}

TEST (DataLoaderClassTest, InsertIntoTable) {
  std::initializer_list<std::string> test_lst = {"name1 LOGNTEXT", "name2 LONGTEXT"};
  DataLoader::GetDataLoaderInstance("test_db.db").SetDBTableStructTemplate(test_lst);

  std::string db_table_name = "test_db_table";
  DataLoader::GetDataLoaderInstance("test_db.db").CreateDBTable(db_table_name);

  std::initializer_list<std::string> test_val_lst = {"1", "2"};
  DataLoader::GetDataLoaderInstance("test_db.db").InsertIntoTable(test_val_lst);
}

}
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
