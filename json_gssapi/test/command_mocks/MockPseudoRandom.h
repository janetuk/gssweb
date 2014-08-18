/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef MOCKPSEUDORANDOM_H
#define MOCKPSEUDORANDOM_H

#include <datamodel/GSSBuffer.h>
#include <gssapi/gssapi.h>

class MockPseudoRandom
{
public:
  static OM_uint32     retVal;
  static OM_uint32     minor_status;
  
  static gss_ctx_id_t  context_handle;
  static int           key;
  static GSSBuffer     inputMessageBuffer;
  static ssize_t       desiredOutputLength;
  static GSSBuffer     outputMessageBuffer;
  
  static void reset();
};

#endif // MOCKPSEUDORANDOM_H
