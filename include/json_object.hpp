/*!
 * \file json_object.hpp
 * \author ichramm
 *
 * \date January 16, 2014, 12:59 PM
 *
 * json_object Version 2
 */
#ifndef json_object_json_object_hpp__
#define json_object_json_object_hpp__

#include "detail/compiler.hpp"
#include "detail/json_diffcheck.hpp"

namespace json
{
	/*!
	 * Supported JSON types
	 */
	enum json_type_t
	{
		/*! Null object */
		JST_NULL   = 1,
		/*! Object, a map of key -> json_object */
		JST_OBJ,
		/*! Array of json_object */
		JST_ARRAY,
		/*! String */
		JST_STRING,
		/*! Integer (up to 64 bit) */
		JST_INT,
		/*! Boolean */
		JST_BOOL,
		/*! Floating point number */
		JST_DOUBLE
	};


	/*!
	 * \brief Thrown when the string being parsed does not represent a valid JSON
	 */
	class parse_error : public std::runtime_error
	{
	public:
		parse_error(int line, int column, const char *err, const std::string& context = "");

		virtual ~parse_error() throw();

		int line() const;

		int column() const;

		const char *error() const;

		const std::string& context() const;

	private:
		int         m_line;
		int         m_column;
		const char *m_err;
		std::string m_context;
	};


	/*!
	 * \brief Thrown by \c to_<type> functions when the type is not correct
	 */
	class bad_json_type : public std::runtime_error
	{
	public:
		bad_json_type(json_type_t expected, json_type_t got);
	};


	/*!
	 * Thrown by the indexing operator, or when trying to use an invalid JSON object.
	 *
	 * \remarks The non-const indexing operator inserts a non-valid JSON object, any
	 * attempt to use that object will result on a \c key_not_found exception
	 * being thrown.
	 */
	class key_not_found : public std::runtime_error
	{
	public:
		key_not_found(const std::string &key);
	};

	/*!
	 * Used to call the constructor that parses the \c string instead
	 * of the one which just uses it.
	 */
	extern const struct json_spec_parse {} option_parse;


	/*!
	 * The class json_object
	 */
	class json_object
	{
	public:
		typedef std::vector<json_object>   json_array;
		typedef json_array::iterator       array_iterator;
		typedef json_array::const_iterator const_array_iterator;

		typedef std::map<std::string, json_object> json_map;
		typedef json_map::iterator                 iterator;
		typedef json_map::const_iterator           const_iterator;

	public:
		static const json_object Null; // NULL JSON object

		/*!
		 * Constructs an empty JSON object.
		 */
		json_object(void);

		/*!
		 * Constructs a JSON object with type \c JST_BOOL and having the value \p value.
		 */
		json_object(bool value);

		/*!
		 * Constructs a JSON object with type \c JST_INT and having the value \p value.
		 */
		json_object(int value);

		/*!
		 * Constructs a JSON object with type \c JST_INT and having the value \p value.
		 */
		json_object(std::int64_t value);

		/*!
		 * Constructs a JSON object with type \c JST_DOUBLE and having the value \p value.
		 */
		json_object(double value);

		/*!
		 * Constructs a JSON object with type \c JST_STRING and having the value \p value.
		 *
		 * \note This overload is needed because the compiler chooses the built-in
		 * convertion from \c char* to \c bool instead of the (user-defined) conversion
		 * from \c char* to \c std::string
		 */
		json_object(const char *value);

		/*!
		* Constructs a JSON object with type \c JST_STRING and having the value \p value.
		*/
		json_object(const std::string& value);

		/*!
		 * Constructs a JSON object with type \c JST_STRING and having the value \p value.
		 */
		json_object(const std::wstring& value);

		/*!
		 * Constructs a JSON object with type \c JST_ARRAY and having the value \p value.
		 */
		json_object(const json_array& value);

		/*!
		 * Constructs a JSON object by parsing the string \p str.
		 * \exception parse_error
		 * \exception std::bad_alloc
		 */
		json_object(const json_spec_parse&, const std::string& str);

		/*!
		 * Constructs a JSON object by parsing the content of the stream \p stream.
		 * \exception parse_error
		 * \exception std::bad_alloc
		 */
		json_object(std::istream& stream);

#if JSONOBJECT_HAVE_CXX11
		/*!
		 * Constructs a JSON object with type \c JST_OBJ and having the elements
		 * provided in the initializer list \p childs as childs
		 *
		 * \param childs An initializer list of key-value pairs
		 *
		 * This constructos gets called when the \c json_object is instantiated as in the
		 * following code:
		 * \code
		 *   json::json_object obj{
		 *       { "hello", "world" } // 'hello' is the key, 'world' is the value, type is \c string
		 *       { "age"  , 27      } // 'age' is the key, 27 is the value, type is \c int
		 *       { "compo", {         // 'compo' is the key, and the value is a JSON object
		 *           { "site", "one" } // nested object
		 *       }
		 *   };
		 * \endcode
		 *
		 * \note If you want to construct a JSON array use something like the
		 * following (note the double brackets):
		 * \code
		 *    json_object obj{{ "one", "two", 3, 4.0, false }};
		 * \endcode
		 * The previous code ends-up calling \c json_object::json_object(const json_array&) beacause
		 * it takes advantage of \c std::vector constructor
		 *
		 * \note Visual Studio Users: Initializer list calls object destructor twice!
		 * \see http://goo.gl/WDLJq0 and http://goo.gl/ZGcAcs
		 */
		json_object(std::initializer_list<std::pair<const std::string, json_object>> childs);
#endif

		/*!
		 * \return This JSON object's type. \see json_type
		 */
		json_type_t get_type() const;

		/*
		 * \return \c true if this JSON Object's type is \c JST_BOOL
		 */
		bool is_boolean() const;

		/*
		 * \return \c true if this JSON Object's type is \c JST_INT
		 */
		bool is_integer() const;

		/*
		 * \return \c true if this JSON Object's type is \c JST_DOUBLE
		 */
		bool is_double() const;

		/*
		 * \return \c true if this JSON Object's type is \c JST_STRING
		 */
		bool is_string() const;

		/*
		 * \return \c true if this JSON Object's type is \c JST_ARRAY
		 */
		bool is_array() const;

		/*
		 * \return \c true if this JSON Object's type is \c JST_OBJ
		 */
		bool is_object() const;

		/*!
		 * \return \c true if one of the following conditions is met:
		 * \li This objects has type \c JST_OBJ and its underlying container is empty.
		 * \li This objects has type \c JST_ARRAY and its underlying array is empty.
		 */
		bool is_empty() const;

		/*!
		 * \return The size of the underlying container.
		 * \exception bad_json_type When called on objects having type other
		 * than \c JST_OBJ or \c JST_ARRAY.
		 */
		size_t size() const;

		/*!
		 * \return The difference state for this object
		 */
		diff_state_t get_diff_state() const;

		/*!
		 * \return \c true if this JSON object has type JST_OBJ and contains a child
		 * having a key \p key.
		 * \exception bad_json_type When called on objects having type other than \c JST_OBJ.
		 */
		bool exists(const std::string& key) const;

		/*!
		 * Equality operator
		 */
		bool operator==(const json_object& other) const;

		/*!
		 * \return the opposite of \ operator==
		 */
		bool operator!=(const json_object& other) const;

		/*!
		 * \return the internal value as a unicode string
		 * \exception bad_json_type When called on objects having type other than \c JST_STRING.
		 */
		std::string to_string() const;

		/*!
		 * \return the internal value as a unicode string.
		 * \exception bad_json_type When called on objects having type other than \c JST_STRING.
		 */
		std::wstring to_wstring() const;

		/*!
		 * \return the internal value as boolean.
		 * \exception bad_json_type When called on objects having type other than \c JST_BOOL.
		 */
		bool to_bool() const;

		/*!
		 * \return the internal value as int
		 * \exception bad_json_type When called on objects having type other than \c JST_INT.
		 */
		int to_int() const;

		/*!
		 * \return the internal value as a 64-bit int.
		 * \exception bad_json_type When called on objects having type other than \c JST_INT.
		 */
		std::int64_t to_int64() const;

		/*!
		 * \return the internal value as double.
		 * \exception bad_json_type When called on objects having type other than \c JST_DOUBLE.
		 */
		double to_double() const;

		/*!
		 * \return the internal value as array.
		 * \exception bad_json_type When called on objects having type other than \c JST_ARRAY.
		 */
		json_array to_array() const;

		/*!
		 * \return An iterator referring to the first element in the container.
		 * \exception bad_json_type  When called on objects having type other than \c JST_OBJ.
		 */
		iterator begin();

		/*!
		 * \return An iterator referring to the first element in the container.
		 * \exception bad_json_type  When called on objects having type other than \c JST_OBJ.
		 */
		const_iterator begin() const;

		/*!
		 * \return An iterator referring to the past-the-end element in the container.
		 * \exception bad_json_type  When called on objects having type other than \c JST_OBJ.
		 */
		iterator end();

		/*!
		 * \return An iterator referring to the past-the-end element in the container.
		 * \exception bad_json_type  When called on objects having type other than \c JST_OBJ.
		 */
		const_iterator end() const;

		/*!
		 * Searches the container for an element with a key equivalent to \p key and returns
		 * an iterator to it if found, otherwise it returns an iterator to \c end().
		 * \exception bad_json_type  When called on objects having type other than \c JST_OBJ.
		 */
		iterator find(const std::string& key);

		/*!
		 * Searches the container for an element with a key equivalent to \p key and returns
		 * an iterator to it if found, otherwise it returns an iterator to \c end().
		 * \exception bad_json_type  When called on objects having type other than \c JST_OBJ
		 */
		const_iterator find(const std::string& key) const;

		/*!
		 * If \p key matches the key of an element in the container, the
		 * function returns a reference to its mapped value.
		 *
		 * If \p key does not match the key of any element in the container, the
		 * function throws an \c key_not_found exception.
		 *
		 * \exception bad_json_type When called on objects having type other than \c JST_OBJ
		 * \exception key_not_found When the \p key is not found
		 */
		const json_object& operator[](const std::string& key) const;

		/*!
		 * If \p key matches the key of an element in the container, the
		 * function returns a reference to its mapped value.
		 *
		 * If k does not match the key of any element in the container, the function
		 * inserts a new INVALID JSON element with that key and returns a reference to its
		 * mapped value. Any attempt to use that element will result in a \c key_not_found exception
		 * being thrown.
		 *
		 * \exception bad_json_type  When called on objects having type other than \c JST_OBJ
		 */
		json_object& operator[](const std::string& key);

		/*!
		 * Removes the element with key \p k from the container.
		 * \exception bad_json_type When called on objects having type other than \c JST_OBJ.
		 */
		void erase(const std::string& k);

		/*!
		 * \return An iterator referring to the first element in the array.
		 * \exception bad_json_type  When called on objects having type other than \c JST_ARRAY.
		 */
		array_iterator abegin();

		/*!
		 * \return A const-iterator referring to the first element in the array.
		 * \exception bad_json_type  When called on objects having type other than \c JST_ARRAY.
		 */
		const_array_iterator abegin() const;

		/*!
		 * \return An iterator to past-the end of the array.
		 * \exception bad_json_type  When called on objects having type other than \c JST_ARRAY.
		 */
		array_iterator aend();

		/*!
		 * \return A const-iterator to the past-the-end element in the array.
		 * \exception bad_json_type  When called on objects having type other than \c JST_ARRAY.
		 */
		const_array_iterator aend() const;

		/*!
		 * \return A const reference to the \p idx-th element in the array.
		 * \exception bad_json_type When called on objects having type other than \c JST_ARRAY.
		 * \exception std::out_of_range When \p idx is out of bounds.
		 */
		const json_object& operator[](size_t idx) const;

		/*!
		 * \return A reference to the \p idx-th element in the array.
		 * \exception bad_json_type When called on objects having type other than \c JST_ARRAY.
		 * \exception std::out_of_range When \p idx is out of bounds.
		 */
		json_object& operator[](size_t idx);

		/*!
		 * Inserts an element at the end of the underlying array.
		 * \return A reference to the newly added element.
		 * \exception bad_json_type When called on objects having type other than \c JST_ARRAY.
		 */
		json_object& push_back(const json_object &obj);

		/*!
		 * Appends new elements to this JSON Array
		 *
		 * \exception bad_json_type When called on objects having type other than \c JST_ARRAY.
		 */
		void append(const json_array& a);

		/*!
		 * Appends new elements to this JSON Array
		 *
		 * \exception bad_json_type When called on objects having type other than \c JST_ARRAY.
		 */
		void append(const_array_iterator ibegin, const_array_iterator iend);

		/*!
		 * Converts an object to JSON notation.
		 * \param pretty Causes the resulting string to be pretty-printed.
		 * \return The JSON string.
		 *
		 * \exception runtime_error If all else fails..
		 */
		std::string stringify(bool pretty = true) const;

		/*!
		 * Converts an object to JSON notation.
		 * This function writes its output in the stream \p os instead of writing it to a string.
		 * \param os The stream to write into.
		 * \param pretty Causes the resulting string to be pretty-printed.
		 *
		 * \exception runtime_error If all else fails (http://goo.gl/j5M2ki)
		 */
		void stringify(std::ostream &os, bool pretty = true) const;

		/*!
		 * Parses \p str
		 * \return *this
		 * \exception std::bad_alloc
		 * \exception parse_error
		 */
		json_object& parse(const std::string& str);

		/*!
		 * Reads and tries to parse the stream \p strm.
		 * \return *this
		 * \exception std::bad_alloc
		 * \exception parse_error
		 */
		json_object& parse(std::istream& strm);

	private:

		/*! Internal function used to initialize the \c Null object */
		static json_object create_null_object();

		/*! Internal function used by non-const indexing operators */
		static json_object create_from_search(const std::string& search_key);

		/*! object type */
		json_type_t  m_type;

		/*! internal variable indicating whether this objects is valid or not */
		bool         m_valid;

		/*! internal variable used in conjunction with \c _valid
		 * contains the search key used when the object was created */
		std::string  m_search_key;

		/*! The diff state for this object */
		diff_state_t m_diff_state;

		/*!
		 * An union-like value-holder
		 */
		struct inner_value {
			inner_value()
				{}
			inner_value(const json_array& v)
				: arrVal(v)  {}
			inner_value(const json_map& v)
				: mapVal(v)    {}
			inner_value(const std::string& v)
				: strVal(v) {}
			inner_value(std::int64_t v)
				: intVal(v)    {}
			inner_value(double v)
				: doubleVal(v) {}
			inner_value(bool v)
				: boolVal(v)   {}
			json_array   arrVal;
			json_map     mapVal;
			std::string  strVal;
			std::int64_t intVal;
			double       doubleVal;
			bool         boolVal;
		} m_value;

		friend json_object  json::compute_diff(const json_object&, const json_object&);
		friend json_object& json::detail::set_diff_state_deep(json_object&, diff_state_t);
	}; // class json_object

	/*!
	 * Creates a new \c json_object by parsing \p str
	 *
	 * \exception std::bad_alloc
	 * \exception parse_error
	 */
	json_object parse(const std::string& jsonString);

	/*!
	 * Creates a new \c json_object by reading on \p stream.
	 *
	 * \exception std::bad_alloc
	 * \exception parse_error
	 */
	json_object parse(std::istream& jsonStream);

	/* inline member functions */

	inline bool json_object::is_boolean() const {
		return get_type() == JST_BOOL;
	}

	inline bool json_object::is_integer() const {
		return get_type() == JST_INT;
	}

	inline bool json_object::is_double() const {
		return get_type() == JST_DOUBLE;
	}

	inline bool json_object::is_string() const {
		return get_type() == JST_STRING;
	}

	inline bool json_object::is_array() const {
		return get_type() == JST_ARRAY;
	}

	inline bool json_object::is_object() const {
		return get_type() == JST_OBJ;
	}

	/* operator overloading for when the json_object is at the right of the comparator */

	inline bool operator ==(const char *s, const json_object& o) {
		return o.to_string() == s;
	}
	inline bool operator!=(const char *s, const json_object& o) {
		return o.to_string() != s;
	}
	inline bool operator==(const std::string& s, const json_object& o) {
		return o.to_string() == s;
	}
	inline bool operator!=(const std::string& s, const json_object& o) {
		return o.to_string() != s;
	}
	inline bool operator==(const wchar_t *s, const json_object& o) {
		return o.to_wstring() == s;
	}
	inline bool operator==(const std::wstring& s, const json_object& o) {
		return o.to_wstring() == s;
	}
	inline bool operator!=(const std::wstring& s, const json_object& o) {
		return o.to_wstring() != s;
	}
	inline bool operator==(const int& i, const json_object& o) {
		return o.to_int() == i;
	}
	inline bool operator!=(const int& i, const json_object& o) {
		return o.to_int()!= i;
	}
	inline bool operator==(const bool& b, const json_object& o) {
		return o.to_bool() == b;
	}
	inline bool operator!=(const bool& b, const json_object& o) {
		return o.to_bool() != b;
	}
	inline bool operator==(const double& d, const json_object& o) {
		return o.to_double() == d;
	}
	inline bool operator!=(const double& d, const json_object& o) {
		return o.to_double() != d;
	}
} // namespace json

#endif // json_object_json_object_hpp__
