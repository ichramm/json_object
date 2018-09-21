/*!
 * \file errors.cpp
 * \author ichramm
 *
 * \date 2017-08-17
 */

#include "json/errors.hpp"

#include "json/detail/compiler.hpp"

namespace json {

/* error_base */

error_base::~error_base() noexcept {}

/* bad_json_type */

bad_json_type::bad_json_type(Type::Value expected, Type::Value got)
    : error_base(std::string("Invalid type, expected ") + Type::to_string(expected) + ", got " + Type::to_string(got)) {
}

bad_json_type::~bad_json_type() noexcept {
}

/* key_not_found */

key_not_found::key_not_found(const std::string& key)
    : error_base(std::string("Key '") + key + "' does not exist") {
}

key_not_found::~key_not_found() noexcept {
}

/* parse_error */

parse_error::parse_error(const std::string& msg)
    : error_base(std::string("Parse error: ") + msg),
      error_(""),
      line_(0),
      column_(0) {
}

parse_error::parse_error(const char *error, unsigned int line, unsigned int column)
    : error_base(std::string("Parse error at (line ") + detail::to_string(line) + ", column " + detail::to_string(column) + "): " + error),
      error_(error),
      line_(line),
      column_(column) {
}

parse_error::~parse_error() noexcept {
}

const char * parse_error::error() const {
    return error_;
}

unsigned int parse_error::line() const {
    return line_;
}

unsigned int parse_error::column() const {
    return column_;
}

} // namespace json
