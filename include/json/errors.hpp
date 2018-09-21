/*!
 * \file errors.hpp
 * \author ichramm
 *
 * \date 2017-08-17
 */

#ifndef JSON_ERRORS_HPP
#define JSON_ERRORS_HPP

#include <string>
#include <stdexcept>

#include "type.hpp"
#include "detail/compiler.hpp"

namespace json {

/**
 * @brief Base error class for type unification
 */
class error_base: public std::runtime_error {
public:
    error_base(const std::string &s): std::runtime_error(s){}
    error_base(const char *s): std::runtime_error(s){}
    virtual ~error_base() noexcept;
};

/**
 * @brief Thrown by @c to_<type> functions when the type is not correct
 */
class bad_json_type: public error_base {
public:
    bad_json_type(Type::Value expected, Type::Value got);
    virtual ~bad_json_type() noexcept;
};

/**
 * Thrown by the indexing operator if the key is not found.
 *
 * @remarks The non-const indexing operator inserts a non-valid JSON object. Any
 * attempt to read that object will cause this error to be thrown.
 */
class key_not_found: public error_base {
public:
    explicit key_not_found(const std::string &key);
    virtual ~key_not_found() noexcept;
};

/**
 * @brief Thrown by the parser if the json is invalid
 */
class parse_error: public error_base {
public:
    parse_error(const std::string& msg);

    parse_error(const char *error,
                unsigned int line,
                unsigned int column);

    virtual ~parse_error() noexcept;

    const char *error() const;

    unsigned int line() const;

    unsigned int column() const;

private:
    const char*  error_;
    unsigned int line_;
    unsigned int column_;
};

} // namespace json

#endif // JSON_ERRORS_HPP
