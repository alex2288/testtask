#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

#define LOG(msg) \
std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl

#endif //LOGGER_HPP
