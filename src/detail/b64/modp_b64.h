/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/**
 * \file
 * <PRE>
 * High performance base64 encoder / decoder
 *
 * Copyright &copy; 2005, 2006, 2007 Nick Galbreath -- nickg [at] modp [dot] com
 * All rights reserved.
 *
 * http://code.google.com/p/stringencoders/
 *
 * Released under bsd license.  See modp_b64.c for details.
 * </pre>
 *
 * This uses the standard base 64 alphabet.  If you are planning
 * to embed a base 64 encoding inside a URL use modp_b64w instead.
 *
 */

#ifndef COM_MODP_STRINGENCODERS_B64
#define COM_MODP_STRINGENCODERS_B64

#ifdef __cplusplus
#define BEGIN_C extern "C" {
#define END_C }
#else
#define BEGIN_C
#define END_C
#endif

BEGIN_C

/* if on motoral, sun, ibm; uncomment this */
/* #define WORDS_BIGENDIAN 1 */
/* else for Intel, Amd; uncomment this */
#undef WORDS_BIGENDIAN

#include "modp_b64_data.h"

#define BADCHAR 0x01FFFFFF

/**
 * you can control if we use padding by commenting out this
 * next line.  However, I highly recommend you use padding and not
 * using it should only be for compatability with a 3rd party.
 * Also, 'no padding' is not tested!
 */
#define DOPAD 1

/*
 * if we aren't doing padding
 * set the pad character to NULL
 */
#ifndef DOPAD
#undef CHARPAD
#define CHARPAD '\0'
#endif


/**
 * Encode a raw binary string into base 64.
 * \param[out] dest should be allocated by the caller to contain
 *   at least modp_b64_encode_len(len) bytes (see below)
 *   This will contain the null-terminated b64 encoded result
 * \param[in] src contains the bytes
 * \param[in] len contains the number of bytes in the src
 * \return length of the destination string plus the ending null byte
 *    i.e.  the result will be equal to strlen(dest) + 1
 *
 * Example
 *
 * \code
 * char* src = ...;
 * int srclen = ...; //the length of number of bytes in src
 * char* dest = (char*) malloc(modp_b64_encode_len);
 * int len = modp_b64_encode(dest, src, sourcelen);
 * if (len == -1) {
 *   printf("Error\n");
 * } else {
 *   printf("b64 = %s\n", dest);
 * }
 * \endcode
 *
 */
static int modp_b64_encode(char* dest, const char* str, int len)
{
    int i;
    const uint8_t* s = (const uint8_t*) str;
    uint8_t* p = (uint8_t*) dest;

    /* unsigned here is important! */
    /* uint8_t is fastest on G4, amd */
    /* uint32_t is fastest on Intel */
    uint32_t t1, t2, t3;

    for (i = 0; i < len - 2; i += 3) {
        t1 = s[i]; t2 = s[i+1]; t3 = s[i+2];
        *p++ = e0[t1];
        *p++ = e1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)];
        *p++ = e1[((t2 & 0x0F) << 2) | ((t3 >> 6) & 0x03)];
        *p++ = e2[t3];
    }

    switch (len - i) {
    case 0:
        break;
    case 1:
        t1 = s[i];
        *p++ = e0[t1];
        *p++ = e1[(t1 & 0x03) << 4];
        *p++ = CHARPAD;
        *p++ = CHARPAD;
        break;
    default: /* case 2 */
        t1 = s[i]; t2 = s[i+1];
        *p++ = e0[t1];
        *p++ = e1[((t1 & 0x03) << 4) | ((t2 >> 4) & 0x0F)];
        *p++ = e2[(t2 & 0x0F) << 2];
        *p++ = CHARPAD;
    }

    *p = '\0';
    return (int)(p - (uint8_t*)dest);
}

/**
 * Decode a base64 encoded string
 *
 * \param[out] dest should be allocated by the caller to contain at least
 *    len * 3 / 4 bytes.  The destination cannot be the same as the source
 *    They must be different buffers.
 * \param[in] src should contain exactly len bytes of b64 characters.
 *     if src contains -any- non-base characters (such as white
 *     space, -1 is returned.
 * \param[in] len is the length of src
 *
 * \return the length (strlen) of the output, or -1 if unable to
 * decode
 *
 * \code
 * char* src = ...;
 * int srclen = ...; // or if you don't know use strlen(src)
 * char* dest = (char*) malloc(modp_b64_decode_len(srclen));
 * int len = modp_b64_decode(dest, src, sourcelen);
 * if (len == -1) { error }
 * \endcode
 */
#ifdef WORDS_BIGENDIAN   /* BIG ENDIAN -- SUN / IBM / MOTOROLA */
static int modp_b64_decode(char* dest, const char* src, int len)
{
    int i;
    if (len == 0) return 0;

#ifdef DOPAD
    /* if padding is used, then the message must be at least
       4 chars and be a multiple of 4.
       there can be at most 2 pad chars at the end */
    if (len < 4 || (len % 4 != 0)) return -1;
    if (src[len-1] == CHARPAD) {
        len--;
        if (src[len -1] == CHARPAD) {
            len--;
        }
    }
#endif  /* DOPAD */

    int leftover = len % 4;
    int chunks = (leftover == 0) ? len / 4 - 1 : len /4;

    uint8_t* p = (uint8_t*) dest;
    uint32_t x = 0;
    uint32_t* destInt = (uint32_t*) p;
    uint32_t* srcInt = (uint32_t*) src;
    uint32_t y = *srcInt++;
    for (i = 0; i < chunks; ++i) {
        x = d0[y >> 24 & 0xff] | d1[y >> 16 & 0xff] |
            d2[y >> 8 & 0xff] | d3[y & 0xff];

        if (x >= BADCHAR)  return -1;
        *destInt = x << 8;
        p += 3;
        destInt = (uint32_t*)p;
        y = *srcInt++;
    }

    switch (leftover) {
    case 0:
        x = d0[y >> 24 & 0xff] | d1[y >> 16 & 0xff] |
            d2[y >>  8 & 0xff] | d3[y & 0xff];
        if (x >= BADCHAR)  return -1;
        *p++ = ((uint8_t*)&x)[1];
        *p++ = ((uint8_t*)&x)[2];
        *p = ((uint8_t*)&x)[3];
        return (chunks+1)*3;
#ifndef DOPAD
    case 1:  /* with padding this is an impossible case */
        x = d3[y >> 24];
        *p =  (uint8_t)x;
        break;
#endif
    case 2:
        x = d3[y >> 24] *64 + d3[(y >> 16) & 0xff];
        *p =  (uint8_t)(x >> 4);
        break;
    default:  /* case 3 */
        x = (d3[y >> 24] *64 + d3[(y >> 16) & 0xff])*64 +
            d3[(y >> 8) & 0xff];
        *p++ = (uint8_t) (x >> 10);
        *p = (uint8_t) (x >> 2);
        break;
    }

    if (x >= BADCHAR) return -1;
    return 3*chunks + (6*leftover)/8;
}

#else /* LITTLE  ENDIAN -- INTEL AND FRIENDS */

static int modp_b64_decode(char* dest, const char* src, int len)
{
    int i;
    if (len == 0) return 0;

#ifdef DOPAD
    /*
     * if padding is used, then the message must be at least
     * 4 chars and be a multiple of 4
     */
    if (len < 4 || (len % 4 != 0)) return -1; /* error */
    /* there can be at most 2 pad chars at the end */
    if (src[len-1] == CHARPAD) {
        len--;
        if (src[len -1] == CHARPAD) {
            len--;
        }
    }
#endif

    int leftover = len % 4;
    int chunks = (leftover == 0) ? len / 4 - 1 : len /4;

    uint8_t* p = (uint8_t*) dest;
    uint32_t x = 0;
    uint32_t* destInt = (uint32_t*) p;
    uint32_t* srcInt = (uint32_t*) src;
    uint32_t y = *srcInt++;
    for (i = 0; i < chunks; ++i) {
        x = d0[y & 0xff] |
            d1[(y >> 8) & 0xff] |
            d2[(y >> 16) & 0xff] |
            d3[(y >> 24) & 0xff];

        if (x >= BADCHAR) return -1;
        *destInt = x ;
        p += 3;
        destInt = (uint32_t*)p;
        y = *srcInt++;}


    switch (leftover) {
    case 0:
        x = d0[y & 0xff] |
            d1[(y >> 8) & 0xff] |
            d2[(y >> 16) & 0xff] |
            d3[(y >> 24) & 0xff];

        if (x >= BADCHAR) return -1;
        *p++ =  ((uint8_t*)(&x))[0];
        *p++ =  ((uint8_t*)(&x))[1];
        *p =    ((uint8_t*)(&x))[2];
        return (chunks+1)*3;
        break;
#ifndef DOPAD
    case 1:  /* with padding this is an impossible case */
        x = d0[y & 0xff];
        *p = *((uint8_t*)(&x)); // i.e. first char/byte in int
        break;
#endif
    case 2: // * case 2, 1  output byte */
        x = d0[y & 0xff] | d1[y >> 8 & 0xff];
        *p = *((uint8_t*)(&x)); // i.e. first char
        break;
    default: /* case 3, 2 output bytes */
        x = d0[y & 0xff] |
            d1[y >> 8 & 0xff ] |
            d2[y >> 16 & 0xff];  /* 0x3c */
        *p++ =  ((uint8_t*)(&x))[0];
        *p =  ((uint8_t*)(&x))[1];
        break;
    }

    if (x >= BADCHAR) return -1;

    return 3*chunks + (6*leftover)/8;
}

#endif  /* if bigendian / else / endif */
/**
 * Given a source string of length len, this returns the amount of
 * memory the destination string should have.
 *
 * remember, this is integer math
 * 3 bytes turn into 4 chars
 * ceiling[len / 3] * 4 + 1
 *
 * +1 is for any extra null.
 */
#define modp_b64_encode_len(A) ((A+2)/3 * 4 + 1)

/**
 * Given a base64 string of length len,
 *   this returns the amount of memory required for output string
 *  It maybe be more than the actual number of bytes written.
 * NOTE: remember this is integer math
 * this allocates a bit more memory than traditional versions of b64
 * decode  4 chars turn into 3 bytes
 * floor[len * 3/4] + 2
 */
#define modp_b64_decode_len(A) (A / 4 * 3 + 2)

/**
 * Will return the strlen of the output from encoding.
 * This may be less than the required number of bytes allocated.
 *
 * This allows you to 'deserialized' a struct
 * \code
 * char* b64encoded = "...";
 * int len = strlen(b64encoded);
 *
 * struct datastuff foo;
 * if (modp_b64_encode_strlen(sizeof(struct datastuff)) != len) {
 *    // wrong size
 *    return false;
 * } else {
 *    // safe to do;
 *    if (modp_b64_decode((char*) &foo, b64encoded, len) == -1) {
 *      // bad characters
 *      return false;
 *    }
 * }
 * // foo is filled out now
 * \endcode
 */
#define modp_b64_encode_strlen(A) ((A + 2)/ 3 * 4)

END_C

#ifdef __cplusplus
#include <cstring>
#include <string>

namespace modp {
    /** \brief b64 encode a cstr with len
     *
     * \param[in] s the input string to encode
     * \param[in] len the length of the input string
     * \return a newly allocated b64 string.  Empty if failed.
     */
    inline std::string b64_encode(const char* s, size_t len)
    {
        std::string x(modp_b64_encode_len(len), '\0');
        int d = modp_b64_encode(const_cast<char*>(x.data()), s,
                                static_cast<int>(len));
        x.erase(d, std::string::npos);
        return x;
    }

    /** \brief b64 encode a cstr
     *
     * \param[in] s the input string to encode
     * \return a newly allocated b64 string.  Empty if failed.
     */
    inline std::string b64_encode(const char* s)
    {
        return b64_encode(s, static_cast<int>(strlen(s)));
    }

    /** \brief b64 encode a const std::string
     *
     * \param[in] s the input string to encode
     * \return a newly allocated b64 string.  Empty if failed.
     */
    inline std::string b64_encode(const std::string& s)
    {
        return b64_encode(s.data(), s.size());
    }

    /**
     * base 64 encode a string (self-modifing)
     *
     * This function is for C++ only (duh)
     *
     * \param[in,out] s the string to be decoded
     * \return a reference to the input string
     */
    inline std::string& b64_encode(std::string& s)
    {
        std::string x(b64_encode(s.data(), s.size()));
        s.swap(x);
        return s;
    }

    inline std::string b64_decode(const char* src, size_t len)
    {
        std::string x(modp_b64_decode_len(len)+1, '\0');
        int d = modp_b64_decode(const_cast<char*>(x.data()), src,
                                static_cast<int>(len));
        if (d < 0) {
            x.clear();
        } else {
            x.erase(d, std::string::npos);
        }
        return x;
    }

    inline std::string b64_decode(const char* src)
    {
        return b64_decode(src, strlen(src));
    }

    /**
     * base 64 decode a string (self-modifing)
     * On failure, the string is empty.
     *
     * This function is for C++ only (duh)
     *
     * \param[in,out] s the string to be decoded
     * \return a reference to the input string
     */
    inline std::string& b64_decode(std::string& s)
    {
        std::string x(b64_decode(s.data(), s.size()));
        s.swap(x);
        return s;
    }

    inline std::string b64_decode(const std::string& s)
    {
        return b64_decode(s.data(), s.size());
    }

}

#endif /* __cplusplus */

#endif /* MODP_B64 */
