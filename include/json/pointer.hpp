/*!
 * \file pointer.hpp
 * \author ichramm
 *
 * \date 2017-08-20
 */

#ifndef JSON_POINTER_HPP
#define JSON_POINTER_HPP

#include <string>
#include <vector>

namespace json {

class Value;

/**
 * @brief JSON pointer defines a string syntax for identifying a specific
 * value within a JSON document. Implements RFC-6901 (https://tools.ietf.org/html/rfc6901)
 */
class Pointer {
public:

    /**
     * @brief Construct a new JSON pointer according to the syntax defined in the RFC.
     *
     * @param spec String representation of the JSON pointer
     *
     * @remarks The ABNF syntax of a JSON pointer is:
     * @code
     *     json-pointer    = *( "/" reference-token )
     *     reference-token = *( unescaped / escaped )
     *     unescaped       = %x00-2E / %x30-7D / %x7F-10FFFF
     *        ; %x2F ('/') and %x7E ('~') are excluded from 'unescaped'
     *     escaped         = "~" ( "0" / "1" )
     *       ; representing '~' and '/', respectively
     * @endcode
     *
     * @exception parse_error If the spec is invalid
     *
     * @remarks This implementation supports tokens of the format <key>=<value>.
     */
    explicit Pointer(const std::string& spec);

    /**
     * @brief Looks for a nested JSON using the reference tokens
     * specified in this JSON pointer.
     *
     * @param tgt The JSON on which to look
     *
     * @return A const reference to the JSON identified with the
     * reference tokens defined by this JSON pointer.
     */
    const Value& get_reference(const Value& tgt) const;

    /**
     * @brief Checks whether this JSON pointer is valid within the JSON
     * value specified as a parameter.
     *
     * @param tgt The JSON to test this pointer in.
     *
     * @return Whether this pointer is valid or not.
     */
    bool is_valid_for(const Value& tgt) const;

private:
    std::vector<std::string> tokens_;

    /* escape characters as defined by the RFC */
    void unescape(std::string& token);
    /* splits the spec into reference tokens */
    std::vector<std::string> make_tokens(const std::string& spec);
    /* checks if the JSON obj contains key=value */
    static bool is_a_match(const Value& obj, const std::string key, const std::string value);
    /* looks for a child object matching the token */
    static bool get_by_property_value(const Value * & obj, const std::string& token);
};

} // namespace json

#endif // JSON_POINTER_HPP
