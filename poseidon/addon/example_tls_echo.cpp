// This file is part of Poseidon.
// Copyleft 2020, LH_Mouse. All wrongs reserved.

#include "../src/precompiled.hpp"
#include "../src/network/abstract_tls_server_socket.hpp"
#include "../src/network/abstract_tls_socket.hpp"
#include "../src/static/network_driver.hpp"
#include "../src/utilities.hpp"

namespace {

using namespace poseidon;

struct Example_Session : Abstract_TLS_Socket
  {
    explicit
    Example_Session(unique_FD&& fd, ::SSL_CTX* ctx)
      : Abstract_TLS_Socket(::std::move(fd), ctx)
      { }

    void
    do_on_async_receive(void* data, size_t size)
    override
      {
        POSEIDON_LOG_WARN("example TLS client received: $1",
                          cow_string(static_cast<char*>(data), size));

        this->async_send(data, size);
      }
  };

rcptr<Abstract_TLS_Socket> s_client;  // only one client is allowed

constexpr char bind[] = "0.0.0.0";
constexpr uint16_t port = 3808;

struct Example_Server : Abstract_TLS_Server_Socket
  {
    Example_Server()
      : Abstract_TLS_Server_Socket(bind, port)
      {
        POSEIDON_LOG_WARN("example TLS server listening: $1",
                          this->get_local_address());
      }

    uptr<Abstract_TLS_Socket>
    do_on_async_accept_tls(unique_FD&& fd, ::SSL_CTX* ctx)
    override
      {
        return ::rocket::make_unique<Example_Session>(::std::move(fd), ctx);
      }

    void
    do_on_async_register_tls(rcptr<Abstract_TLS_Socket>&& sock)
      {
        POSEIDON_LOG_WARN("example TLS server accepted client: $1",
                          sock->get_remote_address());

        s_client = ::std::move(sock);
      }
  };

const auto s_server = Network_Driver::insert(::rocket::make_unique<Example_Server>());

}  // namespace
