//
// Created by lol on 3/21/2017.
//
#include "utils.h"

namespace utils {
    const std::string NotImplementedError::STATIC_MSG = "Method is not implemented";

    NotImplementedError::NotImplementedError(const std::string &msg) : runtime_error(msg) {

    }

    NotImplementedError::NotImplementedError(const char *msg) : runtime_error(msg) {

    }

    NotImplementedError::NotImplementedError() : runtime_error(STATIC_MSG) {

    }


}
