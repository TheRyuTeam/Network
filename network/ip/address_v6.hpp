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

    /// @see Constructors
    address_v6() noexcept = default;
    explicit address_v6(addr_t _addr, scope_id_t _scope_id = 0) noexcept;
    explicit address_v6(const byte_t& _bytes, scope_id_t _scope_id = 0) noexcept;
    address_v6(address_v6&& _other) noexcept = default;
    address_v6(const address_v6& _other) noexcept = default;

    /// @see Assign operators
    address_v6& operator=(const address_v6& _other) noexcept = default;
    address_v6& operator=(address_v6&& _other) noexcept = default;

    /// @see Conversions
    byte_t to_bytes() const noexcept;
    std::string to_string() const;

    /// @see Properties
    addr_t addr() const noexcept;
    scope_id_t scope_id() const noexcept;
    bool is_loopback() const noexcept;

    /// @see Comparison operators
    friend bool operator==(const address_v6& _a, const address_v6& _b) noexcept;
    friend bool operator!=(const address_v6& _a, const address_v6& _b) noexcept;
    friend bool operator>(const address_v6& _a, const address_v6& _b) noexcept;
    friend bool operator>=(const address_v6& _a, const address_v6& _b) noexcept;
    friend bool operator<(const address_v6& _a, const address_v6& _b) noexcept;
    friend bool operator<=(const address_v6& _a, const address_v6& _b) noexcept;

    /// @see Static
    static address_v6 loopback() noexcept;
private:
    addr_t addr_ { };
    scope_id_t scope_id_ { };
};

namespace detail {

network::ip::address_v6 to_address_v6(const char* _saddr, network::error* _error = nullptr) noexcept;

} // namespace detail

network::ip::address_v6 to_address_v6(const char* _saddr) noexcept;
network::ip::address_v6 to_address_v6(const char* _saddr, network::error& _error) noexcept;
network::ip::address_v6 to_address_v6(const std::string& _saddr) noexcept;
network::ip::address_v6 to_address_v6(const std::string& _saddr, network::error& _error) noexcept;

template<class _CharT, class _Traits>
std::basic_ostream<_CharT, _Traits>& operator<<(std::basic_ostream<_CharT, _Traits>& _os, const address_v4& _address);

#include "impl/address_v6.hpp"

} // namespace ip
} // namespace network