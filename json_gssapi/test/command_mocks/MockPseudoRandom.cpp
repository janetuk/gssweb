/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "MockPseudoRandom.h"

/*
 * static OM_uint32     retVal;
 * static OM_uint32     minor_status;
 * 
 * static gss_ctx_id_t  context_handle;
 * static int           key;
 * static GSSBuffer     inputMessageBuffer;
 * static ssize_t       desiredOutputLength;
 * static GSSBuffer     outputMessageBuffer;
 * 
 * static void reset();
 */

OM_uint32    MockPseudoRandom::retVal;
OM_uint32    MockPseudoRandom::minor_status;
gss_ctx_id_t MockPseudoRandom::context_handle;
int          MockPseudoRandom::key;
GSSBuffer    MockPseudoRandom::inputMessageBuffer;
ssize_t      MockPseudoRandom::desiredOutputLength;
GSSBuffer    MockPseudoRandom::outputMessageBuffer;

void MockPseudoRandom::reset()
{
  retVal = 0;
  minor_status = 0;
  context_handle = GSS_C_NO_CONTEXT;
  key = 0;
  inputMessageBuffer.setValue(std::string());
  desiredOutputLength = 0;
  outputMessageBuffer.setValue(std::string());
  
  return;
}

