// This file is part of Poseidon.
// Copyleft 2020, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_NETWORK_ABSTRACT_TLS_CLIENT_SOCKET_HPP_
#define POSEIDON_NETWORK_ABSTRACT_TLS_CLIENT_SOCKET_HPP_

#include "abstract_tls_socket.hpp"
#include "socket_address.hpp"

namespace poseidon {

class Abstract_TLS_Client_Socket
  : public Abstract_TLS_Socket
  {
  public:
    explicit
    Abstract_TLS_Client_Socket(const Socket_Address& addr)
      : Abstract_TLS_Socket(addr.create_socket(SOCK_STREAM, IPPROTO_TCP),
                            noadl::get_client_ssl_ctx())
      { this->do_async_connect(addr);  }

    Abstract_TLS_Client_Socket(const char* host, uint16_t port)
      : Abstract_TLS_Client_Socket(Socket_Address(host, port))
      { }

    ASTERIA_NONCOPYABLE_DESTRUCTOR(Abstract_TLS_Client_Socket);

  private:
    // This functions is forbidden for derived classes.
    using Abstract_TLS_Socket::do_async_connect;

  public:
    using Abstract_Socket::get_fd;
    using Abstract_Socket::abort;
    using Abstract_Socket::get_local_address;

    using Abstract_Stream_Socket::get_remote_address;
    using Abstract_Stream_Socket::async_send;
    using Abstract_Stream_Socket::async_shutdown;
  };

}  // namespace poseidon

#endif
