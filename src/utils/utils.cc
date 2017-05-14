//
// Created by lol on 3/21/2017.
//
#include <sstream>
#include "utils.h"

namespace utils {
    const std::string NotImplementedError::STATIC_MSG = "Method is not implemented";
    NotImplementedError::NotImplementedError(const std::string &method_name, const std::string &msg) : runtime_error(
            BuildMsg(method_name, msg)) {

    }

    NotImplementedError::NotImplementedError(const char *method_name, const char *msg) : runtime_error(
            BuildMsg(method_name, msg)) {

    }

    NotImplementedError::NotImplementedError() : runtime_error(STATIC_MSG) {

    }

    std::string NotImplementedError::BuildMsg(const std::string &method_name, const std::string &msg) {
        std::stringstream ss; ss << msg << "\r\nFrom method: " << '\'' << method_name << '\'';
        return ss.str();
    }


}
