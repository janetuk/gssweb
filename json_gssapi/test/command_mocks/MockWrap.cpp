/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "MockWrap.h"


/*
  static OM_uint32     retVal;
  static OM_uint32     minor_status;
  static gss_ctx_id_t  context_handle;
  static int           conf_req_flag;
  static gss_qop_t     qop_req;
  static gss_buffer_t  input_message_buffer;
  static int          *conf_state;
  static gss_buffer_t  output_message_buffer;
*/

OM_uint32    MockWrap::retVal;
OM_uint32    MockWrap::minor_status;
gss_ctx_id_t MockWrap::context_handle;
int          MockWrap::conf_req_flag;
gss_qop_t    MockWrap::qop_req;
GSSBuffer    MockWrap::inputMessageBuffer;
int          MockWrap::conf_state;
GSSBuffer    MockWrap::outputMessageBuffer;


void MockWrap::reset()
{
  retVal = 0;
  minor_status = 0;
  context_handle = GSS_C_NO_CONTEXT;
  conf_req_flag = 0;
  qop_req = 0;
  inputMessageBuffer.setValue(std::string());
  conf_state = 0;
  outputMessageBuffer.setValue(std::string());
}
