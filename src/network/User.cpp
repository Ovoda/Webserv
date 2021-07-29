#include "User.hpp"

namespace network {
User::User(void) {}
User::User(SimpleRequest const &request) : _ipv4(request.get_ipv4()) {
    _requests.push_back(request);
}

void User::add(SimpleRequest const &request) { _requests.push_back(request); }

char const *User::get_ipv4(void) const { return _ipv4.c_str(); }

void User::requests_log(ostream &o) {
    std::vector<network::SimpleRequest>::iterator it = _requests.begin();
    for (; it != _requests.end(); it++) {
        o << it << std::endl;
    }
}

}  // namespace network