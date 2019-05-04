/// \file uri_parser.h
/// \brief Class responsible for parsing URI.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.1
/// \date 14.06.2019

#ifndef CRYPTO_WALLET_CLIENT_URI_PARSER_H_
#define CRYPTO_WALLET_CLIENT_URI_PARSER_H_

#include <memory>
#include <string>

/// \namespace crypto_wallet.
/// \brief Project namespace.
namespace crypto_wallet {

/// \namesapce clietn.
/// \brief Clietn namespace.
namespace client {

/// \class UriParser uri_parser.h.
/// \brief Class responsible for parsing URI.
class UriParser {
 public:
  /// \brief UriParser constrictor.
  UriParser();

  /// \brief UriParser destructor.
  ~UriParser() = default;

  /// \brief Parse passed string.
  /// \param[in] uri URI sequence.
  void parse( const std::string &uri );

  /// \brief Get scheme.
  /// \return scheme.
  /// \retval
  ///    "" - empty string is returned in case of errors.
  const std::string &get_scheme() const noexcept;

  /// \brief Get authority.
  /// \return authority.
  /// \retval
  ///    "" - empty string is returned in case of errors.
  const std::string &get_authority() const noexcept;

  /// \brief Get path.
  /// \return path.
  /// \retval
  ///    "" - empty string is returned in case of errors.
  const std::string &get_path() const noexcept;

  /// \brief Get query.
  /// \return query.
  /// \retval
  ///    "" - empty string is returned in case of errors.
  const std::string &get_query() const noexcept;  

  /// \brief Get fragment.
  /// \return fragment.
  /// \retval
  ///    "" - empty string is returned in case of errors.
  const std::string &get_fragment() const noexcept;

  struct PimplUriParser;
  std::shared_ptr<PimplUriParser> pimpl_uri_parser_;
};

} // namespace client
} // namespace crypto_wallet

#endif // CRYPTO_WALLET_CLIENT_URI_PARSER_H_
