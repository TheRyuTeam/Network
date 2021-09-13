#pragma once


///                              Constructors

address::address(address_v4&& _v4) noexcept
    : v4_(std::move(_v4))
{
}

address::address(address_v6&& _v6) noexcept
    : v6_(std::move(_v6))
    , is_v4_(false)
{
}

address::address(const address_v4& _v4) noexcept
    : v4_(_v4)
{
}

address::address(const address_v6& _v6) noexcept
    : v6_(_v6)
    , is_v4_(false)
{
}

///                              Constructors


///                             Assign operators

address& address::operator=(address_v4&& _v4) noexcept
{
    v4_ = std::move(_v4);
    is_v4_ = true;
    return *this;
}

address& address::operator=(const address_v4& _v4) noexcept
{
    v4_ = _v4;
    is_v4_ = true;
    return *this;
}

address& address::operator=(address_v6&& _v6) noexcept
{
    v6_ = std::move(_v6);
    is_v4_ = false;
    return *this;
}

address& address::operator=(const address_v6& _v6) noexcept
{
    v6_ = _v6;
    is_v4_ = false;
    return *this;
}

///                             Assign operators


///                             Conversions

/// Address string representation.
std::string address::to_string() const
{
    return is_v4_ ? v4_.to_string() : v6_.to_string();
}

///                             Conversions


///                             Properties

/// Is address ipv4.
bool address::is_v4() const noexcept
{
    return is_v4_;
}

/// Is address loopback.
bool address::is_loopback() const noexcept
{
    return is_v4_ ? v4_.is_loopback() : v6_.is_loopback();
}


address_v4 address::v4() const noexcept
{
    return v4_;
}

address_v6 address::v6() const noexcept
{
    return v6_;
}

///                             Properties


namespace detail {

/**
 * Creates address from cstring ipv4/ipv6 address and puts error if occured in _error
 * @param _saddr - cstring ipv4/ipv6 address
 * @param _error - slot for error handling(by default nullptr)
 * @return cstring address address class representation
 */
network::ip::address to_address(const char* _saddr, network::error* _error) noexcept
{
    network::detail::in4_addr_t in4_addr;
    int tmp = inet_pton(AF_INET, _saddr, &in4_addr);
    if( tmp != 1 ) {
        network::detail::in6_addr_t in6_addr;
        tmp = inet_pton(AF_INET6, _saddr, &in6_addr);
        if( tmp != 1 ) {
            if( _error )
                _error->value = network::detail::last_error();
        }
        else {
            if( _error )
                _error->value = NO_ERROR;
            return network::ip::address_v6(in6_addr);
        }
    }
    else {
        if( _error )
            _error->value = NO_ERROR;
        return network::ip::address_v4(in4_addr);
    }
    return { };
}

} // namespace detail

/**
 * Creates address from cstring ipv4/ipv6 address without error handling
 * @param _saddr - cstring ipv4/ipv6 address
 * @return cstring ipv4/ipv6 address address representation
 */
network::ip::address to_address(const char* _saddr) noexcept
{
    return network::ip::detail::to_address(_saddr);
}

/**
 * Creates address from cstring ipv4/ipv6 address with error handling
 * @param _saddr - cstring ipv4/ipv6 address
 * @param _error - slot for error handling
 * @return cstring ipv4/ipv6 address address representation
 */
network::ip::address to_address(const char* _saddr, network::error& _error) noexcept
{
    return network::ip::detail::to_address(_saddr, &_error);
}

/**
 * Creates address from string ipv4/ipv6 address without error handling
 * @param _saddr - string ipv4/ipv6 address
 * @return string ipv4/ipv6 address address representation
 */
network::ip::address to_address(const std::string& _saddr) noexcept
{
    return network::ip::to_address(_saddr.c_str());
}

/**
 * Creates address from string ipv4/ipv6 address with error handling
 * @param _saddr - string ipv4/ipv6 address
 * @param _error - slot for error handling
 * @return string ipv4/ipv6 address address representation
 */
network::ip::address to_address(const std::string& _saddr, network::error& _error) noexcept
{
    return network::ip::to_address(_saddr.c_str(), _error);
}

template<class _CharT, class _Traits>
std::basic_ostream<_CharT, _Traits>& operator<<(std::basic_ostream<_CharT, _Traits>& _os, const address& _address)
{
    return _os << _address.to_string();
}