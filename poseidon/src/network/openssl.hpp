// This file is part of Poseidon.
// Copyleft 2020, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_NETWORK_OPENSSL_HPP_
#define POSEIDON_NETWORK_OPENSSL_HPP_

#include "../fwd.hpp"
#include <openssl/ssl.h>

namespace poseidon {

// RAII
struct SSL_CTX_deleter
  {
    void
    operator()(::SSL_CTX* ctx)
    noexcept
      { ::SSL_CTX_free(ctx);  }
  };

using unique_SSL_CTX = uptr<::SSL_CTX, SSL_CTX_deleter>;

struct SSL_deleter
  {
    void
    operator()(::SSL* ssl)
    noexcept
      { ::SSL_free(ssl);  }
  };

using unique_SSL = uptr<::SSL, SSL_deleter>;

// Prints all errors in OpenSSL error queue, then clears it.
// Returns the number of errors that have been dumped.
size_t
dump_ssl_errors()
noexcept;

#define POSEIDON_SSL_THROW(...)   (::poseidon::dump_ssl_errors(), POSEIDON_THROW(__VA_ARGS__))

// Creates an `SSL_CTX` object for server use.
// If neither `cert_opt` nor `pkey_opt` is null, they shall denote PEM files containing the
// certificate chain and private key, respectively.
// If both `cert_opt` and `pkey_opt` are null, the default ones in `main.conf` are used.
unique_SSL_CTX
create_server_ssl_ctx(const char* cert_opt, const char* pkey_opt);

// Gets the static `SSL_CTX` object for client use.
::SSL_CTX*
get_client_ssl_ctx();

// Creates an `SSL` structure for a socket.
unique_SSL
create_ssl(::SSL_CTX* ctx, int fd);

}  // namespace poseidon

#endif
