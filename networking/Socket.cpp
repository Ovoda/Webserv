#include "Socket.hpp"

#include <errno.h>
#include <stdio.h>
namespace network
{

    Socket::Socket(void)
        : _type(SOCK_STREAM), _domain(AF_INET), _protocol(0), _is_good(true) {}

    Socket::Socket(Socket const &src) { *this = src; }

    Socket::~Socket() {}

    Socket::Socket(int const port)
        : _port(port),
          _type(SOCK_STREAM),
          _domain(AF_INET),
          _protocol(0),
          _is_good(true)
    {
        int enable = 1;
        if ((_id = socket(_domain, _type, _protocol)) < 0)
        {
            std::cerr << "Error: Cannot create socket" << std::endl;
            _is_good = false;
            return;
        }
        if (setsockopt(_id, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        _address.sin_family = _domain;
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(_port);

        memset(_address.sin_zero, '\0', sizeof(_address.sin_zero));
        if (_is_good == true)
            do_bind();
        if (_is_good == true)
            do_listen();
        // if (_is_good)
        //     std::cout << "Socket creation and binding successfull. Socket port : "
        //               << _port << " with ID : " << get_id() << std::endl;
        // else {
        //     std::cout << "Socket creation and binding failed. Socket port : "
        //               << _port << std::endl;
        // }
    }

    /***************************************************
                Overloads
***************************************************/

    Socket &Socket::operator=(Socket const &rhs)
    {
        if (this != &rhs)
        {
            _address = *(rhs.get_addr());
            _protocol = rhs.get_protocol();
            _addrlen = rhs.get_addr_len();
            _domain = rhs.get_domain();
            _type = rhs.get_type();
            _port = rhs.get_port();
            _id = rhs.get_id();
            _is_good = rhs.is_good();
        }
        return (*this);
    }

    /***************************************************
                                Member Functions specific to sockets
***************************************************/

    int Socket::do_bind(void)
    {
        _address.sin_family = _domain;
        _address.sin_addr.s_addr = INADDR_ANY;
        _address.sin_port = htons(_port);
        if (bind(_id, (struct sockaddr *)&_address, sizeof(_address)) < 0)
        {
            std::cerr << "Error: Cannot bind socket" << std::endl;
            std::cerr << strerror(errno) << std::endl;
            _is_good = false;
            return (-1);
        }
        return (0);
    }

    int Socket::do_listen(void)
    {
        if (listen(_id, 10) < 0)
        {
            std::cerr << "Error: Listen" << std::endl;
            _is_good = false;
            return (-1);
        }
        fcntl(_id, F_SETFL, O_NONBLOCK);
        return (0);
    }

    int Socket::do_accept(void)
    {
        socklen_t addr_len = get_addr_len();
        struct sockaddr_in client_addr;
        int new_fd;

        if ((new_fd = accept(get_id(), (struct sockaddr *)&client_addr,
                             &addr_len)) < 0)
        {
            // std::cout << strerror(errno) << std::endl;
            // std::cerr << "Error accepting" << std::endl;
            _is_good = false;
            return (new_fd);
        }
        return (new_fd);
    }

    bool Socket::is_good(void) const { return (_is_good); }

    int Socket::unblock(void)
    {
        std::cout << "unblock : is good = " << std::boolalpha << _is_good
                  << std::endl;
        if (_is_good == false || _id < 0)
            return -1;
        return (fcntl(_id, F_SETFL, O_NONBLOCK));
    }

    /***************************************************
        Getters
***************************************************/

    int Socket::get_id(void) const { return _id; }
    int Socket::get_protocol(void) const { return _protocol; }
    int Socket::get_domain(void) const { return _domain; }
    int Socket::get_type(void) const { return _type; }
    int Socket::get_port(void) const { return _port; }
    int Socket::get_addr_len(void) const { return (sizeof(_address)); }
    struct sockaddr_in *Socket::get_addr(void) const
    {
        return ((struct sockaddr_in *)&_address);
    }

} // namespace network

std::ostream &operator<<(std::ostream &o, network::Socket const &s)
{
    o << "Socket " << s.get_id() << " listening on port " << s.get_port()
      << std::endl;
    return (o);
}
