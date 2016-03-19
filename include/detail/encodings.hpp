/*!
 * \file encodings.hpp
 * \author ichramm
 *
 * \date January 22, 2014, 10:37 AM
 *
 * json_object Version 2 - Unicode stuff
 */
#ifndef jsonobject_detail_encodings_hpp__
#define jsonobject_detail_encodings_hpp__

#include <string>

namespace json
{
	namespace detail
	{

		/*!
		* Converts unicode to UTF-8
		*/
		std::string wstrToUtf8(const std::wstring& src);

		/*!
		* Converts from UTF-8 to unicode
		*/
		std::wstring utf8toWStr(const std::string& src);

		/*!
		* Sanitizes malformed utf8 strings
		*
		* Tested against http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
		* and http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-demo.txt
		*
		* \warning it considers valid some well formed but invalid Unicode code points
		*/
		std::string sanitizeUtf8(const std::string& src);

		/*!
		 * Decodes a base 64 string
		 */
		std::string fromBase64(const std::string& inBase64);

		/*
		 * Encodes data in base 64
		 */
		std::string toBase64(const char* data, size_t len);
	}
}

#endif // jsonobject_detail_encodings_hpp__
