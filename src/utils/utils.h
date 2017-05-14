//
// Created by lol on 3/21/2017.
//
#pragma once
#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include <stdexcept>
#include <functional>
#include <cstddef>
#include <memory>

namespace utils {
    template <class ContainerType>
    class default_preallocator {
    public:
        void operator () (ContainerType& cnt, size_t size) const {
            cnt.reserve(size);
        }
    };

    template <typename T, template<class> class ContainerType>
    ContainerType<T*> ContainerToRefContainer(const ContainerType<T>& container) {
        auto arr = std::make_unique<T*>(container.size());
        size_t i = 0;
        for ( auto &el : container )
            (*arr)[i++] = &el; // TODO: IS IT GOOD?
        return { *arr, *arr + container.size() };
    };


    class NotImplementedError : public std::runtime_error {
    public:
        static const std::string STATIC_MSG;
        NotImplementedError(const std::string &msg, const std::string &method_name = "");
        NotImplementedError(const char *msg, const char *method_name = nullptr);
        NotImplementedError();
        static std::string BuildMsg(const std::string &method_name, const std::string &msg);
    };


}


#endif // SRC_UTILS_H