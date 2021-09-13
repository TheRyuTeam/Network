#pragma once

///                              Constructors

address_v4::address_v4(address_v4::addr_t _addr) noexcept
    : addr_(_addr)
{
}

/// Creates address from uint in a host byte order.
address_v4::address_v4(address_v4::uint_t _uint) noexcept
{
    addr_.s_addr = _uint;
}

/// Creates address from bytes in a network byte order.
address_v4::address_v4(const address_v4::byte_t& _bytes) noexcept
{
    memcpy(&addr_, _bytes.data(), _bytes.size());
}

///                             Constructors


///                             Assign operators

address_v4& address_v4::operator=(address_v4::addr_t _addr) noexcept
{
    addr_ = _addr;
    return *this;
}

/// Assigns uint address in a host byte order.
address_v4& address_v4::operator=(address_v4::uint_t _uint) noexcept
{
    addr_.s_addr = htonl(_uint);
    return *this;
}

/// Assigns byte address in a network byte order.
address_v4& address_v4::operator=(const address_v4::byte_t& _bytes) noexcept
{
    memcpy(&addr_, _bytes.data(), _bytes.size());
    return *this;
}

///                             Assign operators


///                             Conversions

/// Uint address in a host byte order.
address_v4::uint_t address_v4::to_uint() const noexcept
{
    return htonl(addr_.s_addr);
}

/// Byte address in a network byte order.
address_v4::byte_t address_v4::to_bytes() const noexcept
{
    byte_t bytes;
    memcpy(bytes.data(), &addr_, bytes.size());
    return bytes;
}

/// Dot-delimited address string representation.
std::string address_v4::to_string() const
{
    return inet_ntoa(addr_);
}

///                             Conversions


///                             Properties

/// Native ipv4 address.
address_v4::addr_t address_v4::addr() const noexcept
{
    return addr_;
}

bool address_v4::is_unicast() const noexcept
{
    return true;
}

bool address_v4::is_multicast() const noexcept
{
    return address_class() == Class::D;
}

bool address_v4::is_broadcast() const noexcept
{
    return true;
}


address_v4::Class address_v4::address_class() const noexcept
{
    // TODO: Improve this sh*t!
    auto b = ((unsigned char*)&addr_)[0];
    if( b == static_cast<unsigned char>(Class::This) )
        return Class::This;
    if( b == static_cast<unsigned char>(Class::Loopback) )
        return Class::Loopback;
    if( b < static_cast<unsigned char>(Class::A) )
        return Class::A;
    if( b < static_cast<unsigned char>(Class::B) )
        return Class::B;
    if( b < static_cast<unsigned char>(Class::C) )
        return Class::C;
    return b < static_cast<unsigned char>(Class::D) ? Class::D : Class::E;
}

address_v4::uint_t address_v4::network_address() const noexcept
{
    Class addr_class = address_class();
    if( addr_class == Class::A )
        return to_uint() & static_cast<uint_t>(Mask::A);
    return to_uint() & static_cast<uint_t>(addr_class == Class::B ? Mask::B : Mask::C);
}

/// If address is ipv4 loopback (i.e 127.0.0.1).
bool address_v4::is_loopback() const noexcept
{
    return addr_.s_addr == network::detail::inet_in4_addr_loopback;
}

///                             Properties


///                             Comparison operators

bool operator==(const address_v4& _a, const address_v4& _b) noexcept
{
    return _a.addr_.s_addr == _b.addr_.s_addr;
}

bool operator!=(const address_v4& _a, const address_v4& _b) noexcept
{
    return !(_a == _b);
}

bool operator>(const address_v4& _a, const address_v4& _b) noexcept
{
    return _a.addr_.s_addr > _b.addr_.s_addr;
}

bool operator>=(const address_v4& _a, const address_v4& _b) noexcept
{
    return !(_b > _a);
}

bool operator<(const address_v4& _a, const address_v4& _b) noexcept
{
    return !(_a >= _b);
}

bool operator<=(const address_v4& _a, const address_v4& _b) noexcept
{
    return !(_a > _b);
}

///                             Comparison operators


///                             Static

/// Loopback ipv4 address.
address_v4 address_v4::loopback() noexcept
{
    return address_v4(network::detail::in4_addr_loopback);
}

///                             Static


namespace detail {

/**
 * Creates address_v4 from cstring ipv4 address and puts error if occured in _error
 * @param _saddr - cstring ipv4 address
 * @param _error - slot for error handling(by default nullptr)
 * @return cstring ipv4 address address_v4 representation
 */
network::ip::address_v4 to_address_v4(const char* _saddr, network::error* _error) noexcept
{
    using namespace network::detail;
    in4_addr_t addr{ };
    int tmp = inet_pton(AF_INET, _saddr, &addr);
    if( _error ) {
        _error->value = tmp == 1 ? NO_ERROR : last_error();
    }
    return network::ip::address_v4(addr);
}

} // namespace detail

/**
 * Creates address_v4 from cstring ipv4 address without error handling
 * @param _saddr - cstring ipv4 address
 * @return cstring ipv4 address address_v4 representation
 */
address_v4 to_address_v4(const char* _saddr) noexcept
{
    return network::ip::detail::to_address_v4(_saddr);
}

/**
 * Creates address_v4 from cstring ipv4 address with error handling
 * @param _saddr - cstring ipv4 address
 * @param _error - slot for error handling
 * @return cstring ipv4 address address_v4 representation
 */
address_v4 to_address_v4(const char* _saddr, network::error& _error) noexcept
{
    return network::ip::detail::to_address_v4(_saddr, &_error);
}

/**
 * Creates address_v4 from string ipv4 address without error handling
 * @param _saddr - string ipv4 address
 * @return string ipv4 address address_v4 representation
 */
address_v4 to_address_v4(const std::string& _saddr) noexcept
{
    return network::ip::to_address_v4(_saddr.c_str());
}

/**
 * Creates address_v4 from string ipv4 address with error handling
 * @param _saddr - string ipv4 address
 * @param _error - slot for error handling
 * @return string ipv4 address address_v4 representation
 */
address_v4 to_address_v4(const std::string& _saddr, network::error& _error) noexcept
{
    return network::ip::to_address_v4(_saddr.c_str(), _error);
}


template<class _CharT, class _Traits>
std::basic_ostream<_CharT, _Traits>& operator<<(std::basic_ostream<_CharT, _Traits>& _os, const address_v4& _address)
{
    return _os << _address.to_string();
}