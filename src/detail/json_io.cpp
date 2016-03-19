/*!
 * \file json_io.cpp
 * \author ichramm
 *
 * \date January 20, 2014, 11:33 PM
 *
 * json_object Version 2 - Parser, Printer
 */
#include "json_io.hpp"
#include "detail/encodings.hpp"
#include <cstring> // memset
using namespace json;

namespace
{
	struct parser_status
	{
		int         errorcode;
		bool        topLevel;
		json_config config;
		json_parser parser;
		std::string key;
		std::vector<json_object*>  _stack;

		parser_status(json_object& obj)
		{
			topLevel = true;
			_stack.reserve(30);
			_stack.push_back(&obj);

			memset(&config, 0, sizeof(config));
			config.max_nesting = 1024;
			config.max_data = 100 * 1024 * 1024; //100 MB
			config.allow_c_comments = 1;

			errorcode = json_parser_init(&parser, &config, &parser_callback, this); // TODO: Use a lambda in C++11
		}

		~parser_status()
		{
			json_parser_free(&parser);
		}

		operator bool() const {
			return errorcode == 0;
		}

		bool pseudo_parse(const std::string& str, json_object& json)
		{
			if (str == "null") {
				json = json_object::Null; return true;
			}
			if (str == "true") {
				json = true; return true;
			}
			if (str == "false") {
				json = false; return true;
			}
			if (str[0] == '\"' && str[str.size() - 1] == '\"') {
				json = str.substr(1, str.size() - 2); return true;
			}
			try {
				size_t idx;
				json = detail::stoi64(str, &idx);
				if (str[idx]) { // still data in the string, a double maybe?
					json = detail::stod(str, &idx);
				}
				return !str[idx];
			}
			catch (const std::invalid_argument&) {
				while (false); // avoid compiler warnings, this exception is expected and must be ignored
			}
			return false;
		}

		json_object& set_json_value(json_object& obj, const std::string& key, const json_object& val)
		{
			switch (obj.get_type()) {
				case JST_OBJ:
					return (obj[key] = val); // indexing operator returns a reference to the inserted object
				case JST_ARRAY:
					return obj.push_back(val);
				default:
					throw std::runtime_error("Unexpected json type");
			}
		}

		int parser_callback_impl(int type, const char *data, unsigned int length)
		{
			json_object *obj = _stack.back();

			switch (type) {
				case JSON_OBJECT_BEGIN:
					if (topLevel) {
						topLevel = false;
						*obj = json_object();
					} else {
						_stack.push_back(&set_json_value(*obj, key, json_object()));
					}
					break;
				case JSON_ARRAY_BEGIN:
					if (topLevel) {
						topLevel = false;
						*obj = json_object::json_array();
					} else {
						_stack.push_back(&set_json_value(*obj, key, json_object::json_array()));
					}
					break;
				case JSON_OBJECT_END:	//one level back
				case JSON_ARRAY_END:
					_stack.pop_back();
					break;
				case JSON_KEY:
					key = std::string(data, length);
					break;
				case JSON_TRUE:
					set_json_value(*obj, key, true);
					break;
				case JSON_FALSE:
					set_json_value(*obj, key, false);
					break;
				case JSON_NULL:
					set_json_value(*obj, key, json_object::Null);
					break;
				case JSON_STRING:
					set_json_value(*obj, key, std::string(data, length));
					break;
				case JSON_INT:
					set_json_value(*obj, key, detail::stoi64(data));
					break;
				case JSON_FLOAT:
					set_json_value(*obj, key, detail::stod(data));
					break;
				default:
					return -1;
			}

			return 0;
		}

		static int parser_callback(void *userdata, int type, const char *data, unsigned int length)
		{
			return ((parser_status*)userdata)->parser_callback_impl(type, data, length);
		}
	};
} // namespace

const char *detail::parseerror2str(int errcode)
{
	static const char *libjson_error_strings[] = {
		"success",
		"out of memory",
		"bad character (character < 32, except space newline tab)",
		"trying to pop more object/array than pushed on the stack",
		"trying to pop wrong type of mode. popping array in object mode, vice versa",
		"reach nesting limit on stack",
		"reach data limit on buffer",
		"comment are not allowed with current configuration",
		"unexpected char in the current parser context",
		"unicode low surrogate missing after high surrogate",
		"unicode low surrogate missing without previous high surrogate",
		"found a comma not in structure (array/object)",
		"error in a callback (callback returns error)",
		"utf8 stream is invalid",
		// this one is mine :)
		"incomplete json" // JSON_ERROR_UTF8 + 1
	};

	return libjson_error_strings[errcode];
}

static int json_print_callback(void* userdata, const char* data, unsigned int length) {
	std::ostream *os = (std::ostream *) userdata;
	os->write(data, length);
	return 0;
}

static void json_recursive_print(json_printer* print_handle, const json_object& obj, int(*print_function)(json_printer*, int, const char *, unsigned int))
{
	switch (obj.get_type()) {
		case JST_OBJ: {
			print_function(print_handle, JSON_OBJECT_BEGIN, nullptr, 0);
			json_object::const_iterator it = obj.begin(), end = obj.end();
			for (; it != end; ++it) {
				const std::string& key = it->first;
				print_function(print_handle, JSON_KEY, key.c_str(), key.length());
				json_recursive_print(print_handle, it->second, print_function);
			}
			print_function(print_handle, JSON_OBJECT_END, nullptr, 0);
			break;
		}
		case JST_ARRAY: {
			print_function(print_handle, JSON_ARRAY_BEGIN, nullptr, 0);
			json_object::const_array_iterator it = obj.abegin(), end = obj.aend();
			for (; it != end; ++it) {
				json_recursive_print(print_handle, *it, print_function);
			}
			print_function(print_handle, JSON_ARRAY_END, nullptr, 0);
			break;
		}
		case JST_STRING: {
			std::string str = obj.to_string();
			print_function(print_handle, JSON_STRING, str.c_str(), str.length());
			break;
		}
		case JST_INT: {
			std::string str = detail::to_string(obj.to_int64());
			print_function(print_handle, JSON_INT, str.c_str(), str.size());
			break;
		}
		case JST_DOUBLE:{
			std::string str = detail::to_string(obj.to_double());
			print_function(print_handle, JSON_FLOAT, str.c_str(), str.size());
			break;
		}
		case JST_BOOL:
			print_function(print_handle, obj.to_bool() ? JSON_TRUE : JSON_FALSE, nullptr, 0);
			break;
		default:
			print_function(print_handle, JSON_NULL, nullptr, 0);
			break;
	}
}


int detail::parse(json_object& target, const std::string& str, int& line, int& column)
{
	parser_status status(target);
	if (!status) {
		return status.errorcode;
	}

	unsigned int processed = 0;
	int parseError = json_parser_string(
			&status.parser,
			str.c_str(),
			str.size(),
			&processed
		);
	int done = json_parser_is_done(&status.parser);

	if ( (parseError || !done) && !status.pseudo_parse(str, target) ) { // short circuit evaluation
		bool res = status.pseudo_parse(str, target);
		if (res == false) {
			line = 1, column = 0;
			for (unsigned int i = 0; i < processed; ++i) {
				if (str[i] == '\n') {
					column = 0;
					++line;
				} else {
					++column;
				}
			}
			parseError = parseError > 0 ? parseError : JSON_ERROR_UTF8 + 1;
		}
	}

	return parseError;
}

int detail::parse( json_object &target, std::istream& strm, int &line, int &column )
{
	std::string linestr;
	int parseError = 0, done = 0;
	unsigned int processed = 0;

	parser_status status(target);
	if (!status) {
		return status.errorcode;
	}

	line = 0;
	while (strm.good() && !done) {
		getline(strm, linestr);
		line ++;

		linestr = sanitizeUtf8(linestr);

		parseError = json_parser_string(
				&status.parser,
				linestr.c_str(),
				linestr.size(),
				&processed
			);
		done = json_parser_is_done(&status.parser);

		if (parseError) {
			if (status.pseudo_parse(linestr, target)) {
				return 0; // all good
			}
			column = processed;
			return parseError;
		}
	}

	if (!done && !status.pseudo_parse(linestr, target)) {
		column = processed;
		return JSON_ERROR_UTF8 + 1;
	}

	return 0;
}


std::ostream& detail::print(const json_object& obj, std::ostream& os, bool pretty)
{
	json_printer print;

	if (json_print_init(&print, &json_print_callback, &os)) {
		throw std::runtime_error("error initializing json print api");
	}

	json_recursive_print(&print, obj, pretty ? json_print_pretty : json_print_raw);

	json_print_free(&print);

	return os;
}

