/// \file cash_storage.cpp
/// \brief Source file for class CashStorage.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.0
/// \data 01.05.2019

#include "Cash_storage.h"

namespace crypto_wallet {

namespace client {

void CashStorage::SetData(const std::string &value, const std::string &data) {
  if (!IsValueStored(value))
    storage_[value] = data;
}

} // namespace client

} // namespace crypto_wallet
