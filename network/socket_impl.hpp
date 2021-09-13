#pragma once
#include "base_socket.hpp"
#include <vector>

namespace network {

#define SOCKET_IMPL_METHOD_WITH_ERROR_SET_MACRO(Name)                                           \
template<class... _Args>                                                                        \
bool Name(_Args&&... _args, error& _error, int _flags = 0) const                                \
{                                                                                               \
    return network::detail::set_error(Name(std::forward<_Args>(_args)..., _flags), _error);     \
}

template<class _InternetProtocol>
class socket_impl
{
private:
    template<class _CharT, class _FuncT>
    bool io_n(_CharT _data, int _length, const _FuncT& _func, int _flags = 0) const noexcept
    {
        long long size { };
        std::size_t global = 0;
        while( global + chunk_size < _length ) {
            size = ::send(socket(), _data + global, chunk_size, _flags);
            if( size == -1 )
                return false;
            global += size;
        }
        size = ::send(socket(), _data + global, _length - global, _flags);
        return size != -1;
    }

public:
    typedef base_socket impl_type;
    static const long long chunk_size = 4096;

    socket_impl() noexcept = default;
    socket_impl(_InternetProtocol _ip, SocketType _type) noexcept
        : s_(_ip.family(), (int)_type)
    {
    }
    socket_impl(SocketType _type) noexcept
        : s_(_InternetProtocol::family(), (int)_type)
    {
        static_assert(_InternetProtocol::family(), "");
    }
    socket_impl(socket_impl&& _other) noexcept = default;
    socket_impl(const socket_impl& _other) noexcept = default;

    socket_impl& operator=(socket_impl&& _other) noexcept = default;
    socket_impl& operator=(const socket_impl& _other) noexcept = default;

    bool is_open() const noexcept
    {
        return is_open_;
    }

    bool close() noexcept
    {
        return is_open_ &= s_.close();
    }

    bool open(const ip::endpoint& _ep, int _n = INT_MAX) noexcept
    {
        return is_open_ |=  s_.bind(_ep) && s_.listen(_n) ;
    }

    bool open(const ip::endpoint& _ep, network::error& _error, int _n = INT_MAX) noexcept
    {
        return is_open_ |= s_.bind(_ep, _error) && s_.listen(_error, _n);
    }

    template<class... _Args>
    bool accept(socket_impl& _s, _Args&&... _args) const noexcept
    {
        return s_.accept(_s.s_, std::forward<_Args>(_args)...);
    }

    template<class... _Args>
    bool connect(_Args&&... _args) const noexcept
    {
        return s_.connect(std::forward<_Args>(_args)...);
    }

    network::detail::socket_t socket() const noexcept
    {
        return s_.socket();
    }


    bool write_n(const char* _data, int _length, int _flags = 0) const noexcept
    {
        return io_n(_data, _length, ::send, _flags);
    }

    bool write_n(const std::vector<char>& _data, int _flags = 0) const noexcept
    {
        return write_n(_data.data(), _data.size(), _flags);
    }

    bool write(const std::string& _data, int _flags = 0) const noexcept
    {
        return write_n(_data.data(), _data.size(), _flags);
    }

    bool read_n(char* _buff, int _length, int _flags = 0) const noexcept
    {
        return io_n(_buff, _length, ::recv, _flags);
    }


    bool read_n(std::vector<char>& _buff, int _flags = 0) const noexcept
    {
        return io_n(_buff.data(), _buff.size(), _flags);
    }

    template<class _Container>
    bool read(_Container& _data, int _flags = 0) const
    {
        _data.clear();
        long long size;
        do {
            typename _Container::value_type bytes[chunk_size + 1]{ };
            size = ::recv(socket(), bytes, chunk_size, _flags);
            if( size == -1 )
                return false;
            _data.assign(bytes, bytes + size);
        } while( size == chunk_size );
        return true;
    }

    template<class _Container>
    bool read_until(_Container& _data, char _val, int _flags = 0) const
    {
        _data.clear();
        long long size;
        typename _Container::value_type* end;
        typename _Container::value_type* it;
        do {
            typename _Container::value_type bytes[chunk_size + 1]{ };
            size = ::recv(socket(), bytes, chunk_size, _flags);
            if( size == -1 )
                return false;
            end = bytes + size;
            it = std::find(bytes, end + 1, _val);
            _data.assign(bytes, it);
        } while( size == chunk_size && it != end + 1 );
        return true;
    }
    SOCKET_IMPL_METHOD_WITH_ERROR_SET_MACRO(write_n)
    SOCKET_IMPL_METHOD_WITH_ERROR_SET_MACRO(write)
    SOCKET_IMPL_METHOD_WITH_ERROR_SET_MACRO(read_n)
    SOCKET_IMPL_METHOD_WITH_ERROR_SET_MACRO(read)
private:
    base_socket s_;
    bool is_open_ { false };
};


} // namespace network