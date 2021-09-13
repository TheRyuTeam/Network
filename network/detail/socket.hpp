#include "common.hpp"

namespace network {
namespace detail {

#define GOOD 0

bool set_error(bool _f, network::error& _error) noexcept
{
    if( _f ) {
        _error.clear();
        return true;
    }
    _error = last_error();
    return false;
}

template<class _Endpoint>
bool bind(socket_t _s, const _Endpoint& _ep) noexcept
{
    return ::bind(_s, _ep, _ep.size()) == GOOD;
}

template<class _Endpoint>
bool bind(socket_t _s, const _Endpoint& _ep, network::error& _error) noexcept
{
    return set_error(bind(_s, _ep), _error);
}

bool listen(socket_t _s, int _n = INT_MAX) noexcept
{
    return ::listen(_s, _n) == GOOD;
}

bool listen(socket_t _s, network::error& _error, int _n = INT_MAX) noexcept
{
    return set_error(listen(_s, _n), _error);
}

bool accept(socket_t _s, socket_t& _accepted) noexcept
{
    socket_t s = ::accept(_s, nullptr, nullptr);
    if( s != invalid_socket ) {
        _accepted = s;
        return true;
    }
    return false;
}


bool accept(socket_t _s, socket_t& _accepted, network::error& _error) noexcept
{
    socket_t s = ::accept(_s, nullptr, nullptr);
    if( s != network::detail::invalid_socket ) {
        _accepted = s;
        _error.clear();
        return true;
    }
    _error = last_error();
    return false;
}

template<class _Endpoint>
bool accept(socket_t _s, socket_t& _accepted, _Endpoint& _ep) noexcept
{
    _Endpoint tmp_ep;
    int size = sizeof(sockaddr_in6);
    socket_t s = ::accept(_s, tmp_ep, &size);
    if( s != invalid_socket ) {
        _accepted = s;
        _ep = std::move(tmp_ep);
        return true;
    }
    return false;
}

template<class _Endpoint>
bool accept(socket_t _s, socket_t& _accepted, _Endpoint& _ep, network::error& _error) noexcept
{
    return set_error(accept(_s, _accepted), _error);
}

template<class _Endpoint>
bool connect(socket_t _s, const _Endpoint& _ep) noexcept
{
    return ::connect(_s, _ep, _ep.size()) == GOOD;
}

template<class _Endpoint>
bool connect(socket_t _s, const _Endpoint& _ep, network::error& _error) noexcept
{
    return set_error(connect(_s, _ep), _error);
}

bool close(socket_t _s) noexcept
{
#ifdef __WIN32
    return !::closesocket(_s);
#else
    return !::close(_s);
#endif
}
} // namespace network::detail
} // namespace network