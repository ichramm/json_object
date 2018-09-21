/*!
 * \file type.hpp
 * \author ichramm
 *
 * \date 2017-08-17
 */

#ifndef JSON_TYPE_HPP
#define JSON_TYPE_HPP

#include <string>

namespace json {

/**
 * @brief JSON Object Types
 */
class Type {
public:
    /**
     * @brief Known JSON Object types
     */
    enum Value {
        Null     = 1,
        Boolean  = 2,
        Integer  = 4,
        Double   = 8,
        String   = 16,
        Array    = 32,
        Object   = 64,
    };

    static std::string to_string(Value v);
};

} // namespace json

#endif // JSON_TYPE_HPP
