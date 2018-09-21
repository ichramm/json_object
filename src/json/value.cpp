/*!
 * \file value.cpp
 * \author ichramm
 *
 * \date January 16, 2014, 12:59 PM
 */

#include "json/value.hpp"

#include "detail/json_io.hpp"
#include <sstream>

#define OBJECT_VALID_OR_THROW() do { \
    if (temp_flag_.is_temporary) { \
        throw key_not_found(temp_flag_.temporary_key); \
    } \
} while (false)

#define TYPE_VALID_OR_THROW(type) do { \
    if (!((type) & type_)) { \
        throw bad_json_type(static_cast<Type::Value>(type), type_); \
    } \
} while (false)

#define TYPE_VALID_OR_THROW_EX(type, orexpr) do { \
    if (!((type) & type_) && ((orexpr) == false)) { \
        throw bad_json_type(static_cast<Type::Value>(type), type_); \
    } \
} while (false)

namespace json {

/********************/
/* constants */
/********************/

const json_spec_parse option_parse = {};
const json_spec_force option_force = {};
const Value::json_spec_private Value::option_private = {};
const Value Value::Null(option_private);

/***********************/
/* public constructors */
/***********************/

Value::Value(void)
    : type_(Type::Object) {
}

Value::Value(bool value)
    : type_(Type::Boolean),
      value_(value) {
}

Value::Value(int16_t value)
    : type_(Type::Integer),
      value_(static_cast<int64_t>(value)) {
}

Value::Value(uint16_t value)
    : type_(Type::Integer),
      value_(static_cast<int64_t>(value)) {
}

Value::Value(int32_t value)
    : type_(Type::Integer),
      value_(static_cast<int64_t>(value)) {
}

Value::Value(uint32_t value)
    : type_(Type::Integer),
      value_(static_cast<int64_t>(value)) {
}

Value::Value(int64_t value)
    : type_(Type::Integer),
      value_(value) {
}

Value::Value(double value)
    : type_(Type::Double),
      value_(value) {
}

Value::Value(const char *value)
    : type_(Type::String),
      value_(value) {
}

Value::Value(const std::string& value)
    : type_(Type::String),
      value_(value) {
}

Value::Value(const array_type& value)
    : type_(Type::Array),
      value_(value) {
}

Value::Value(const map_type& map)
    : type_(Type::Object),
      value_(map) {
}

Value::Value(const json_spec_parse&, const std::string& spec)
    : type_(Type::Object) /* might change */ {
    parse(spec);
}

Value::Value(std::istream& stream)
    : type_(Type::Object) /* might change */ {
    parse(stream);
}

#if JSON_HAVE_CXX11
Value::Value(std::initializer_list<std::pair<const std::string, Value>> childs)
    : type_{Type::Object} {
    value_.map_val.insert(childs);
}
#endif

/***********************/
/* private contructors */
/***********************/

Value::Value(const json_spec_private&)
    : type_(Type::Null) {
}

Value::Value(const json_spec_private&, const std::string& key)
    : type_(Type::Object),
      temp_flag_(key) {
}

/****************************/
/* public member functions */
/****************************/

Type::Value Value::type() const {
    OBJECT_VALID_OR_THROW();
    return type_;
}

bool Value::empty() const {
    OBJECT_VALID_OR_THROW();
    switch (type_) {
        case Type::Object:
            return value_.map_val.empty();
        case Type::Array:
            return value_.arr_val.empty();
        default:
            return false;
    }
}

size_t Value::size() const {
    OBJECT_VALID_OR_THROW();
    switch (type_) {
        case Type::Object:
            return value_.map_val.size();
        case Type::Array:
            return value_.arr_val.size();
        default:
            throw bad_json_type(static_cast<Type::Value>(Type::Array|Type::Object), type_);
    }
}

bool Value::exists(const key_type& key) const {
    OBJECT_VALID_OR_THROW();
    return find(key) != end();
}

bool Value::exists(const Pointer &pointer) const {
    OBJECT_VALID_OR_THROW();
    TYPE_VALID_OR_THROW_EX(Type::Object, this->empty());
    return pointer.is_valid_for(*this);
}

bool Value::to_bool() const {
    OBJECT_VALID_OR_THROW();
    TYPE_VALID_OR_THROW(Type::Boolean);
    return value_.bool_val;
}

int64_t Value::to_int() const {
    OBJECT_VALID_OR_THROW();
    TYPE_VALID_OR_THROW(Type::Integer);
    return value_.int_val;
}

double Value::to_double() const {
    OBJECT_VALID_OR_THROW();
    TYPE_VALID_OR_THROW(Type::Double|Type::Integer);
    if (type_ == Type::Integer) {
        return static_cast<double>(value_.int_val);
    }
    return value_.dbl_val;
}

const std::string& Value::to_string() const {
    OBJECT_VALID_OR_THROW();
    TYPE_VALID_OR_THROW(Type::String);
    return value_.str_val;
}

std::string Value::to_string(const json_spec_force&) const {
    OBJECT_VALID_OR_THROW();
    switch (type_) {
        case Type::Null:
            return "null";
        case Type::Boolean:
            return value_.bool_val ? "true" : "false";
        case Type::Integer:
            return detail::to_string(value_.int_val);
        case Type::Double:
            return detail::to_string(value_.dbl_val);
        case Type::String:
            return value_.str_val;
        default:
            throw bad_json_type(static_cast<Type::Value>(Type::Array-1), type_);
    }
}

const Value::array_type& Value::to_array() const {
    OBJECT_VALID_OR_THROW();
    TYPE_VALID_OR_THROW_EX(Type::Array, this->empty()); // accept empty objects
    return value_.arr_val;
}

const Value::map_type& Value::to_map() const {
    OBJECT_VALID_OR_THROW();
    TYPE_VALID_OR_THROW_EX(Type::Object, this->empty()); // accept empty objects
    return value_.map_val;
}

/**********************************/
/* object accessors and iterators */
/**********************************/

Value::iterator Value::begin() {
    TYPE_VALID_OR_THROW_EX(Type::Object, this->empty());
    if (type_ != Type::Object) {
        type_ = Type::Object; // change type from array to object
    }
    return value_.map_val.begin();
}

Value::const_iterator Value::begin() const {
    TYPE_VALID_OR_THROW_EX(Type::Object, this->empty());
    return value_.map_val.begin();
}

Value::iterator Value::end() {
    TYPE_VALID_OR_THROW_EX(Type::Object, this->empty());
    if (type_ != Type::Object) {
        type_ = Type::Object; // change type from array to object
    }
    return value_.map_val.end();
}

Value::const_iterator Value::end() const {
    TYPE_VALID_OR_THROW_EX(Type::Object, this->empty());
    return value_.map_val.end();
}

Value::iterator Value::find(const key_type& key) {
    TYPE_VALID_OR_THROW_EX(Type::Object, this->empty());
    if (type_ != Type::Object) {
        type_ = Type::Object; // change type from array to object
    }
    return value_.map_val.find(key);
}

Value::const_iterator Value::find(const key_type& key) const {
    TYPE_VALID_OR_THROW_EX(Type::Object, this->empty());
    if (type_ != Type::Object) {
        return value_.map_val.end();
    }
    return value_.map_val.find(key);
}

Value& Value::operator[](const key_type& key) {
    iterator it = find(key); // throws bad_json_type
    if (it != end()) {
        return it->second;
    }

    // object not found, but this method is not const so the object
    // must be inserted in the map, this allows to build JSON object
    std::pair<iterator, bool> ret = value_.map_val.insert(std::make_pair(key, Value(option_private, key)));
    if (temp_flag_.is_temporary) {
        temp_flag_.is_temporary = false;
        temp_flag_.temporary_key = "";
    }

    return ret.first->second;
}

const Value& Value::operator[](const key_type& key) const {
    const_iterator it = find(key); // throws bad_json_type
    if (it == end()) {
        throw key_not_found(key);
    }
    return it->second;
}

void Value::erase(const key_type& key) {
    iterator it = find(key);
    if (it != end()) {
        value_.map_val.erase(it);
    }
}

Value::array_iterator Value::abegin() {
    TYPE_VALID_OR_THROW_EX(Type::Array, this->empty());
    if (type_ != Type::Array) {
        type_ = Type::Array; // change type from object to array
    }
    return value_.arr_val.begin();
}

Value::const_array_iterator Value::abegin() const {
    TYPE_VALID_OR_THROW_EX(Type::Array, this->empty());
    return value_.arr_val.begin();
}

Value::array_iterator Value::aend() {
    TYPE_VALID_OR_THROW_EX(Type::Array, this->empty());
    if (type_ != Type::Array) {
        type_ = Type::Array; // change type from object to array
    }
    return value_.arr_val.end();
}

Value::const_array_iterator Value::aend() const {
    TYPE_VALID_OR_THROW_EX(Type::Array, this->empty());
    return value_.arr_val.end();
}

const Value& Value::operator[](size_t idx) const {
    TYPE_VALID_OR_THROW(Type::Array);
    return value_.arr_val.at(idx);
}

Value& Value::operator[](size_t idx) {
    TYPE_VALID_OR_THROW(Type::Array);
    return value_.arr_val.at(idx);
}

Value& Value::push_back(const Value& val) {
    bool was_empty = value_.arr_val.empty();
    TYPE_VALID_OR_THROW_EX(Type::Array, (temp_flag_.is_temporary || was_empty));

    value_.arr_val.push_back(val);

    if (temp_flag_.is_temporary || was_empty) {
    // user is building a nested json object
        type_ = Type::Array;
        temp_flag_.is_temporary = false;
        temp_flag_.temporary_key = "";
    }

    return value_.arr_val.back();
}

void Value::append(const array_type& a) {
    append(a.begin(), a.end());
}

void Value::append(const_array_iterator ibegin, const_array_iterator iend) {
    bool was_empty = value_.arr_val.empty();
    TYPE_VALID_OR_THROW_EX(Type::Array, (temp_flag_.is_temporary || was_empty));

    value_.arr_val.insert(value_.arr_val.end(), ibegin, iend);

    if (temp_flag_.is_temporary || was_empty) {
    // user is building a nested json object
        type_ = Type::Array;
        temp_flag_.is_temporary = false;
        temp_flag_.temporary_key = "";
    }
}

const Value& Value::operator[](const Pointer& pointer) const {
    OBJECT_VALID_OR_THROW();
    return pointer.get_reference(*this);
}

template<> bool Value::get_optional<bool>(const key_type& key, const bool &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_bool() : default_value;
}

template<> int16_t Value::get_optional<int16_t>(const key_type& key, const int16_t &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_intx<int16_t>() : default_value;
}

template<> uint16_t Value::get_optional<uint16_t>(const key_type& key, const uint16_t &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_intx<uint16_t>() : default_value;
}

template<> int32_t Value::get_optional<int32_t>(const key_type& key, const int32_t &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_intx<int32_t>() : default_value;
}

template<> uint32_t Value::get_optional<uint32_t>(const key_type& key, const uint32_t &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_intx<uint32_t>() : default_value;
}

template<> int64_t Value::get_optional<int64_t>(const key_type& key, const int64_t &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_int() : default_value;
}

template<> uint64_t Value::get_optional<uint64_t>(const key_type& key, const uint64_t &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_intx<uint64_t>() : default_value;
}

template<> float Value::get_optional<float>(const key_type& key, const float &default_value) const {
    return exists(key) && !at(key).is_null() ? static_cast<float>(at(key).to_double()) : default_value;
}

template<> double Value::get_optional<double>(const key_type& key, const double &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_double() : default_value;
}

typedef const char * const_char_ptr;
template<> const_char_ptr Value::get_optional<const_char_ptr>(const key_type& key, const const_char_ptr &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_string().c_str() : default_value;
}

template<> std::string Value::get_optional<std::string>(const key_type& key, const std::string &default_value) const {
    return exists(key) && !at(key).is_null() ? at(key).to_string() : default_value;
}

/************************/
/* comparison operators */
/************************/

bool Value::operator==(const Value& other) const {
    OBJECT_VALID_OR_THROW();
    if (this == &other) {
        return true;
    }

    if (type_ != other.type_) {
        return false;
    }

    switch (type_) {
        case Type::Null:
            return true;
        case Type::Boolean:
            return value_.bool_val == other.value_.bool_val;
        case Type::Integer:
            return value_.int_val == other.value_.int_val;
        case Type::Double:
            return value_.dbl_val == other.value_.dbl_val;
        case Type::String:
            return value_.str_val == other.value_.str_val;
        default:
            // object or arrays are different even when
            //having the same structure with the same values
            return false;
    }
}

bool Value::operator!=(const Value& other) const {
    OBJECT_VALID_OR_THROW();
    return !(*this == other);
}

/***********************/
/* json input / output */
/***********************/

const int iomanip::iword = std::ios_base::xalloc();

const iomanip& pretty() {
    static const iomanip iom = { true };
    return iom;
}

std::string Value::stringify(bool pretty) const {
    OBJECT_VALID_OR_THROW();
    std::ostringstream oss;
    stringify(oss, pretty);
    return oss.str();
}

void Value::stringify(std::ostream& os, bool pretty) const {
    OBJECT_VALID_OR_THROW();
    detail::IO::print(*this, os, pretty);
}

Value& Value::parse(const char *spec, size_t size) {
    struct membuf: std::streambuf {
        membuf(char* begin, char* end) {
            this->setg(begin, begin, end);
        }
    };
    membuf sbuf(const_cast<char*>(spec), const_cast<char*>(spec + size));
    std::istream stream(&sbuf);
    return parse(stream);
}

Value& Value::parse(const std::string& spec) {
    detail::IO::parse(*this, spec);
    return *this;
}

Value& Value::parse(std::istream& stream) {
    detail::IO::parse(*this, stream);
    return *this;
}

Value parse(const char *spec, size_t size) {
    Value val;
    return val.parse(spec, size);
}

Value parse(const std::string& spec) {
    return Value(option_parse, spec);
}

Value parse(std::istream& stream) {
    return Value(stream);
}

std::istream& operator>>(std::istream& stream, Value& jsonval) {
    jsonval.parse(stream);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Value& jsonval) {
    long int& pretty = stream.iword(iomanip::iword);
    if (pretty) {
        pretty = 0;
        jsonval.stringify(stream, true);
    } else {
        jsonval.stringify(stream, false);
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const iomanip& manip) {
    stream.iword(iomanip::iword) = manip.pretty ? 1 : 0;
    return stream;
}

} // namespace json
