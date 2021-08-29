#pragma once

#include <array>
#if defined(_WIN32)

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>

#else

#include <arpa/inet.h>

#endif

namespace network {
namespace detail {

typedef unsigned long uint32_t;

typedef in_addr in4_addr_t;
const uint32_t in4_addr_loopback = INADDR_LOOPBACK;
const uint32_t inet_in4_addr_loopback = htonl(in4_addr_loopback);
const unsigned in4_addr_bytes_len = sizeof(in4_addr_t);
const unsigned in4_addr_str_len = 16;

typedef in6_addr in6_addr_t;
const unsigned in6_addr_bytes_len = sizeof(in6_addr);
const std::array<unsigned char, in6_addr_bytes_len> in6_addr_loopback
    {0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1};
const uint32_t in6_scope_id_loopback = 128;
const unsigned in6_addr_str_len = 256;   // TODO

int last_error() noexcept
{
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}



} // namespace detail

#ifndef NO_ERROR
#define NO_ERROR 0
#endif

struct error
{
    typedef int error_t;

    error_t value { };

    operator error_t() const noexcept
    {
        return value;
    }
};

} // namespace network