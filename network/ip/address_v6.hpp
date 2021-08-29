#pragma once
#include "../detail/common.hpp"
#include <string>

namespace network {
namespace ip {


class address_v6
{
public:
    typedef network::detail::in6_addr_t addr_t;
    typedef network::detail::uint32_t scope_id_t;
    typedef std::array<unsigned char, network::detail::in6_addr_bytes_len> byte_t;

    address_v6() noexcept = default;

    explicit address_v6(addr_t _addr, scope_id_t _scope_id = 0) noexcept
        : addr_(_addr)
        , scope_id_(_scope_id)
    {
    }

    explicit address_v6(const byte_t& _bytes, scope_id_t _scope_id = 0) noexcept
        : scope_id_(_scope_id)
    {
        memcpy(&addr_, _bytes.data(), _bytes.size());
    }

    address_v6(const address_v6& _other) noexcept = default;
    address_v6(address_v6&& _other) noexcept = default;

    address_v6& operator=(const address_v6& _other) noexcept = default;
    address_v6& operator=(address_v6&& _other) noexcept = default;

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

    scope_id_t scope_id() const noexcept
    {
        return scope_id_;
    }

    bool is_loopback() const noexcept
    {
        using namespace network::detail;
        return memcmp(&addr_, in6_addr_loopback.data(), in6_addr_bytes_len) == 0;
    }

    static address_v6 loopback() noexcept
    {
        using namespace network::detail;
        return address_v6(in6_addr_loopback, in6_scope_id_loopback);
    }

    std::string to_string() const
    {
        char buff[network::detail::in6_addr_str_len]{ };
        return inet_ntop(AF_INET6, &addr_, buff, network::detail::in6_addr_bytes_len);
    }


    friend bool operator==(const address_v6& _a, const address_v6& _b) noexcept
    {
        return memcmp(&_a.addr_, &_b.addr_, network::detail::in6_addr_bytes_len) == 0;
    }

    friend bool operator!=(const address_v6& _a, const address_v6& _b) noexcept
    {
        return !(_a == _b);
    }

    friend bool operator>(const address_v6& _a, const address_v6& _b) noexcept
    {
        return memcmp(&_a.addr_, &_b.addr_, network::detail::in6_addr_bytes_len) > 0;
    }

    friend bool operator>=(const address_v6& _a, const address_v6& _b) noexcept
    {
        return !(_b > _a);
    }

    friend bool operator<(const address_v6& _a, const address_v6& _b) noexcept
    {
        return !(_a >= _b);
    }

    friend bool operator<=(const address_v6& _a, const address_v6& _b) noexcept
    {
        return !(_a > _b);
    }

private:
    addr_t addr_ { };
    scope_id_t scope_id_ { };
};

namespace detail {

network::ip::address_v6 to_address_v6(const char* _saddr, network::error* _error = nullptr) noexcept
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

network::ip::address_v6 to_address_v6(const char* _saddr) noexcept
{
    return network::ip::detail::to_address_v6(_saddr);
}

network::ip::address_v6 to_address_v6(const char* _saddr, network::error& _error) noexcept
{
    return network::ip::detail::to_address_v6(_saddr, &_error);
}

network::ip::address_v6 to_address_v6(const std::string& _saddr) noexcept
{
    return network::ip::to_address_v6(_saddr.c_str());
}

network::ip::address_v6 to_address_v6(const std::string& _saddr, network::error& _error) noexcept
{
    return network::ip::to_address_v6(_saddr.c_str(), _error);
}

} // namespace ip
} // namespace network