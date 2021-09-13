#pragma once
#include "../detail/common.hpp"

namespace network {
namespace ip {

template<AddressFamily _Family>
struct internet_protocol
{
    static constexpr int family() noexcept
    {
        return (int)_Family;
    }

    static constexpr AddressFamily address_family() noexcept
    {
        return _Family;
    }
};

using ipv4 = internet_protocol<AddressFamily::Ipv4>;
using ipv6 = internet_protocol<AddressFamily::Ipv6>;

ipv4 v4() noexcept
{
    return { };
}

ipv6 v6() noexcept
{
    return { };
}

} // namespace network::ip
} // namespace network