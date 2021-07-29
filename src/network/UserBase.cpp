#include "UserBase.hpp"

namespace network {
UserBase::UserBase(void) {}
UserBase::~UserBase() {}

void UserBase::add(SimpleRequest const &request) {
	/* if ip is known */
    std::vector<User>::iterator it = _base.begin();
    if (is_known_ip(request.get_ipv4()) == true) {
        for (; it != _base.end(); it++) {
            if (it->get_ipv4() == request.get_ipv4()) {
				it->add(request);
				return ;
			}
        }
    }
	/* if ip isn't known */
    network::User new_user(request);
	_base.push_back(new_user);
}

bool UserBase::is_known_ip(char const *ipv4) {
    std::vector<User>::iterator it = _base.begin();

    for (; it != _base.end(); it++) {
        if (it->get_ipv4() == ipv4) return true;
    }
    return false;
}

}  // namespace network
