/*!
 * \file json_io.hpp
 * \author ichramm
 *
 * \date January 20, 2014, 11:33 PM
 *
 * json_object Version 2 - Parser, Printer
 */
#ifndef jsonobject_v2_json_io_hpp__
#define jsonobject_v2_json_io_hpp__

#include "json_object.hpp"
#include "libjson/json.h"

namespace json
{
	namespace detail
	{
		/*!
		 * \return A human-readable string representing the low-level parse error
		 */
		const char *parseerror2str(int errcode);

		/*!
		 * Parses the string \p spec into \p target
		 */
		int parse(json_object &target, const std::string& spec, int  &line, int &column);

		/*!
		 * Parses the content of the stream \p strm into \p target
		 */
		int parse(json_object &target, std::istream& strm, int  &line, int &column);

		/*!
		 * Printer, used by json_object::stringify()
		 */
		std::ostream &print(const json_object& obj, std::ostream& os, bool pretty);
	}
} // namespace json

#endif //jsonobject_v2_json_io_hpp__
