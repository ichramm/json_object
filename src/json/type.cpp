/*!
 * \file type.cpp
 * \author ichramm
 *
 * \date 2017-08-17
 */

#include "json/type.hpp"

namespace json {

std::string Type::to_string(Type::Value type) {
    std::string res;
    if(type & Null) {
        res = "Null";
    }
    if(type & Boolean) {
        res = res.empty() ? "Boolean" : res + "|Boolean";
    }
    if(type & Integer) {
        res = res.empty() ? "Integer" : res + "|Integer";
    }
    if(type & Double) {
        res = res.empty() ? "Double" : res + "|Double";
    }
    if(type & String) {
        res = res.empty() ? "String" : res + "|String";
    }
    if(type & Array) {
        res = res.empty() ? "Array" : res + "|Array";
    }
    if(type & Object) {
        res = res.empty() ? "Object" : res + "|Object";
    }
    return res.empty() ? "Unknown" : res;
}

} // namespace json
