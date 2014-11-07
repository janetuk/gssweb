/*
 * Copyright (c) 1995-2001 Kungliga Tekniska Högskolan
 * (Royal Institute of Technology, Stockholm, Sweden).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
#include <string>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include "util_base64.h"

#ifdef WIN32
#define GSSWEB_THREAD_ONCE              INIT_ONCE
#define GSSWEB_ONCE_CALLBACK(cb)        BOOL CALLBACK cb(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *Context)
#define GSSWEB_ONCE_LEAVE		do { return TRUE; } while (0)
#define GSSWEB_ONCE(o, i)               InitOnceExecuteOnce((o), (i), NULL, NULL)
#define GSSWEB_ONCE_INITIALIZER         INIT_ONCE_STATIC_INIT
#else
#define GSSWEB_THREAD_ONCE              pthread_once_t
#define GSSWEB_ONCE_CALLBACK(cb)        void cb(void)
#define GSSWEB_ONCE(o, i)               pthread_once((o), (i))
#define GSSWEB_ONCE_INITIALIZER         PTHREAD_ONCE_INIT
#define GSSWEB_ONCE_LEAVE		do { } while (0)
#endif

size_t
base64Size(const char *str);

#define BASE64_EXPAND(n)        (n * 4 / 3 + 4)

static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static char base64_decode[SCHAR_MAX];

static GSSWEB_ONCE_CALLBACK(init_decode_table)
{
    int i;
    memset(&base64_decode[0], -1, sizeof(base64_decode));
    for (i = 0; i < sizeof(base64_chars) / sizeof(base64_chars[0]); i++)
        base64_decode[base64_chars[i]] = (char )i;
    GSSWEB_ONCE_LEAVE;
}

static int
pos(char c)
{
    if (c <= 0)
        return -1;
    return base64_decode[c];
}

void
base64EncodeStr(const void *data, size_t size, std::string &str)
{

    char *s = base64Encode(data, size);
    str = s;
    base64Free(s);
}

char *
base64Encode(const void *data, size_t size)
{
    char *s, *p;
    size_t i;
    int c;
    const unsigned char *q;

    if (size > INT_MAX/4 || size < 0) {
	return NULL;
    }

    p = s = (char *)malloc(BASE64_EXPAND(size));
    if (p == NULL) {
	return NULL;
    }
    q = (const unsigned char *) data;

    for (i = 0; i < size;) {
	c = q[i++];
	c *= 256;
	if (i < size)
	    c += q[i];
	i++;
	c *= 256;
	if (i < size)
	    c += q[i];
	i++;
	p[0] = base64_chars[(c & 0x00fc0000) >> 18];
	p[1] = base64_chars[(c & 0x0003f000) >> 12];
	p[2] = base64_chars[(c & 0x00000fc0) >> 6];
	p[3] = base64_chars[(c & 0x0000003f) >> 0];
	if (i > size)
	    p[3] = '=';
	if (i > size + 1)
	    p[2] = '=';
	p += 4;
    }
    *p = 0;
    return s;
}

#define DECODE_ERROR 0xffffffff

static unsigned int
token_decode(const char *token)
{
    int i, decode;
    unsigned int val = 0;
    unsigned int marker = 0;

    for (i = 0; i < 4; i++) {
	val *= 64;
	if (token[i] == '=')
	    marker++;
	else if (marker > 0)
	    return DECODE_ERROR;
	else {
            decode = pos(token[i]);
            if (decode < 0)
                return DECODE_ERROR;
	    val += decode;
        }
    }
    if (marker > 2)
	return DECODE_ERROR;
    return (marker << 24) | val;
}

void *
base64Decode(const char *str, size_t *size)
{
    static GSSWEB_THREAD_ONCE decode_table_once = GSSWEB_ONCE_INITIALIZER;
    const char *p;
    unsigned char *q;
    void *data = NULL;
    GSSWEB_ONCE(&decode_table_once, init_decode_table);

    *size = base64Size(str);
    if (*size > 0)
        data = malloc(*size);
    if (data == NULL)
        return data;

    q = static_cast<unsigned char *>(data);
    p = str;

    while (*p) {
	unsigned int val = token_decode(p);
	unsigned int marker = (val >> 24) & 0xff;
	if (val == DECODE_ERROR) {
            free(data);
            *size = 0;
	    return NULL;
        }
	*q++ = (val >> 16) & 0xff;
	if (marker < 2)
	    *q++ = (val >> 8) & 0xff;
	if (marker < 1)
	    *q++ = val & 0xff;
	p += 4;
	if (*p == '\n')
	    p++;
    }
    return data;
}

/* Return the size, in bytes, of the data encoded by str
 * Return -1 if str is not a valid base64 encoding
 */
size_t
base64Size(const char *str)
{
    const char *p = str;
    size_t size = 0;

    while (*p) {
	unsigned int val = token_decode(p);
        unsigned int marker = (val >> 24) & 0x3;
	if (val == DECODE_ERROR)
            return 0;

        size += 3 - marker;
	p += 4;
	if (*p == '\n')
	    p++;
    }
    return size;
}

void
base64Free(void *mem)
{
    free(mem);
}