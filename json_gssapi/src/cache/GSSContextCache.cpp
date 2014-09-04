/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include <glib.h>
#include <stdexcept>
#include <openssl/err.h>
#include <openssl/rand.h>

#include "GSSContextCache.h"

#define KEYLEN 128

GSSContextCache* GSSContextCache::_instance = 0;

GSSContextCache::GSSContextCache()
{

}

GSSContextCache::~GSSContextCache()
{

}

GSSContextCache* GSSContextCache::instance()
{
    if (_instance == 0)
      _instance = new GSSContextCache;
    
    return _instance;
}


std::string GSSContextCache::store ( GSSContext& data, std::string inKey )
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
  contexts[key] = data;
  
  /* Cleanup        */
  /* Return         */
  // Return the key for future reference
  return(key);
}

/*************************************
 * Generate random bytes, and base64 *
 * encode them to be JSONable keys   *
 *************************************/
bool GSSContextCache::generateKey(std::string &key)
{
  /* Variables      */
  int  osslReturn = 0;
  unsigned char theKey[KEYLEN];
  bool existingErrors = false;
  
  /* Error checking */
  // See if there are any queued OpenSSL errors already.
  existingErrors = ( 0 == ERR_peek_error() ); 
  
  /* Setup          */
  /* Main           */
  // Generate random byte string
  osslReturn = RAND_pseudo_bytes(theKey, KEYLEN);
  
  // Discard the error message if there weren't any OpenSSL errors to begin with.
  if (osslReturn == 1 && !existingErrors)
  {
    while (0 != ERR_get_error() );
    return(false);
  }

  // Encode the binary string
  key = g_base64_encode(theKey, KEYLEN);
  
  /* Cleanup        */
  /* Return         */
  return(true);
}

GSSContext& GSSContextCache::retrieve ( std::string key )
{
  /* Variables      */
  //GSSContext data;
  
  /* Error checking */
  /* Setup          */
  /* Main           */
  // Maybe do something about data entries that are expired?
  
  /* Cleanup        */
  /* Return         */
  return contexts[key];
}

  /* Variables      */
  /* Error checking */
  /* Setup          */
  /* Main           */
  /* Cleanup        */
  /* Return         */
