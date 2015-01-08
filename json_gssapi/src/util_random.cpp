/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
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

