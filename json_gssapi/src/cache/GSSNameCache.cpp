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

#include "GSSNameCache.h"
// #include "util/base64.h"

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
  names[key] = data;
  
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
  return names[key];
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
