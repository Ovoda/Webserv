#ifndef NETWORK_USER_BASE_HPP
#define NETWORK_USER_BASE_HPP

#include <vector>

#include "User.hpp"

namespace network {
class UserBase {
   public:
    UserBase(void);
    ~UserBase();

    void add(SimpleRequest const &request);
    bool is_known_ip(char const *ipv4);

   private:
    std::vector<network::User> _base;
    unsigned int nb_users;
};
}  // namespace network

#endif