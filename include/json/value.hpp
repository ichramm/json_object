/*!
 * \file value.hpp
 * \author ichramm
 *
 * \date January 16, 2014, 12:59 PM
 * 
 * json_object Version 2
 */

#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

#include "detail/compiler.hpp"
#include "pointer.hpp"
#include "errors.hpp"
#include "type.hpp"

#include <map>
#include <vector>

namespace json {

namespace detail {
    class IO; // parser/writer
}

/**
 * @brief Used to call the constructor that parses the parameter
 * string instead of the one which just uses it.
 */
extern const struct json_spec_parse{} option_parse;

/**
  * @brief Makes @c to_string work on non string objects
  */
extern const struct json_spec_force{} option_force;

/**
 * @brief A JSON value
 */
class Value {
public:
    typedef std::string                key_type;

    typedef std::map<key_type, Value>  map_type;
    typedef map_type::value_type       value_type;
    typedef map_type::iterator         iterator;
    typedef map_type::const_iterator   const_iterator;

    typedef std::vector<Value>         array_type;
    typedef array_type::value_type     array_value_type; // this type
    typedef array_type::iterator       array_iterator;
    typedef array_type::const_iterator const_array_iterator;

    /**
     * @brief Null JSON Value
     */
    static const Value Null;

    /**
     * @brief Constructs an empty JSON Value.
     */
    Value(void);

    /**
     * @brief Constructs a JSON Value having type @c Boolean and value @p value.
     */
    Value(bool value);

    /**
     * @brief Constructs a JSON Value having type @c Integer and value @p value.
     */
    Value(int16_t value);

    /**
     * @brief Constructs a JSON Value having type @c Integer and value @p value.
     */
    Value(uint16_t value);

    /**
     * @brief Constructs a JSON Value having type @c Integer and value @p value.
     */
    Value(int32_t value);

    /**
     * @brief Constructs a JSON Value having type @c Integer and value @p value.
     */
    Value(uint32_t value);

    /**
     * @brief Constructs a JSON Value having type @c Integer and value @p value.
     */
    Value(int64_t value);

    /**
     * @brief Constructs a JSON Value having type @c Double and value @p value.
     */
    Value(double value);

    /**
     * @brief Constructs a JSON Value having type @c String and value @p value.
     *
     * @note This overload is needed because the compiler chooses the built-in
     * convertion from @c char* to @c bool instead of the (user-defined) conversion
     * from @c char* to @c std::string
     */
    Value(const char *value);

    /**
     * @brief Constructs a JSON Value having type @c String and value @p value.
     */
    Value(const std::string& value);

    /**
     * @brief Constructs a JSON Value having type @c Array and value @p value.
     */
    Value(const array_type& value);

    /**
     * @brief Constructs a JSON Value having type @c Object and value @p value.
     */
    Value(const map_type& value);

    /**
     * @brief Constructs a JSON Value by parsing the string @p spec.
     *
     * @exception parse_error
     * @exception std::bad_alloc
     */
    Value(const json_spec_parse&, const std::string& spec);

    /**
     * Constructs a JSON by parsing the content of the stream @p stream.
     *
     * @exception parse_error
     * @exception std::bad_alloc
     */
    explicit Value(std::istream& stream);

#if JSON_HAVE_CXX11
    /**
     * Constructs a JSON object with type @c Object and having the elements
     * provided in the initializer list @p childs as childs
     *
     * @param childs An initializer list of key-value pairs
     *
     * This constructos gets called when the @c JSON Value is instantiated as in the
     * following code:
     * @code
     *   Value obj{
     *       { "hello", "world" } // 'hello' is the key, 'world' is the value, type is string
     *       { "age"  , 27      } // 'age' is the key, 27 is the value, type is int
     *       { "compo", {         // 'compo' is the key, and the value is an object
     *           { "site", "one" } // nested object
     *       }
     *   };
     * @endcode
     *
     * @note If you want to construct an array use something like the
     * following (note the double brackets):
     * @code
     *    Value obj{{ "one", "two", 3, 4.0, false }};
     * @endcode
     * The previous code ends-up calling @c Value::Value(const array_type&) because
     * it takes advantage of the constructor of @c std::vector
     */
    Value(std::initializer_list<std::pair<const std::string, Value>> childs);
#endif

    /**
     * @return The type of this JSON. @see Type
     */
    Type::Value type() const;

    /**
     * @return @c true if this JSON is of type is @c Null
     */
    bool is_null() const;

    /**
     * @return @c true if this JSON is of type is @c Boolean
     */
    bool is_boolean() const;

    /**
     * @return @c true if this JSON is of type is @c Integer
     */
    bool is_integer() const;

    /**
     * @return @c true if this JSON is of type is @c Double
     */
    bool is_double() const;

    /**
     * @return @c true if this JSON is of type is @c String
     */
    bool is_string() const;

    /**
     * @return @c true if this JSON is of type is @c Array
     */
    bool is_array() const;

    /**
     * @return @c true if this JSON is of type is @c Object
     */
    bool is_object() const;

    /**
     * @return @c true if one of the following conditions is met:
     *
     * @li This objects has type @c Object and its underlying container is empty.
     * @li This objects has type @c Array and its underlying array is empty.
     */
    bool empty() const;

    /**
     * @return The size of the underlying container.
     *
     * @exception bad_json_type When called on objects having type other than @c Object and @c Array.
     */
    size_t size() const;

    /**
     * @return @c true if this JSON has type Object and contains the key @c key
     *
     * @exception bad_json_type When called on instances having type other than @c Object.
     */
    bool exists(const key_type& key) const;

    /**
     * @return @c true if this JSON has type Object and contains something in the path
     * specified by the JSON pointer specified as parameter.
     *
     * @exception bad_json_type When called on instances having type other than @c Object.
     */
    bool exists(const Pointer& pointer) const;

    /**
     * @return the underlying boolean value.
     *
     * @exception bad_json_type When called on objects having type other than @c Boolean.
     */
    bool to_bool() const;

    /**
     * @return the underlying (64-bit) integer value.
     *
     * @exception bad_json_type When called on objects having type other than @c Integer.
     */
    int64_t to_int() const;

    /**
     * @brief Cast facility for integer types
     *
     * @return The underlying integer casted to the corresponding types
     */
    template <typename T> T to_intx() const {
        return static_cast<T>(to_int());
    }

    /**
     * @return the underlying double value.
     *
     * @exception bad_json_type When called on objects having type other than @c Double.
     */
    double to_double() const;

    /**
     * @return A const-reference to the underlying string value.
     *
     * @exception bad_json_type When called on objects having type other than @c String.
     */
    const std::string& to_string() const;

    /**
     * @return A string representation of the underlying value.
     *
     * @exception bad_json_type When called Objects or Arrays
     */
    std::string to_string(const json_spec_force&) const;

    /**
     * @return A const-reference to the underlying array.
     *
     * @exception bad_json_type When called on objects having type other than @c Array.
     */
    const array_type& to_array() const;

    /**
     * @return A const-reference to the underlying map.
     *
     * @exception bad_json_type When called on objects having type other than @c Object.
     */
    const map_type& to_map() const;

    /**
     * @return An iterator referring to the first element in the container.
     *
     * @exception bad_json_type  When called on objects having type other than @c Object.
     */
    iterator begin();
    const_iterator begin() const;

    /**
     * @return An iterator referring to the past-the-end element in the container.
     *
     * @exception bad_json_type  When called on objects having type other than @c Object.
     */
    iterator end();
    const_iterator end() const;

    /**
     * @brief Looks for an element at key @p key and returns an iterator to it.
     *
     * @exception bad_json_type  When called on objects having type other than @c Object.
     */
    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;

    /**
     * @return A reference to the value at key @p key.
     *
     * @exception bad_json_type When called on objects having type other than @c Object
     * @exception key_not_found When the @p key is not found (only in const version)
     *
     * @note The non-const version inserts a new element (initialy invalid) if the key
     * does not exist.
     */
    const Value& operator[](const key_type& key) const;
    Value& operator[](const key_type& key);

    /**
     * @return A reference to the value at @c key.
     *
     * @exception bad_json_type When called on objects having type other than @c Object
     * @exception key_not_found When the @p key is not found (only in const version)
     *
     * @note The non-const version inserts a new element (initialy invalid) if the key
     * does not exist.
     */
    const Value& at(const key_type& key) const;
    Value& at(const key_type& key);

    /**
     * @brief Removes the element with key @p key from the container.
     *
     * @exception bad_json_type When called on objects having type other than @c Object.
     */
    void erase(const key_type& key);

    /**
     * @return An iterator referring to the first element in the array.
     *
     * @exception bad_json_type  When called on objects having type other than @c Array.
     */
    array_iterator abegin();
    const_array_iterator abegin() const;

    /**
     * @return An iterator to past-the end of the array.
     *
     * @exception bad_json_type  When called on objects having type other than @c Array.
     */
    array_iterator aend();
    const_array_iterator aend() const;

    /**
     * @return A const reference to the @p idx-th element in the array.
     *
     * @exception bad_json_type When called on objects having type other than @c Array.
     * @exception std::out_of_range When @p idx is out of bounds.
     */
    const Value& operator[](size_t idx) const;
    Value& operator[](size_t idx);

    /**
     * @return A const reference to the @p idx-th element in the array.
     *
     * @exception bad_json_type When called on objects having type other than @c Array.
     * @exception std::out_of_range When @p idx is out of bounds.
     */
    const Value& at(size_t idx) const;
    Value& at(size_t idx);

    /**
     * Inserts an element at the end of the underlying array.
     *
     * @return A reference to the appended element
     *
     * @exception bad_json_type When called on objects having type other than @c Array.
     */
    Value& push_back(const Value& val);

    /**
     * Appends new elements to this JSON Array
     *
     * @return *this
     * @exception bad_json_type When called on objects having type other than @c Array.
     */
    void append(const array_type& a);

    /**
     * Appends new elements to this JSON Array
     *
     * @exception bad_json_type When called on objects having type other than @c Array.key_not_found
     */
    void append(const_array_iterator ibegin, const_array_iterator iend);

    /**
     * @brief Searchs for a value using the JSON Pointer standard as defined in RFC-6901.
     *
     * @return A const reference to the value identified by the JSON Pointer argument.
     *
     * @param pointer A JSON Pointer object
     *
     * @exception bad_json_type When called on objects having type other than @c Array or @c Object
     * @exception key_not_found When no value is found
     */
    const Value& operator[](const Pointer& pointer) const;

    /**
     * @brief Searchs for a value using the JSON Pointer standard as defined in RFC-6901.
     *
     * @return A const reference to the value identified by the JSON Pointer argument.
     *
     * @param pointer A JSON Pointer object
     *
     * @exception bad_json_type When called on objects having type other than @c Array or @c Object.
     * @exception key_not_found When no value is found
     */
    const Value& at(const Pointer& pointer) const;

    /**
     * @brief Optional get of an inner value of type T, returns a pre-defined default value
     * if the child does not exist. Supported values for T are:
     * @li bool
     * @li int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t
     * @li float, double
     * @li const char *, std::string
     *
     * @return The value at the corresponding key
     *
     * @param key The key of the value
     * @param default_value The value to return in case is not found
     *
     * @exception bad_json_type When the type is incorrect
     *
     * @remarks The type T cannot be @c array_type nor @c map_type.
     */
    template<typename T> T get_optional(const key_type& key, const T &default_value = T()) const;

    /**
     * @brief Equality operator
     *
     * @remarks Floating point number equality is not guaranteed, use with caution
     */
    bool operator==(const Value& other) const;

    /**
     * @brief Inequality operator
     */
    bool operator!=(const Value& other) const;

    /**
     * Converts to JSON notation.
     *
     * @param pretty Causes the resulting string to be pretty-printed.
     *
     * @return The JSON string.
     */
    std::string stringify(bool pretty = false) const;

    /**
     * Converts to JSON notation. The result is writen to the stream @p os
     *
     * @param os The stream to write into.
     * @param pretty Causes the resulting string to be pretty-printed.
     */
    void stringify(std::ostream& os, bool pretty = false) const;

    /**
     * Parses the character string @p spec.
     *
     * @param spec The character string to parse
     * @param size Size of the character string
     *
     * @return *this
     *
     * @exception parse_error
     */
    Value& parse(const char *spec, size_t size);

    /**
     * Parses the string @p spec.
     *
     * @param spec The string to parse
     *
     * @return *this
     *
     * @exception parse_error
     */
    Value& parse(const std::string& spec);

    /**
     * Parses the stream @p stream.
     *
     * @param stream The stream to read from
     *
     * @return *this
     *
     * @exception parse_error
     */
    Value& parse(std::istream& stream);

private:
    /* spec used for private contructors */
    static const struct json_spec_private{} option_private;

    /* constructor for null */
    Value(const json_spec_private&);

    /* constructor for invalid object */
    Value(const json_spec_private&, const std::string&);

    struct Union {
        bool         bool_val;
        int64_t      int_val;
        double       dbl_val;
        std::string  str_val;
        array_type   arr_val;
        map_type     map_val;

        Union(){}
        Union(bool v): bool_val(v){}
        Union(int64_t v): int_val(v){}
        Union(double v): dbl_val(v){}
        Union(const char* v): str_val(v){}
        Union(const std::string& v): str_val(v){}
        Union(const array_type& v): arr_val(v){}
        Union(const map_type& v): map_val(v){}
    };

    struct Temporary_Flag {
        bool is_temporary;
        std::string temporary_key;
        Temporary_Flag(): is_temporary(false){}
        Temporary_Flag(const std::string& s): is_temporary(true), temporary_key(s){}
    };

    Type::Value    type_;
    Union          value_;
    Temporary_Flag temp_flag_;

    friend class detail::IO; /* parser and printer */
    friend class Pointer; /* will read private data */
};

/***********************/
/* json input / output */
/***********************/

/**
 * @brief Creates a new JSON Value by parsing the character string @p spec.
 *
 * @param spec The character string to parse
 * @param size Size of the character string
 *
 * @return The result JSON
 *
 * @exception parse_error
 */
Value parse(const char *spec, size_t size);

/**
 * @brief Creates a new JSON Value by parsing the string @p spec.
 *
 * @param spec The string to parse
 *
 * @return The result JSON
 *
 * @exception parse_error
 */
Value parse(const std::string& spec);

/**
 * @brief Creates a new @c JSON Value by reading on the input stream @p stream.
 *
 * @param stream The stream to read from
 *
 * @return The result JSON
 *
 * @exception parse_error
 */
Value parse(std::istream& stream);

/**
 * @brief JSON IO manipulator. Controls output formatting
 * for @c operator<<
 */
struct iomanip {
    bool pretty; /* flag */
    static const int iword; /* unique identifier */
};

/**
 * @return IO manipulator for pretty output
 */
const iomanip& pretty();

/********************/
/* inline functions */
/********************/

inline bool Value::is_null() const {
    return type() == Type::Null;
}

inline bool Value::is_boolean() const {
    return type() == Type::Boolean;
}

inline bool Value::is_integer() const {
    return type() == Type::Integer;
}

inline bool Value::is_double() const {
    return type() == Type::Double;
}

inline bool Value::is_string() const {
    return type() == Type::String;
}

inline bool Value::is_array() const {
    return type() == Type::Array;
}

inline bool Value::is_object() const {
    return type() == Type::Object;
}

inline const Value& Value::at(const key_type& key) const {
    return (*this)[key];
}

inline Value& Value::at(const key_type& key) {
    return (*this)[key];
}

inline const Value& Value::at(size_t idx) const {
    return (*this)[idx];
}

inline Value& Value::at(size_t idx) {
    return (*this)[idx];
}

inline const Value& Value::at(const Pointer& pointer) const {
    return (*this)[pointer];
}

/**
 * @brief JSON input stream operator
 *
 * @remarks Throws the same exceptions as @c json::parse
 */
std::istream& operator>>(std::istream& stream, Value& jsonval);

/**
 * @brief JSON output stream operator
 */
std::ostream& operator<<(std::ostream& stream, const Value& jsonval);

/**
 * @brief Sets the manipulator flag in the output stream
 */
std::ostream& operator<<(std::ostream& stream, const iomanip& manip);

} // namespace json

#endif // JSON_VALUE_HPP
