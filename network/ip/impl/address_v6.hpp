#pragma once

///                              Constructors

address_v6::address_v6(address_v6::addr_t _addr, address_v6::scope_id_t _scope_id) noexcept
    : addr_(_addr)
    , scope_id_(_scope_id)
{
}

/// Creates address from bytes in a network byte order and scope ip.
address_v6::address_v6(const address_v6::byte_t& _bytes, address_v6::scope_id_t _scope_id) noexcept
    : scope_id_(_scope_id)
{
    memcpy(&addr_, _bytes.data(), _bytes.size());
}

///                              Constructors


///                             Conversions

/// Byte address in a network byte order.
address_v6::byte_t address_v6::to_bytes() const noexcept
{
    byte_t bytes;
    memcpy(bytes.data(), &addr_, bytes.size());
    return bytes;
}

/// Colon-delimited address string representation.
std::string address_v6::to_string() const
{
    char buff[network::detail::in6_addr_str_len]{ };
    return inet_ntop(AF_INET6, &addr_, buff, network::detail::in6_addr_bytes_len);
}

///                             Conversions


///                             Properties

/// Native ipv6 address.
address_v6::addr_t address_v6::addr() const noexcept
{
    return addr_;
}

/// Address scope id.
address_v6::scope_id_t address_v6::scope_id() const noexcept
{
    return scope_id_;
}

/// If address is ipv6 loopback (i.e ::1).
bool address_v6::is_loopback() const noexcept
{
    using namespace network::detail;
    return memcmp(&addr_, in6_addr_loopback.data(), in6_addr_bytes_len) == 0;
}

///                             Properties

///                             Comparison operators

bool operator==(const address_v6& _a, const address_v6& _b) noexcept
{
    return memcmp(&_a.addr_, &_b.addr_, network::detail::in6_addr_bytes_len) == 0;
}

bool operator!=(const address_v6& _a, const address_v6& _b) noexcept
{
    return !(_a == _b);
}

bool operator>(const address_v6& _a, const address_v6& _b) noexcept
{
    return memcmp(&_a.addr_, &_b.addr_, network::detail::in6_addr_bytes_len) > 0;
}

bool operator>=(const address_v6& _a, const address_v6& _b) noexcept
{
    return !(_b > _a);
}

bool operator<(const address_v6& _a, const address_v6& _b) noexcept
{
    return !(_a >= _b);
}

bool operator<=(const address_v6& _a, const address_v6& _b) noexcept
{
    return !(_a > _b);
}

///                             Comparison operators


///                             Static

/// Loopback ipv6 address.
address_v6 address_v6::loopback() noexcept
{
    using namespace network::detail;
    return address_v6(in6_addr_loopback, in6_scope_id_loopback);
}

///                             Static



namespace detail {

/**
 * Creates address_v6 from cstring ipv6 address and puts error if occured in _error
 * @param _saddr - cstring ipv6 address
 * @param _error - slot for error handling(by default nullptr)
 * @return cstring ipv6 address address_v6 representation
 */
network::ip::address_v6 to_address_v6(const char* _saddr, network::error* _error) noexcept
{
    using namespace network::detail;
    in6_addr addr{ };
    int tmp = inet_pton(AF_INET6, _saddr, &addr);
    if( _error ) {
        _error->value = tmp == 1 ? NO_ERROR : last_error();
    }
    return network::ip::address_v6(addr);
}

} // namespace detail

/**
 * Creates address_v6 from cstring ipv6 address without error handling
 * @param _saddr - cstring ipv6 address
 * @return cstring ipv6 address address_v6 representation
 */
network::ip::address_v6 to_address_v6(const char* _saddr) noexcept
{
    return network::ip::detail::to_address_v6(_saddr);
}

/**
 * Creates address_v6 from cstring ipv4 address with error handling
 * @param _saddr - cstring ipv6 address
 * @param _error - slot for error handling
 * @return cstring ipv6 address address_v6 representation
 */
network::ip::address_v6 to_address_v6(const char* _saddr, network::error& _error) noexcept
{
    return network::ip::detail::to_address_v6(_saddr, &_error);
}

/**
 * Creates address_v6 from string ipv6 address without error handling
 * @param _saddr - string ipv6 address
 * @return string ipv6 address address_v6 representation
 */
network::ip::address_v6 to_address_v6(const std::string& _saddr) noexcept
{
    return network::ip::to_address_v6(_saddr.c_str());
}

/**
 * Creates address_v6 from string ipv6 address with error handling
 * @param _saddr - string ipv6 address
 * @param _error - slot for error handling
 * @return string ipv6 address address_v6 representation
 */
network::ip::address_v6 to_address_v6(const std::string& _saddr, network::error& _error) noexcept
{
    return network::ip::to_address_v6(_saddr.c_str(), _error);
}

template<class _CharT, class _Traits>
std::basic_ostream<_CharT, _Traits>& operator<<(std::basic_ostream<_CharT, _Traits>& _os, const address_v6& _address)
{
    return _os << _address.to_string();
}