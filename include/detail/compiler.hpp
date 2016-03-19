/*!
 * \file compiler.hpp
 * \author ichramm
 *
 * \date January 20, 2014, 11:41 PM
 *
 * json_object Version 2 - Compiler specific stuff
 */
#ifndef jsonobject_detail_compiler_hpp__
#define jsonobject_detail_compiler_hpp__

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#if __cplusplus >= 201103L || _MSC_VER >= 1700 // Visual C++ 11-12 do not define __cplusplus as 201103L
// C++11
	#define JSONOBJECT_HAVE_CXX11 1
	#include <cstdint>
	#include <initializer_list>

	#if defined(__GNUC__)
		// Cannnot use std::unordered_map because GCC's implementation
		//does not support incomplete types (tested with GCC 4.8.1)
		#include <boost/unordered_map.hpp>
	#else
		#include <unordered_map>
	#endif

	namespace json
	{
		namespace detail
		{ // Import stuff to namespace detail in order to prevent name collision
			using std::move;
			using std::to_string;
			using std::stod;
			#if defined(__GNUC__)
				using boost::unordered_map;
			#else
				using std::unordered_map;
			#endif

			// wrapper a around std::stoll (which returns long long, which is at least 64-bit size)
			inline std::int64_t stoi64(const std::string& str, size_t* idx = 0, int base = 10) {
				return std::stoll(str, idx, base);
			}
		}
	}

#else
// not C++11
	#define JSONOBJECT_HAVE_CXX11 0
	#include <cmath>
	#include <cerrno>
	#include <cstdio>
	#include <cstdlib>
	#include <climits>
	#include <boost/unordered_map.hpp>

	#if defined(_MSC_VER)
		namespace std {
			typedef __int64 int64_t;
		}
	#else // GCC is assumed here
		#include <stdint.h>
		// C99 format macros for int64_t
		#define __STDC_FORMAT_MACROS
		#include <inttypes.h>
		#undef __STDC_FORMAT_MACROS
		namespace std {
			using ::int64_t;
		}
	#endif

	#define nullptr NULL

	namespace json
	{
		namespace detail
		{ // Push stuff into namespace detail in order to prevent name collision
			using boost::unordered_map;

			// lie about move semantics
			template <
				typename T
			> const T& move(const T& t) {
				return t;
			}

			// helper
			template <
				typename T
			> std::string to_string_T( T val, const char *fmt ) {
#if defined(_MSC_VER)
				static __declspec( thread ) char buff[128];
				int len = sprintf_s(buff, sizeof(buff), fmt, val);
#else // GCC
				static __thread char buff[128];
				int len = snprintf(buff, sizeof(buff), fmt, val);
#endif
				return std::string(buff, len);
			}

			// conversion functions available in C++11

			inline std::string to_string(int val) {
				return to_string_T(val, "%d");
			}

			inline std::string to_string(std::int64_t val) {
#if defined(_MSC_VER)
				static const char *fmt = "I64u";
#else
				static const char *fmt = PRId64;
#endif
				return to_string_T(val, fmt);
			}

			inline std::string to_string(double val) {
				return to_string_T(val, "%f");
			}

#if defined(_MSC_VER)
# define strtoll _strtoi64
#endif
			inline std::int64_t stoi64(const std::string& str, size_t* idx = 0, int base = 10) {
				const char *ptr = str.c_str();
				char *eptr;
				errno = 0;

				std::int64_t res = strtoll(ptr, &eptr, base);
				if (ptr == eptr) {
					throw std::invalid_argument("invalid stoi64 argument");
				}
				if (errno == ERANGE) {
					throw std::out_of_range("stoi64 argument out of range");
				}
				if ( idx ) {
					*idx = (size_t)(eptr - ptr);
				}
				return res;
			}
#if defined(_MSC_VER)
# undef strtoll
#endif

			// see http://www.cplusplus.com/reference/string/stod/
			inline double stod(const std::string& str, size_t* idx = 0) {
				const char *ptr = str.c_str();
				char *eptr;
				errno = 0;

				double res = strtod(ptr, &eptr);
				if (ptr == eptr) {
					throw std::invalid_argument("invalid stod argument");
				}
				if (errno == ERANGE) {
					throw std::out_of_range("stod argument out of range");
				}
				if ( idx ) {
					*idx = (size_t)(eptr - ptr);
				}
				return res;
			}
		} // namespace detail
	} // namespace json

#endif

#endif //jsonobject_detail_compiler_hpp__
