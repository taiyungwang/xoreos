/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  Utility templates and functions for working with strings.
 */

#ifndef XOREOS_COMMON_STRING_H
#define XOREOS_COMMON_STRING_H

#include <cstdint>
#include <string>
#include <type_traits>

#include "src/common/system.h"

namespace Common {
namespace String {

/** Print formatted data into a std::string object, similar to sprintf(). */
[[gnu::format(printf, 1, 2)]] std::string format(const char *format, ...);

/** Print formatted data into a std::string object, similar to vsprintf(). */
[[gnu::format(printf, 1, 0)]] std::string formatV(const char *format, va_list args);

/** The function type used by the ctype class functions. */
typedef int (*CTypeFunction)(int);

/** Is the character an ASCII character? */
template<typename T>
inline bool isASCII(T c) {
	using UnsignedType = std::make_unsigned_t<T>;
	UnsignedType u = static_cast<UnsignedType>(c);
	return (u & ~static_cast<UnsignedType>(0x7F)) == 0;
}

/** Internal function for implementing ctype function wrappers. */
template<typename T, CTypeFunction func>
inline bool ctypeWrapper(T c) {
	return isASCII(c) && func(static_cast<unsigned char>(c));
}

/** Is the character an ASCII space character? */
template<typename T>
inline bool isSpace(T c) {
	return ctypeWrapper<T, std::isspace>(c);
}

/** Is the character an ASCII digit character? */
template<typename T>
inline bool isDigit(T c) {
	return ctypeWrapper<T, std::isdigit>(c);
}

/** Is the character an ASCII alphabetic character? */
template<typename T>
inline bool isAlpha(T c) {
	return ctypeWrapper<T, std::isalpha>(c);
}

/** Is the character an ASCII alphanumeric character? */
template<typename T>
inline bool isAlNum(T c) {
	return ctypeWrapper<T, std::isalnum>(c);
}

/** Is the character an ASCII control character? */
template<typename T>
inline bool isCntrl(T c) {
	return ctypeWrapper<T, std::iscntrl>(c);
}

/** Convert an ASCII character to its lowercase equivalent. */
template<typename T>
inline T toLower(T c) {
	// NOTE: If we ever need uppercase<->lowercase mappings for non-ASCII
	//       characters: http://www.unicode.org/reports/tr21/tr21-5.html
	return isASCII(c) ? std::tolower(c) : c;
}

/** Convert an ASCII character to its uppercase equivalent. */
template<typename T>
inline T toUpper(T c) {
	// NOTE: If we ever need uppercase<->lowercase mappings for non-ASCII
	//       characters: http://www.unicode.org/reports/tr21/tr21-5.html
	return isASCII(c) ? std::toupper(c) : c;
}

/** Get a UTF-32 codepoint from a UTF-16 character. */
uint32_t fromUTF16(uint16_t c);

/** Perform a case-insensitive comparison. */
int compareIgnoreCase(const std::string &left, const std::string &right);

/** Perform a case-insensitive comparison. */
inline bool equalsIgnoreCase(const std::string &left, const std::string &right) {
	return compareIgnoreCase(left, right) == 0;
}

/** Perform a case-insensitive comparison. */
int compareIgnoreCase(const char *left, const char *right);

/** Perform a case-insensitive comparison. */
inline bool equalsIgnoreCase(const char *left, const char *right) {
	return compareIgnoreCase(left, right) == 0;
}

} // End of namespace String
} // End of namespace Common

#endif // XOREOS_COMMON_STRING_H
