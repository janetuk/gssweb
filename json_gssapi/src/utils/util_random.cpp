/*
 * Copyright (c) 2014, 2015 JANET(UK)
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
 * 3. Neither the name of JANET(UK) nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "util_random.h"

#ifdef WIN32
#include <random>
#endif
#ifdef HAVE_OPENSSL
#include <openssl/err.h>
#include <openssl/rand.h>
#endif


bool randGenPseudoRandom(unsigned char *buffer, size_t len)
{
#ifdef WIN32
  std::random_device rd;   // slow; use to seed only!
  std::mt19937 gen(rd());  // seed mersenne twister.
  std::uniform_int_distribution<> dist(0,UCHAR_MAX);
  for (size_t i=0; i<len; i++)
    buffer[i] = dist(gen);
  return true;
#elif defined(HAVE_OPENSSL)
  int  osslReturn = 0;
  bool existingErrors = false;

  /* Error checking */
  // See if there are any queued OpenSSL errors already.
  existingErrors = ( 0 == ERR_peek_error() );

  /* Setup          */
  /* Main           */
  // Generate random byte string
  osslReturn = RAND_pseudo_bytes(buffer, len);

  // Discard the error message if there weren't any OpenSSL errors to begin with.
  if (osslReturn == 1 && !existingErrors)
  {
    while (0 != ERR_get_error() );
    return(false);
  }
  return true;
#else
  #error no pseudo random generator available
#endif
}

