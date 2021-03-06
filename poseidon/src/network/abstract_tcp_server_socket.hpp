// This file is part of Poseidon.
// Copyleft 2020, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_NETWORK_ABSTRACT_TCP_SERVER_SOCKET_HPP_
#define POSEIDON_NETWORK_ABSTRACT_TCP_SERVER_SOCKET_HPP_

#include "abstract_listen_socket.hpp"
#include "socket_address.hpp"
#include "abstract_tcp_socket.hpp"

namespace poseidon {

class Abstract_TCP_Server_Socket
  : public Abstract_Listen_Socket
  {
  public:
    explicit
    Abstract_TCP_Server_Socket(const Socket_Address& addr)
      : Abstract_Listen_Socket(addr.create_socket(SOCK_STREAM, IPPROTO_TCP))
      { this->do_listen(addr);  }

    Abstract_TCP_Server_Socket(const char* bind, uint16_t port)
      : Abstract_TCP_Server_Socket(Socket_Address(bind, port))
      { }

    ASTERIA_NONCOPYABLE_DESTRUCTOR(Abstract_TCP_Server_Socket);

  private:
    uptr<Abstract_Socket>
    do_on_async_accept(unique_FD&& fd)
    final;

    void
    do_on_async_register(rcptr<Abstract_Socket>&& sock)
    final;

  protected:
    // Consumes an accepted socket.
    // This function shall allocate and return a new socket object.
    // Please mind thread safety, as this function is called by the network thread.
    virtual
    uptr<Abstract_TCP_Socket>
    do_on_async_accept_tcp(unique_FD&& fd)
      = 0;

    // Registers a socket object.
    // This function shall ensure `sock` is not orphaned by storing the pointer
    // somewhere (for example into a user-defined client map).
    // Please mind thread safety, as this function is called by the network thread.
    virtual
    void
    do_on_async_register_tcp(rcptr<Abstract_TCP_Socket>&& sock)
      = 0;

  public:
    using Abstract_Socket::get_fd;
    using Abstract_Socket::abort;
    using Abstract_Socket::get_local_address;
  };

}  // namespace poseidon

#endif
