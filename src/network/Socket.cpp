#include "Socket.hpp"
namespace network {

Socket::Socket(void) : _type(SOCK_STREAM), _domain(AF_INET), _protocol(0) {}

Socket::Socket(Socket const &src) { *this = src; }

Socket::~Socket() {}

Socket::Socket(int const port)
    : _port(port), _type(SOCK_STREAM), _domain(AF_INET), _protocol(0) {
    if ((_id = socket(_domain, _type, _protocol)) == 0) {
        std::cerr << "Error: Cannot create socket" << std::endl;
        return;
    }
    _address.sin_family = _domain;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);

    memset(_address.sin_zero, '\0', sizeof(_address.sin_zero));
    if (do_bind() == 0 && do_listen() == 0)
        std::cout << "Socket creation and binding successfull. Socket port : "
                  << _port << std::endl;
}

/***************************************************
        Overloads
***************************************************/

Socket &Socket::operator=(Socket const &rhs) {
    if (this != &rhs) {
        _address = *(rhs.get_addr());
        _protocol = rhs.get_protocol();
        _addrlen = rhs.get_addr_len();
        _domain = rhs.get_domain();
        _type = rhs.get_type();
        _port = rhs.get_port();
        _id = rhs.get_id();
    }
    return (*this);
}

/***************************************************
                Member Functions specific to sockets
***************************************************/

int Socket::do_bind(void) {
    _address.sin_family = _domain;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
    if (bind(_id, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
        std::cerr << "Error: Cannot bind socket" << std::endl;
        return (-1);
    }
    return (0);
}

int Socket::do_listen(void) const {
    if (listen(_id, 10) < 0) {
        std::cerr << "Error: Listen" << std::endl;
        return (-1);
    }
    return (0);
}

network::SimpleRequest	Socket::do_accept(void) const {
    socklen_t addr_len = get_addr_len();
    network::SimpleRequest request;
    struct sockaddr_in client_addr;
    char buffer[INET_ADDRSTRLEN];

    request.set_fd(
        accept(get_id(), (struct sockaddr *)&client_addr, &addr_len));
    if (request.get_fd() < 0) {
        std::cerr << "Error accepting" << std::endl;
        return (request);
    }
	request.set_time();
    request.set_ipv4(inet_ntop(AF_INET, &client_addr.sin_addr, buffer, INET_ADDRSTRLEN));

	// DEBUG
	std::cout << "Request comming from : " << request.get_ipv4() << std::endl;
    return (request);
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
struct sockaddr_in *Socket::get_addr(void) const {
    return ((struct sockaddr_in *)&_address);
}

}  // namespace network

std::ostream &operator<<(std::ostream &o, network::Socket const &s) {
    o << "Socket " << s.get_id() << " listening on port " << s.get_port()
      << std::endl;
    return (o);
}
