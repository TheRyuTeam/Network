#pragma once
#include "../detail/common.hpp"

#include <array>
#include <string>

namespace network {
namespace ip {

//TODO: link to impl?
/// @class address_v4
/**
 * Just a wrap above native ipv4 address
 * @param Threadsafe - no threadsafe
 * @throw any std::string exceptions in to_string() function
 */
class address_v4
{
public:
    typedef network::detail::in4_addr_t addr_t;
    typedef network::detail::uint32_t uint_t;
    typedef std::array<unsigned char, network::detail::in4_addr_bytes_len> byte_t;

    enum class Class : unsigned char
    {
        This = 0,
        A = 126,
        Loopback = 127,
        B = 193,
        C = 225,
        D = 241,
        E = 255
    };

    enum class Mask : uint_t
    {
        A = 0xff000000,
        B = 0xffff0000,
        C = 0xffffff00
    };

    /// @see Constructors
    address_v4() noexcept = default;
    explicit address_v4(addr_t _addr) noexcept;
    explicit address_v4(uint_t _uint) noexcept;
    explicit address_v4(const byte_t& _bytes) noexcept;
    address_v4(address_v4&& _other) noexcept = default;
    address_v4(const address_v4& _other) noexcept = default;

    /// @see Assing operators
    address_v4& operator=(addr_t _addr) noexcept;
    address_v4& operator=(uint_t _uint) noexcept;
    address_v4& operator=(const byte_t& _bytes) noexcept;
    address_v4& operator=(address_v4&& _other) noexcept = default;
    address_v4& operator=(const address_v4& _other) noexcept = default;

    /// @see Conversions
    uint_t to_uint() const noexcept;
    byte_t to_bytes() const noexcept;
    std::string to_string() const;

    /// @see Properties
    addr_t addr() const noexcept;
    bool is_unicast() const noexcept;
    bool is_multicast() const noexcept;
    bool is_broadcast() const noexcept;
    bool is_loopback() const noexcept;
    Class address_class() const noexcept;
    uint_t network_address() const noexcept;


    /// @see Comparison operators
    friend bool operator==(const address_v4& _a, const address_v4& _b) noexcept;
    friend bool operator!=(const address_v4& _a, const address_v4& _b) noexcept;
    friend bool operator>(const address_v4& _a, const address_v4& _b) noexcept;
    friend bool operator>=(const address_v4& _a, const address_v4& _b) noexcept;
    friend bool operator<(const address_v4& _a, const address_v4& _b) noexcept;
    friend bool operator<=(const address_v4& _a, const address_v4& _b) noexcept;

    /// @see Static
    static address_v4 loopback() noexcept;
private:
    addr_t addr_ { };
};

namespace detail {

network::ip::address_v4 to_address_v4(const char* _saddr, network::error* _error = nullptr) noexcept;

} // namespace detail

address_v4 to_address_v4(const char* _saddr) noexcept;
address_v4 to_address_v4(const char* _saddr, network::error& _error) noexcept;
address_v4 to_address_v4(const std::string& _saddr) noexcept;
address_v4 to_address_v4(const std::string& _saddr, network::error& _error) noexcept;

template<class _CharT, class _Traits>
std::basic_ostream<_CharT, _Traits>& operator<<(std::basic_ostream<_CharT, _Traits>& _os, const address_v4& _address);

#include "impl/address_v4.hpp"

} // namespace ip
} // namespace network