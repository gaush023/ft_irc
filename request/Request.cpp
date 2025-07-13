#include "../include/Request.hpp"

Request::Request() : args(), command(), invalidMessage(false) {}
Request::Request( const Request &other) { *this = other; }
Request::~Request() {}

Request &Request::operator=(const Request &other) {
    if (this == &other)
        return *this;
    this->args = other.args;
    this->command = other.command;
    this->invalidMessage = other.invalidMessage;
    return *this;
}




