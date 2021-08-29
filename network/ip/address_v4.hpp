#pragma once
#include "../detail/common.hpp"

#include <array>
#include <string>

namespace network {
namespace ip {

/**
 * IPv4 address
 */
class address_v4
{
public:
    typedef network::detail::in4_addr_t addr_t;
    typedef network::detail::uint32_t uint_t;
    typedef std::array<unsigned char, network::detail::in4_addr_bytes_len> byte_t;


    address_v4() noexcept = default;

    explicit address_v4(addr_t _addr) noexcept
        : addr_(_addr)
    {
    }

    /// Uint type constructor
    /**
     * Creates address from uint in host byte order
     * @param _uint - uint in host byte order
     */
    explicit address_v4(uint_t _uint) noexcept
    {
        addr_.s_addr = htonl(_uint);
    }

    /// Bytes constructor
    /**
     * Creates address from bytes in network byte order
     * @param _bytes - bytes in network byte order
     */
    explicit address_v4(const byte_t& _bytes) noexcept
    {
        memcpy(&addr_, _bytes.data(), _bytes.size());
    }

    address_v4(const address_v4& _other) noexcept = default;
    address_v4(address_v4&& _other) noexcept = default;

    address_v4& operator=(addr_t _addr) noexcept
    {
        addr_ = _addr;
        return *this;
    }

    /// Uint type assign operator
    /**
     * Assign address from uint in host byte order
     */
    address_v4& operator=(uint_t _uint) noexcept
    {
        addr_.s_addr = htonl(_uint);
        return *this;
    }

    /// Bytes type assign operator
    /**
     * Assign address from bytes in network byte order
     */
    address_v4& operator=(const byte_t& _bytes) noexcept
    {
        memcpy(&addr_, _bytes.data(), _bytes.size());
        return *this;
    }

    address_v4& operator=(const address_v4& _other) noexcept = default;
    address_v4& operator=(address_v4&& _other) noexcept = default;

    friend bool operator==(const address_v4& _a, const address_v4& _b) noexcept
    {
        return _a.addr_.s_addr == _b.addr_.s_addr;
    }

    friend bool operator!=(const address_v4& _a, const address_v4& _b) noexcept
    {
        return !(_a == _b);
    }

    friend bool operator>(const address_v4& _a, const address_v4& _b) noexcept
    {
        return _a.addr_.s_addr > _b.addr_.s_addr;
    }

    friend bool operator>=(const address_v4& _a, const address_v4& _b) noexcept
    {
        return !(_b > _a);
    }

    friend bool operator<(const address_v4& _a, const address_v4& _b) noexcept
    {
        return !(_a >= _b);
    }

    friend bool operator<=(const address_v4& _a, const address_v4& _b) noexcept
    {
        return !(_a > _b);
    }


    /**
     * @return uint in host byte order
     */
    uint_t to_uint() const noexcept
    {
        return htonl(addr_.s_addr);
    }

    /**
     * @return bytes in network byte order
     */
    byte_t to_bytes() const noexcept
    {
        byte_t bytes;
        memcpy(bytes.data(), &addr_, bytes.size());
        return bytes;
    }


    addr_t addr() const noexcept
    {
        return addr_;
    }

    /**
     * @return adress string representation
     */
    std::string to_string() const
    {
        return inet_ntoa(addr_);
    }

    bool is_loopback() const noexcept
    {
        return addr_.s_addr == network::detail::inet_in4_addr_loopback;
    }

    static address_v4 loopback() noexcept
    {
        return address_v4(network::detail::in4_addr_loopback);
    }

private:
    addr_t addr_ { };
};

namespace detail {

network::ip::address_v4 to_address_v4(const char* _saddr, network::error* _error = nullptr) noexcept
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

address_v4 to_address_v4(const char* _saddr) noexcept
{
    return network::ip::detail::to_address_v4(_saddr);
}

address_v4 to_address_v4(const char* _saddr, network::error& _error) noexcept
{
    return network::ip::detail::to_address_v4(_saddr, &_error);
}

address_v4 to_address_v4(const std::string& _saddr) noexcept
{
    return network::ip::to_address_v4(_saddr.c_str());
}

address_v4 to_address_v4(const std::string& _saddr, network::error& _error) noexcept
{
    return network::ip::to_address_v4(_saddr.c_str(), _error);
}

} // namespace ip
} // namespace network