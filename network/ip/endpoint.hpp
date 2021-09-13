#pragma once
#include "address.hpp"

namespace network {
namespace ip {

class endpoint
{
public:
    /// @see Constructors
    endpoint() noexcept;
    endpoint(const address& _addr, unsigned short _port = 0) noexcept;
    endpoint(endpoint&& _other) noexcept = default;
    endpoint(const endpoint& _other) noexcept = default;

    /// Assign operators
    endpoint& operator=(endpoint&& _other) noexcept = default;
    endpoint& operator=(const endpoint& _other) noexcept = default;

    /// @see Properties
    bool is_v4() const noexcept;
    unsigned short port() const noexcept;
    void port(unsigned short _port) noexcept;
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    network::detail::sockaddr_t* sockaddr_ptr() noexcept;
    const network::detail::sockaddr_t* sockaddr_ptr() const noexcept;
    operator network::detail::sockaddr_t*() noexcept;
    operator const network::detail::sockaddr_t*() const noexcept;

    /// @see Conversions
    std::string to_string() const;
    address to_address() const noexcept;

private:
    union {
        network::detail::family_t base;
        network::detail::sockaddr_in4_t v4;
        network::detail::sockaddr_in6_t v6;
    } data_ { };
    bool is_v4_ { true };
};


#include "impl/endpoint.hpp"

} // namespace ip
} // namespace network

