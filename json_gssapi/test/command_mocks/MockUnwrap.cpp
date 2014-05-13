/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "MockUnwrap.h"


OM_uint32    MockUnwrap::retVal;
OM_uint32    MockUnwrap::minor_status;
gss_ctx_id_t MockUnwrap::context_handle;
GSSBuffer    MockUnwrap::inputMessageBuffer;
GSSBuffer    MockUnwrap::outputMessageBuffer;
int          MockUnwrap::conf_state;
gss_qop_t    MockUnwrap::qop_state;


void MockUnwrap::reset()
{
  retVal = 0;
  minor_status = 0;
  inputMessageBuffer.setValue(std::string());
  outputMessageBuffer.setValue(std::string());
  conf_state = 0;
  qop_state= 0;
}
