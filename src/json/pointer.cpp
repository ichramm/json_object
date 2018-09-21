/*!
 * \file pointer.cpp
 * \author ichramm
 *
 * \date 2017-08-20
 */

#include "json/pointer.hpp"

#include "json/value.hpp"
#include "json/errors.hpp"

namespace json {

Pointer::Pointer(const std::string& spec)
    : tokens_(make_tokens(spec)) {
}

const Value& Pointer::get_reference(const Value& tgt) const {
    const Value *obj = &tgt;

    std::vector<std::string>::const_iterator it, end;
    for (it = tokens_.begin(), end = tokens_.end(); it != end; ++it) {
        const std::string& token = *it;
        switch (obj->type_) {
            case Type::Object: {
                Value::const_iterator it = obj->value_.map_val.find(token);
                if (it != obj->value_.map_val.end()) {
                    obj = &it->second;
                } else if (!get_by_property_value(obj, token)) { // try <key>=<value> scheme
                    throw key_not_found(token);
                }
                break;
            }
            case Type::Array: {
                if (token == "-" || (token.size() > 1 && (token[0] == '0' || token[0] == '-'))) {
                    throw parse_error(std::string("Reference token '") + token  + "' is invalid in this context");
                }
                if (!get_by_property_value(obj, token)) {
                    try {
                        obj = &obj->value_.arr_val.at(static_cast<size_t>(detail::stoi64(token)));
                    } catch (const std::invalid_argument&) {
                        throw parse_error(std::string("Reference token '") + token  + "' is not a number");
                    }
                }
                break;
            }
            default:
                throw std::out_of_range("Unresolved reference token " + token);
        }
    }

    return *obj;
}

bool Pointer::is_valid_for(const Value &tgt) const {
    try {
        get_reference(tgt);
        return true;
    } catch (const key_not_found&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

void Pointer::unescape(std::string& token) {
    for (size_t pos = token.find_first_of('~');
            pos != std::string::npos;
            pos = token.find_first_of('~', pos + 1)) {
        // ~ must be followed by 0 or 1
        if (pos == token.size() - 1) {
            throw parse_error("Escape character '~' found at the end of string");
        }
        switch (token[pos + 1]) {
            case '0':
                token.replace(pos, 2, "~");
                break;
            case '1':
                token.replace(pos, 2, "/");
                break;
            default:
                throw parse_error("Escape character '~' must be followed with '0' or '1'");
        }
    }
}

std::vector<std::string> Pointer::make_tokens(const std::string& spec) {
    std::vector<std::string> tokens;

    if (spec.empty() || (spec.size() == 1 && spec[0] == '/')) {
        return tokens; // whole JSON
    }

    if (spec[0] != '/') {
        throw parse_error("Non-empty JSON pointer string must start with '/'");
    }

    for (size_t slash = spec.find_first_of('/', 1), start = 1;
            start != 0;
            start = slash + 1, slash = spec.find_first_of('/', start)) {
        tokens.push_back(spec.substr(start, slash-start));
        unescape(tokens.back());
    }

    return tokens;
}

bool Pointer::is_a_match(const Value& obj, const std::string key, const std::string value) {
    if (obj.type_ == Type::Object) {
        Value::const_iterator kit = obj.value_.map_val.find(key);
        if (kit != obj.end()) {
            switch (kit->second.type_) {
                case Type::Null:
                    return (value == "null");
                case Type::Boolean:
                    return kit->second.value_.bool_val ? (value == "true") : (value == "false");
                case Type::Integer:
                    return value == detail::to_string(kit->second.value_.int_val);
                case Type::Double:
                    return value == detail::to_string(kit->second.value_.dbl_val);
                case Type::String:
                    return value == kit->second.value_.str_val;
                default:
                    break;
            }
        }
    }
    return false;
}

bool Pointer::get_by_property_value(const Value * & obj, const std::string& token) {
    size_t pos = token.find_first_of("=");
    if (pos != std::string::npos) {
        std::string key = token.substr(0, pos);
        std::string value = token.substr(pos+1);
        if (obj->type_ == Type::Object) {
            for (Value::const_iterator it = obj->value_.map_val.begin(), end = obj->value_.map_val.end();
                    it != end;
                    ++it) {
                if (is_a_match(it->second, key, value)) {
                    obj = &it->second;
                    return true;
                }
            }
        } else {
            for (Value::const_array_iterator it = obj->value_.arr_val.begin(), end = obj->value_.arr_val.end();
                    it != end;
                    ++it) {
                if (is_a_match(*it, key, value)) {
                    obj = &(*it);
                    return true;
                }
            }
        }
    }
    return false;
}

} // namespace json
