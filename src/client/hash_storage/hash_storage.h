/// \file hash_storage.h
/// \brief Keep frequently used data.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.0
/// \date 01.05.2019

#ifndef CRYPTO_WALLET_HASH_STORAGE_H_
#define CRYPTO_WALLET_HASH_STORAGE_H_

#include <map>
#include <string>

/// \namesapce crypto_wallet.
/// \brief Project namesapce.
namespace crypto_wallet {

/// \namespace client.
/// \brief Client namespace.
namespace client {

class HashStorage {
 public:
  /// \brief Return corresponding data.
  /// \param[in] value Value.
  /// \return Data.
  std::string GetData(const std::string &value) const noexcept {
    auto found = storage_.find(value);
    if (found != storage_.end())
      return found->second;
  }
  
  /// \brief Save data into temporary storage.
  /// \param[in] value Value.
  /// \param[in] data Data.
  void SetData(const std::string &value, const std::string &data);
  
  bool IsValueStored(const std::string &value) const noexcept {
    return (storage_.find(value) != storage_.end());
  }

 private:
  std::map<std::string, std::string> storage_;

};

} // namespace client

} // namespace crypto_wallet

#endif // CRYPTO_WALLET_HASH_STORAGE_H_
