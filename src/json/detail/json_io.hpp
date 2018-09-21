/*!
 * \file json_io.hpp
 * \author ichramm
 *
 * \date January 20, 2014, 11:33 PM
 *
 * json_object Version 2 - Parser, Printer
 */

#ifndef JSON_DETAIL_JSON_IO_HPP
#define JSON_DETAIL_JSON_IO_HPP

#include <string>
#include <sstream>

#include "json/value.hpp"


namespace json {
namespace detail {

class IO {
public:
    /* parses the string `spec`, throws parse_error */
    static void parse(Value& target,
                      const std::string& spec);

    /* parses the content of the input stream `stream`, throws parse_error */
    static void parse(Value& target,
                      std::istream& stream);

    /* dumps the json into the output stream `stream` */
    static std::ostream& print(const Value& obj,
                               std::ostream& stream,
                               bool pretty);
private:
    class parser_context;
    class writer_context;
};

} // namespace detail
} // namespace json

#endif // JSON_DETAIL_JSON_IO_HPP
