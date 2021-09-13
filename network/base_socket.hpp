#pragma once
#include "ip/endpoint.hpp"
#include "detail/socket.hpp"

namespace network {

class executor
{
public:
    executor() noexcept
    {
        ::WSAStartup(MAKEWORD(2,2), &data_);
    }

    ~executor() noexcept
    {
        ::WSACleanup();
    }

private:
    WSAData data_ { };
};

#define BASE_SOCKET_METHODS_MACRO(Name)                                 \
template<class... _Args>                                                \
bool Name(_Args&&... _args) const noexcept                              \
{                                                                       \
    return network::detail::Name(s_, std::forward<_Args>(_args)...);    \
}

class base_socket : executor
{
public:
    base_socket(network::detail::socket_t _s = network::detail::invalid_socket) noexcept
        : s_(_s)
    {
    }
    base_socket(int _af, int _type, int _proto = 0) noexcept
        :s_(::socket(_af, _type, _proto))
    {
    }

    base_socket(base_socket&& _other) noexcept
        : s_(_other.exchange())
    {
    }

    base_socket(const base_socket& _other) noexcept = default;

    ~base_socket() noexcept
    {
        close();
    }

    base_socket& operator=(network::detail::socket_t _s) noexcept
    {
        s_ = _s;
        return *this;
    }
    base_socket& operator=(base_socket&& _other) noexcept
    {
        s_ = _other.exchange();
        return *this;
    }
    base_socket& operator=(const base_socket& _other) noexcept = default;


    network::detail::socket_t socket() const noexcept
    {
        return s_;
    }

    operator network::detail::socket_t() const noexcept
    {
        return socket();
    }


    network::detail::socket_t exchange(network::detail::socket_t _s = network::detail::invalid_socket) noexcept
    {
        auto tmp = s_;
        s_ = _s;
        return tmp;
    }

    BASE_SOCKET_METHODS_MACRO(close)
    BASE_SOCKET_METHODS_MACRO(bind)
    BASE_SOCKET_METHODS_MACRO(listen)
    BASE_SOCKET_METHODS_MACRO(connect)

    template<class... _Args>
    bool accept(base_socket& _s, _Args&&... _args) const noexcept
    {
        return network::detail::accept(s_, _s.s_, std::forward<_Args>(_args)...);
    }

private:
    network::detail::socket_t s_;
};

} // namespace network