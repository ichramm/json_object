/*!
 * \file compiler.hpp
 * \author ichramm
 *
 * \date January 20, 2014, 11:41 PM
 *
 * json_object Version 2 - Compiler specific stuff
 */

#ifndef JSON_DETAIL_COMPILER_HPP
#define JSON_DETAIL_COMPILER_HPP

#include <stdint.h>
#include <string.h>

#include <string>
#include <stdexcept>

#if __cplusplus >= 201103L
#define JSON_HAVE_CXX11 1

#include <utility>
#include <initializer_list>

namespace json {
namespace detail {

using std::to_string;
using std::stod;

// wrapper a around std::stoll (which returns long long, which is at least 64-bit size)
inline int64_t stoi64(const std::string& str, size_t* idx = 0, int base = 10) {
   return std::stoll(str, idx, base);
}

} // namespace detail
} // namespace json

#else // __cplusplus >= 201103L || _MSC_VER >= 1700
#define JSON_HAVE_CXX11 0

#include <cmath>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <climits>

#define nullptr NULL
#define noexcept throw()

namespace json {
namespace detail {

// helper
template<typename T> std::string to_string_T(T val, const char *fmt) {
    static __thread char buff[128];
    int len = snprintf(buff, sizeof(buff), fmt, val);
    return std::string(buff, static_cast<size_t>(len));
}

// conversion functions available since C++11

inline std::string to_string(int val) {
    return to_string_T(val, "%d");
}

inline std::string to_string(unsigned int val) {
    return to_string_T(val, "%u");
}

inline std::string to_string(int64_t val) {
    static const char *fmt = sizeof(val) == sizeof(long) ? "%ld" : "%lld";
    return to_string_T(val, fmt);
}

inline std::string to_string(uint64_t val) {
    static const char *fmt = sizeof(val) == sizeof(unsigned long) ? "%lu" : "%llu";
    return to_string_T(val, fmt);
}

inline std::string to_string(double val) {
    return to_string_T(val, "%g");
}

inline int64_t stoi64(const std::string& str, size_t* idx = 0, int base = 10) {
    const char *ptr = str.c_str();
    char *eptr;
    errno = 0;
    int64_t res = strtoll(ptr, &eptr, base);
    if (ptr == eptr) {
        throw std::invalid_argument("invalid stoi64 argument");
    }
    if (errno == ERANGE) {
        throw std::out_of_range("stoi64 argument out of range");
    }
    if (idx != NULL) {
        *idx = (size_t)(eptr - ptr);
    }
    return res;
}

// see http://www.cplusplus.com/reference/string/stod/
inline double stod(const std::string& str, size_t* idx = 0) {
    const char *ptr = str.c_str();
    char *eptr;
    errno = 0;
    double res = strtod(ptr, &eptr);
    if (ptr == eptr) {
        throw std::invalid_argument("invalid stod argument");
    }
    if (errno == ERANGE) {
        throw std::out_of_range("stod argument out of range");
    }
    if (idx != NULL) {
        *idx = (size_t)(eptr - ptr);
    }
    return res;
}

} // namespace detail
} // namespace json

#endif // __cplusplus >= 201103L

#endif // JSON_DETAIL_COMPILER_HPP
