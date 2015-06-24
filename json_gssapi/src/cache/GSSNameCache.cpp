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

#include <stdexcept>
#include "GSSNameCache.h"
#include "utils/util_base64.h"
#include "utils/util_random.h"

#define KEYLEN 128

GSSNameCache* GSSNameCache::_instance = 0;

GSSNameCache::GSSNameCache()
{

}

GSSNameCache::GSSNameCache ( const GSSNameCache& other )
{
  names = other.names;
}

GSSNameCache::~GSSNameCache()
{

}

GSSNameCache& GSSNameCache::operator= ( const GSSNameCache& other )
{
  names = other.names;
  
  return *this;
}


std::string GSSNameCache::store ( GSSName& data, std::string inKey )
{
  /* Variables      */
  std::string key;
  
  /* Error checking */
  /* Setup          */
  /* Main           */
  // Generate a key
  if ( inKey.length() > 0 )
  {
    key = inKey;
  }
  else if ( !generateKey(key) )
  {
    // Key generation failed.  Eeek!
    throw std::runtime_error("Could not generate random data for an ID");
  }
  
  // Store the key/value pair in the map
  // Store the key in the context for convenience
//   std::cout << "\n==> In GSSNameCache::store, about to store data in the names hash.\n";
  names[key] = &data;
  
  /* Cleanup        */
  /* Return         */
  // Return the key for future reference
  return(key);
}

/*************************************
 * Generate random bytes, and base64 *
 * encode them to be JSONable keys   *
 *************************************/
bool GSSNameCache::generateKey(std::string &key)
{
  /* Variables      */
  unsigned char theKey[KEYLEN];
  
  /* Setup          */
  /* Main           */
  // Generate random byte string
  if (!randGenPseudoRandom(theKey, KEYLEN))
    return(false);

  // Encode the binary string
  base64EncodeStr(theKey, KEYLEN, key);
  /* Cleanup        */
  /* Return         */
  return(true);
}

GSSName& GSSNameCache::retrieve ( std::string key )
{
  /* Variables      */
  //GSSName data;
  
  /* Error checking */
  /* Setup          */
  /* Main           */
  // Maybe do something about data entries that are expired?
  
  /* Cleanup        */
  /* Return         */
  return *(names[key]);
}

GSSNameCache* GSSNameCache::instance()
{
    if (_instance == 0)
      _instance = new GSSNameCache;
    
    return _instance;
}



  /* Variables      */
  /* Error checking */
  /* Setup          */
  /* Main           */
  /* Cleanup        */
  /* Return         */
