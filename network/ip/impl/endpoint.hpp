#pragma once

///                              Constructors

endpoint::endpoint() noexcept
{
    data_.base = AF_INET;
    data_.v4.sin_addr.s_addr = INADDR_ANY;
    data_.v4.sin_port = 0;
}

endpoint::endpoint(const address& _addr, unsigned short _port) noexcept
{
    if( _addr.is_v4() ) {
        data_.base = AF_INET;
        data_.v4.sin_addr = _addr.v4().addr();
        data_.v4.sin_port = htons(_port);
    }
    else {
        data_.base = AF_INET6;
        data_.v6.sin6_addr = _addr.v6().addr();
        data_.v6.sin6_port = htons(_port);
    }
}

///                              Constructors


///                             Properties

bool endpoint::is_v4() const noexcept
{
    return data_.base == AF_INET;
}


unsigned short endpoint::port() const noexcept
{
    return is_v4() ? htons(data_.v4.sin_port) : htons(data_.v6.sin6_port);
}

void endpoint::port(unsigned short _port) noexcept
{
    ( is_v4() ? data_.v4.sin_port : data_.v6.sin6_port) = htons(_port);
}

std::size_t endpoint::size() const noexcept
{
    return is_v4() ? sizeof(network::detail::sockaddr_in4_t) : sizeof(network::detail::sockaddr_in6_t);
}

std::size_t endpoint::capacity() const noexcept
{
    return sizeof(data_);
}

network::detail::sockaddr_t* endpoint::sockaddr_ptr() noexcept
{
    return (network::detail::sockaddr_t*)&data_;
}

const network::detail::sockaddr_t* endpoint::sockaddr_ptr() const noexcept
{
    return (const network::detail::sockaddr_t*)&data_;
}

endpoint::operator network::detail::sockaddr_t*() noexcept
{
    return sockaddr_ptr();
}

endpoint::operator const network::detail::sockaddr_t*() const noexcept
{
    return sockaddr_ptr();
}

///                             Properties


///                             Conversions

/// Endpoint string representation.
std::string endpoint::to_string() const
{
    std::string str_ep;
    if( is_v4() ) {
        str_ep = inet_ntoa(data_.v4.sin_addr);
        str_ep += ':';
        str_ep += std::to_string(htons(data_.v4.sin_port));
    }
    else {
        char buff[network::detail::in6_addr_str_len]{ };
        str_ep = '[';
        str_ep += inet_ntop(AF_INET6, &data_.v6.sin6_addr, buff, network::detail::in6_addr_str_len);
        str_ep += ']';
        str_ep += ':';
        str_ep += std::to_string(htons(data_.v6.sin6_port));
    }
    return str_ep;
}

address endpoint::to_address() const noexcept
{
    if( is_v4() )
        return address_v4(data_.v4.sin_addr);
    return address_v6(data_.v6.sin6_addr);
}

///                             Conversions