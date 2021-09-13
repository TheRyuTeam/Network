#pragma once
#include "address_v4.hpp"
#include "address_v6.hpp"

namespace network {
namespace ip {

class address
{
public:
    /// @see Constructors
    address() noexcept = default;
    address(address_v4&& _v4) noexcept;
    address(address_v6&& _v6) noexcept;
    address(const address_v4& _v4) noexcept;
    address(const address_v6& _v6) noexcept;
    address(address&& _other) noexcept = default;
    address(const address& _other) noexcept = default;

    /// @see Assign operators
    address& operator=(address_v4&& _v4) noexcept;
    address& operator=(address_v6&& _v6) noexcept;
    address& operator=(const address_v4& _v4) noexcept;
    address& operator=(const address_v6& _v6) noexcept;
    address& operator=(address&& _other) noexcept = default;
    address& operator=(const address& _other) noexcept = default;

    /// @see Properties
    bool is_v4() const noexcept;
    bool is_loopback() const noexcept;
    address_v4 v4() const noexcept;
    address_v6 v6() const noexcept;

    /// @see Conversions
    std::string to_string() const;

private:
    address_v4 v4_ { };
    address_v6 v6_ { };
    bool is_v4_ { true };
};

namespace detail {

// TODO : BIDLOCODE!
network::ip::address to_address(const char* _saddr, network::error* _error = nullptr) noexcept;

} // namespace detail

network::ip::address to_address(const char* _saddr) noexcept;
network::ip::address to_address(const char* _saddr, network::error& _error) noexcept;
network::ip::address to_address(const std::string& _saddr) noexcept;
network::ip::address to_address(const std::string& _saddr, network::error& _error) noexcept;


template<class _CharT, class _Traits>
std::basic_ostream<_CharT, _Traits>& operator<<(std::basic_ostream<_CharT, _Traits>& _os, const address_v4& _address);

#include "impl/address.hpp"

} // namespace ip
} // namespace network