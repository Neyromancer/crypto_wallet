/// \file uri_parser.cpp
/// \brief Class UriParser source file.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex.ru>
/// \version 1.0.0.1
/// \date 14.06.2019

#include "uri_parser.h" 

namespace crypto_wallet {

namespace client {

struct UriParser::PimplUriParser {
  /// \brief Set scheme.
  /// \param[in] uri URI.
  void set_scheme( const std::string &uri );

  /// \brief Set authority.
  /// \param[in] uri URI.
  void set_authority( const std::string &uri );

  /// \brief Set path.
  /// \param[in] uri URI.
  void set_path( const std::string &uri );

  /// \brief Set query.
  /// \param[in] uri URI.
  void set_query( const std::string &uri );

  /// \brief Set fragment.
  /// \param[in] uri URI
  void set_fragment( const std::string &uri );

  std::string scheme_;
  std::string authority_;
  std::string path_;
  std::string query_;
  std::string fragment_;
};

void UriParser::parse( const std::string &uri ) {
  if ( uri.empty() )
    throw std::invalid_argument("Empty uri sequence passed.");

  pimpl_uri_parser_->set_scheme( uri );
  pimpl_uri_parser_->set_authority( uri );
  pimpl_uri_parser_->set_path( uri );
  pimpl_uri_parser_->set_query( uri );
  pimpl_uri_parser_->set_fragment( uri );
}

UriParser::UriParser() : pimpl_uri_parser_{ std::make_shared<PimplUriParser>() } {}

const std::string &UriParser::get_scheme() const noexcept {
  return pimpl_uri_parser_->scheme_;
}

const std::string &UriParser::get_authority() const noexcept {
  return pimpl_uri_parser_->authority_;
}

const std::string &UriParser::get_path() const noexcept {
  return pimpl_uri_parser_->path_;
}

const std::string &UriParser::get_query() const noexcept {
  return pimpl_uri_parser_->query;
}

const std::string &UriParser::get_fragment() const noexcept {
  return pimpl_uri_parser_->fragment;
}

} // namespace client
} // namespace crypto_wallet
