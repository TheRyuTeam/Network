#pragma once
#include "address_v4.hpp"
#include "address_v6.hpp"

namespace network {
namespace ip {

class address
{
public:
    address() noexcept = default;
    address(address_v4 _v4) noexcept
        : v4_(_v4)
    {
    }

    address(address_v6 _v6) noexcept
        : v6_(_v6)
        , is_v4_(false)
    {
    }

    bool is_v4() const noexcept
    {
        return is_v4_;
    }

    bool is_loopback() const noexcept
    {
        return is_v4_ ? v4_.is_loopback() : v6_.is_loopback();
    }

    std::string to_string() const
    {
        return is_v4_ ? v4_.to_string() : v6_.to_string();
    }

private:
    address_v4 v4_ { };
    address_v6 v6_ { };
    bool is_v4_ { true };
};


template<typename Tp>
std::string to_string(const Tp& _arg)
{
    return _arg.to_string();
}

namespace detail {

// TODO : BIDLOCODE!
network::ip::address to_address(const char* _saddr, network::error* _error = nullptr) noexcept
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

network::ip::address to_address(const char* _saddr) noexcept
{
    return network::ip::detail::to_address(_saddr);
}

network::ip::address to_address(const char* _saddr, network::error& _error) noexcept
{
    return network::ip::detail::to_address(_saddr, &_error);
}

network::ip::address to_address(const std::string& _saddr) noexcept
{
    return network::ip::to_address(_saddr.c_str());
}

network::ip::address to_address(const std::string& _saddr, network::error& _error) noexcept
{
    return network::ip::to_address(_saddr.c_str(), _error);
}

} // namespace ip
} // namespace network