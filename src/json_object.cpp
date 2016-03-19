/*!
 * \file json_object.cpp
 * \author ichramm
 *
 * \date January 16, 2014, 12:59 PM
 *
 * json_object Version 2 implementation
 */
#include "json_object.hpp"
#include "detail/encodings.hpp"
#include "detail/json_diffcheck.hpp"
#include "detail/json_io.hpp" // includes json.h

#include <sstream>
#include <string>
#include <stdexcept>

using std::int64_t;
using std::string;
using std::wstring;

#define ASSERT_OBJECT_VALID() do { \
	if (m_valid == false) { \
		throw key_not_found(m_search_key); \
	} \
} while (false)

#define ASSERT_TYPE(type) do { \
	if (m_type != type) { \
		throw bad_json_type(type, m_type); \
	} \
} while (false)

#define ASSERT_TYPE_EX(type, orexpr) do { \
	if (m_type != type && ((orexpr) == false)) { \
		throw bad_json_type(type, m_type); \
	} \
} while (false)

/*!
 * \return a string representation of a \c json_type
 */
static const char *jst2str(json::json_type_t type) {
	static const char *json_type_strings[] =  {
		"null",
		"object",
		"array",
		"string",
		"int",
		"bool",
		"double"
	};
	return json_type_strings[type];
}

namespace json
{
	const json_spec_parse option_parse = {};
	const json_object json_object::Null = json_object::create_null_object();

	/* exceptions */

	parse_error::parse_error(int line, int column, const char *err, const string& context)
	 : runtime_error(string("Parse error at (line ") + detail::to_string(line) + ", column " + detail::to_string(column) + "): " + err)
	 , m_line(line)
	 , m_column(column)
	 , m_err(err)
	 , m_context(context)
	{ }
	parse_error::~parse_error() throw()
	{ }
	int parse_error::line() const {
		return m_line;
	}
	int parse_error::column() const {
		return m_column;
	}
	const char * parse_error::error() const {
		return m_err;
	}
	const string& parse_error::context() const {
		return m_context;
	}

	bad_json_type::bad_json_type(json_type_t expected, json_type_t got)
	 : runtime_error(string("Invalid type, expected ") + jst2str(expected) + ", got " + jst2str(got))
	{ }

	key_not_found::key_not_found(const string& key)
	 : runtime_error(string("Key '") + key + "' does not exist")
	{ }

	/* parsing (free functions) */


	json_object parse(const string& str)
	{
		json_object obj(option_parse, str);
		return obj;
	}

	json_object parse(std::istream& stream)
	{
		json_object obj(stream);
		return obj;
	}

	/* json_object */

	json_object::json_object(void)
	 : m_type(JST_OBJ)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	{ }

	json_object::json_object(bool value)
	 : m_type(JST_BOOL)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	 , m_value(value)
	{ }

	json_object::json_object(int value)
	 : m_type(JST_INT)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	 , m_value((int64_t)value)
	{ }

	json_object::json_object(int64_t value)
	 : m_type(JST_INT)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	 , m_value(value)
	{ }

	json_object::json_object(double value)
	 : m_type(JST_DOUBLE)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	 , m_value(value)
	{ }

	json_object::json_object(const char *value)
	 : m_type(JST_STRING)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	 , m_value(detail::sanitizeUtf8(value))
	{ }

	json_object::json_object(const string& value)
	 : m_type(JST_STRING)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	 , m_value(detail::sanitizeUtf8(value))
	{ }

	json_object::json_object(const wstring& value)
	 : m_type(JST_STRING)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	 , m_value(detail::wstrToUtf8(value))
	{ }

	json_object::json_object(const json_array& value)
	 : m_type(JST_ARRAY)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	 , m_value(value)
	{ }

	json_object::json_object(const json_spec_parse&, const string& str)
	 : m_type(JST_OBJ)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	{
		parse(str);
	}

	json_object::json_object(std::istream& stream)
	 : m_type(JST_OBJ)
	 , m_valid(true)
	 , m_diff_state(JSDS_NONE)
	{
		parse(stream);
	}

#if JSONOBJECT_HAVE_CXX11
	json_object::json_object(std::initializer_list<std::pair<const string, json_object>> childs)
	 : m_type{JST_OBJ}
	 , m_valid{true}
	 , m_diff_state{JSDS_NONE}
	{
		m_value.mapVal.insert(childs);
	}
#endif

	/* private static functions */

	json_object json_object::create_null_object()
	{
		json_object obj;
		obj.m_type = JST_NULL;
		return obj;
	}

	json_object json_object::create_from_search(const string& search_key)
	{
		json_object obj;
		obj.m_valid = false;
		obj.m_search_key = search_key;
		return obj;
	}

	/* public member functions */

	json_type_t json_object::get_type() const
	{
		ASSERT_OBJECT_VALID();
		return m_type;
	}

	bool json_object::is_empty() const
	{
		ASSERT_OBJECT_VALID();
		return (get_type() == JST_OBJ && m_value.mapVal.empty()) || (get_type() == JST_ARRAY && m_value.arrVal.empty());
	}

	size_t json_object::size() const
	{
		ASSERT_OBJECT_VALID();
		switch (m_type) {
		case JST_OBJ:
			return m_value.mapVal.size();
		case JST_ARRAY:
			return m_value.arrVal.size();
		default:
			throw bad_json_type(JST_ARRAY, m_type); // TODO: Allow to specify both
		}
	}

	diff_state_t json_object::get_diff_state() const
	{
		return m_diff_state;
	}

	bool json_object::exists(const string& key) const
	{
		ASSERT_OBJECT_VALID();
		return find(key) != end();
	}

	bool json_object::operator==(const json_object& other) const
	{
		ASSERT_OBJECT_VALID();
		if (this == &other) {
			return true;
		}

		if (m_type == other.m_type) {
			switch (m_type) {
			case JST_NULL:
				return true;
			case JST_STRING:
				return m_value.strVal == other.m_value.strVal;
			case JST_BOOL:
				return m_value.boolVal == other.m_value.boolVal;
			case JST_INT:
				return m_value.intVal == other.m_value.intVal;
			case JST_DOUBLE:
				return m_value.doubleVal == other.m_value.doubleVal;
			default:
				break;
			}
		}

		// not having the same type
		// object or arrays are different even when having the same structure with the same values
		return false;
	}

	bool json_object::operator!=(const json_object& other) const
	{
		ASSERT_OBJECT_VALID();
		return !(*this == other);
	}

	string json_object::to_string() const
	{
		ASSERT_OBJECT_VALID();
		ASSERT_TYPE(JST_STRING);
		return m_value.strVal;
	}

	wstring json_object::to_wstring() const
	{
		ASSERT_OBJECT_VALID();
		ASSERT_TYPE(JST_STRING);
		return detail::utf8toWStr(m_value.strVal);
	}

	bool json_object::to_bool() const
	{
		ASSERT_OBJECT_VALID();
		ASSERT_TYPE(JST_BOOL);
		return m_value.boolVal;
	}

	int json_object::to_int() const
	{
		return static_cast<int>(to_int64());
	}

	int64_t json_object::to_int64() const
	{
		ASSERT_OBJECT_VALID();
		ASSERT_TYPE(JST_INT);
		return m_value.intVal;
	}

	double json_object::to_double() const
	{
		ASSERT_OBJECT_VALID();
		ASSERT_TYPE_EX(JST_DOUBLE, get_type() == JST_INT);
		if (m_type == JST_INT) {
			return static_cast<double>(m_value.intVal);
		}
		return m_value.doubleVal;
	}

	json_object::json_array json_object::to_array() const
	{
		ASSERT_OBJECT_VALID();
		ASSERT_TYPE_EX(JST_ARRAY, is_empty()); // accept empty object as array
		return m_value.arrVal;
	}

	json_object::iterator json_object::begin()
	{
		ASSERT_TYPE_EX(JST_OBJ, is_empty());
		if (m_type != JST_OBJ) {
			m_type = JST_OBJ; // change type from array to object
		}
		return m_value.mapVal.begin();
	}

	json_object::const_iterator json_object::begin() const
	{
		ASSERT_TYPE_EX(JST_OBJ, is_empty());
		return m_value.mapVal.begin();
	}

	json_object::iterator json_object::end()
	{
		ASSERT_TYPE_EX(JST_OBJ, is_empty());
		if (m_type != JST_OBJ) {
			m_type = JST_OBJ; // change type from array to object
		}
		return m_value.mapVal.end();
	}

	json_object::const_iterator json_object::end() const
	{
		ASSERT_TYPE_EX(JST_OBJ, is_empty());
		return m_value.mapVal.end();
	}

	json_object::iterator json_object::find(const string& key)
	{
		ASSERT_TYPE_EX(JST_OBJ, is_empty());
		if (m_type != JST_OBJ) {
			m_type = JST_OBJ; // change type from array to object
			return m_value.mapVal.end();
		}
		return m_value.mapVal.find(key);
	}

	json_object::const_iterator json_object::find(const string& key) const
	{
		ASSERT_TYPE_EX(JST_OBJ, is_empty());
		if (m_type != JST_OBJ) {
			return m_value.mapVal.end();
		}
		return m_value.mapVal.find(key);
	}

	json_object& json_object::operator[](const string& key)
	{
		iterator it = find(key); // throws bad_json_type
		if (it != end()) {
			return it->second;
		}

		// object not found, but this method is not const so the object must be inserted in the map
		std::pair<iterator, bool> ret = m_value.mapVal.insert(make_pair(key, create_from_search(key)));
		if (m_valid == false) {
			m_valid = true;
			m_search_key = "";
		}
		return ret.first->second;
	}

	void json_object::erase(const string& key)
	{
		iterator it = find(key);
		if (it != end()) {
			m_value.mapVal.erase(it);
		}
	}

	json_object::array_iterator json_object::abegin()
	{
		ASSERT_TYPE_EX(JST_ARRAY, is_empty());
		if (m_type != JST_ARRAY) {
			m_type = JST_ARRAY; // change type from object to array
		}
		return m_value.arrVal.begin();
	}

	json_object::const_array_iterator json_object::abegin() const
	{
		ASSERT_TYPE_EX(JST_ARRAY, is_empty());
		return m_value.arrVal.begin();
	}

	json_object::array_iterator json_object::aend()
	{
		ASSERT_TYPE_EX(JST_ARRAY, is_empty());
		if (m_type != JST_ARRAY) {
			m_type = JST_ARRAY; // change type from object to array
		}
		return m_value.arrVal.end();
	}

	json_object::const_array_iterator json_object::aend() const
	{
		ASSERT_TYPE_EX(JST_ARRAY, is_empty());
		return m_value.arrVal.end();
	}

	const json_object& json_object::operator[](const string& key) const
	{
		const_iterator it = find(key); // throws bad_json_type
		if (it == end()) {
			throw key_not_found(key);
		}
		return it->second;
	}

	const json_object& json_object::operator[](size_t idx) const
	{
		ASSERT_TYPE(JST_ARRAY);
		return m_value.arrVal[idx];
	}

	json_object& json_object::operator[](size_t idx)
	{
		ASSERT_TYPE(JST_ARRAY);
		return m_value.arrVal[idx];
	}

	json_object& json_object::push_back(const json_object &obj)
	{
		bool wasEmpty = m_value.arrVal.empty();
		ASSERT_TYPE_EX(JST_ARRAY, (!m_valid || wasEmpty));

		m_value.arrVal.push_back(obj);
		if (m_valid == false || wasEmpty) { // must fix-up this object
			m_valid = true;
			m_type = JST_ARRAY;
			m_search_key = "";
		}

		return m_value.arrVal.back();
	}

	void json_object::append(const json_array &a)
	{
		append(a.begin(), a.end());
	}

	void json_object::append(const_array_iterator ibegin, const_array_iterator iend)
	{
		bool wasEmpty = m_value.arrVal.empty();
		ASSERT_TYPE_EX(JST_ARRAY, (!m_valid || wasEmpty));

		m_value.arrVal.insert(m_value.arrVal.end(), ibegin, iend);
		if (m_valid == false || wasEmpty) { // must fix-up this object
			m_valid = true;
			m_type = JST_ARRAY;
			m_search_key = "";
		}
	}

	string json_object::stringify(bool pretty) const
	{
		std::ostringstream oss;
		stringify(oss, pretty);
		return oss.str();
	}

	void json_object::stringify(std::ostream &os, bool pretty) const
	{
		detail::print(*this, os, pretty);
	}

	json_object& json_object::parse(const string& str)
	{
		int line, column;
		int parseError = detail::parse(*this, detail::sanitizeUtf8(str), line, column);
		if (parseError) {
			throw parse_error(line, column, detail::parseerror2str(parseError));
		}
		return *this;
	}

	json_object& json_object::parse(std::istream& stream)
	{
		int line, column;
		int parseError = detail::parse(*this, stream, line, column);
		if (parseError) {
			throw parse_error(line, column, detail::parseerror2str(parseError));
		}
		return *this;
	}
}
