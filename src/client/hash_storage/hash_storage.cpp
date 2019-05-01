/// \file hash_storage.cpp
/// \brief Source file for class HashStorage.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.0
/// \data 01.05.2019

#include "hash_storage.h"

namespace crypto_wallet {

namespace client {

void HashStorage::SetData(const std::string &value, const std::string &data) {
  if (!IsValueStored(value))
    storage_[value] = data;
}

} // namespace client

} // namespace crypto_wallet
