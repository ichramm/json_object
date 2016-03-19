/*!
 * \file encodings.cpp
 * \author ichramm
 *
 * \date January 22, 2014, 10:37 AM
 *
 * json_object Version 2 - Unicode stuff
 */
#include "detail/encodings.hpp"
#include "b64/modp_b64.h"

using std::string;
using std::wstring;

string json::detail::wstrToUtf8(const wstring& src){
	string dest;
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6) & 0x1f));
			dest.push_back(0x80 | (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12) & 0x0f));
			dest.push_back(0x80 | ((w >> 6) & 0x3f));
			dest.push_back(0x80 | (w & 0x3f));
		}
#ifdef _MSC_VER
#pragma warning(disable: 4333)	//  '>>' : right shift by too large amount, data loss (no aplica en Windows)
#endif
		else if (w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18) & 0x07));
			dest.push_back(0x80 | ((w >> 12) & 0x3f));
			dest.push_back(0x80 | ((w >> 6) & 0x3f));
			dest.push_back(0x80 | (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
	return dest;
}

wstring json::detail::utf8toWStr(const string& src){
	wstring dest;

	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = 0xfffd;
	for (size_t i = 0; i < src.size(); i++){
		unsigned char c = (unsigned char)src[i];
		if (c <= 0x7f){//first byte
			if (bytes){
				dest.push_back(err);
				bytes = 0;
			}
			dest.push_back((wchar_t)c);
		}
		else if (c <= 0xbf){//second/third/etc byte
			if (bytes){
				w = ((w << 6) | (c & 0x3f));
				bytes--;
				if (bytes == 0)
					dest.push_back(w);
			}
			else
				dest.push_back(err);
		}
		else if (c <= 0xdf){//2byte sequence start
			bytes = 1;
			w = c & 0x1f;
		}
		else if (c <= 0xef){//3byte sequence start
			bytes = 2;
			w = c & 0x0f;
		}
		else if (c <= 0xf7){//3byte sequence start
			bytes = 3;
			w = c & 0x07;
		}
		else{
			dest.push_back(err);
			bytes = 0;
		}
	}
	if (bytes)
		dest.push_back(err);

	return dest;
}

string json::detail::sanitizeUtf8(const string& src) {
	string dest;
	int bytes = 0;
	string sequence;
	bool skipSequence = false;
	unsigned char overlongWarn = 0;

	const char errChar = '?';

	size_t i = 0;
	while (i < src.size()) {
		unsigned char c = (unsigned char)src[i];
		if (c <= 0x7f){//first byte
			if (bytes){
				//A single byte char arrives, when a multibyte char is in process
				//discard sequence, no need to resync since we continue with current char
				dest.push_back(errChar);
				sequence.clear();
				bytes = 0;
				overlongWarn = 0;
			}
			dest.push_back(c);
		}
		else if (c <= 0xbf){//second/third/etc byte
			if (bytes && (c & overlongWarn) != 0x80){
				//byte is OK, accumulate it in sequence
				bytes--;
				sequence.push_back(c);
				overlongWarn = 0;
			}
			else {
				//unexpected continuation byte
				//or overlong sequence detected
				skipSequence = true;
			}
		}
		else {//multibyte sequence start

			if (bytes) {
				//discard current sequence
				//no need to skipSequence since we can continue with next sequence
				dest.push_back(errChar);
				sequence.clear();
				overlongWarn = 0;
			}

			//determines how long the sequence is
			//"bytes" will be set with the expected continuation bytes
			for (bytes = 1; c & (0x80 >> bytes); bytes++);
			bytes--; //header counts as one of the bytes

			if (bytes > 6) {
				//no sequence is longer than 6 bytes
				//assume the header is corrupt and discard sequence
				skipSequence = true;
			}
			else {
				//check overlong sequence
				//All possible overlong sequences are explained here http://www.cl.cam.ac.uk/~mgk25/unicode.html#utf-8
				if ((c & 0xfe) == 0xc0) {
					//simplest case of overlong sequence (1100000x)
					skipSequence = true;
				}
				else {
					//more complex case of overlong sequence
					//count off bits from right to left stopping at first on bit
					int zeros = 0;
					for (int mask = 1; !(c & mask); zeros++, mask <<= 1);
					if (zeros + bytes == 7) {
						//byte is one of
						//11100000
						//11110000
						//11111000
						//11111100
						//this may be a sign of an overlong sequence
						//but we don't know until see next byte
						//so we set a mask to flag a warning
						overlongWarn = c;
					}
					sequence.push_back(c);
				}
			}
		}

		if (skipSequence) {
			//skip continuation bytes, adding one error mark for the entire sequence
			dest.push_back(errChar);
			while (i + 1 < src.size() && ((unsigned char)src[i + 1] & 0xc0) == 0x80) {
				i++;
			}
			skipSequence = false;
			sequence.clear();
			bytes = 0;
			overlongWarn = 0;
		}
		else if (!bytes && sequence.size()) {
			//flush sequence
			dest += sequence;
			sequence.clear();
		}
		i++;
	}
	if (bytes || skipSequence) {
		//malformed sequence at the end of string
		dest.push_back(errChar);
	}

	return dest;
}

string json::detail::fromBase64(const string& inBase64)
{
	return modp::b64_decode(inBase64);
}

string json::detail::toBase64(const char* data, size_t len)
{
	return modp::b64_encode(data, len);
}
