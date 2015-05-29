/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "InitSecContextMock.h"
#include <stdio.h>

OM_uint32 InitSecContextMock::retVal = 0;
OM_uint32 InitSecContextMock::minor_status = 0;
OM_uint32 InitSecContextMock::req_flags = 0;
OM_uint32 InitSecContextMock::time_req = 0;
OM_uint32 InitSecContextMock::ret_flags = 0;
OM_uint32 InitSecContextMock::time_rec = 0;

bool InitSecContextMock::visited = false;
bool InitSecContextMock::invalidContextHandle = false;

gss_cred_id_t InitSecContextMock::claimant_cred_handle = NULL;
gss_ctx_id_t  InitSecContextMock::context_handle = NULL;
gss_name_t    InitSecContextMock::target_name = NULL;
gss_OID       InitSecContextMock::mech_type = NULL;
gss_channel_bindings_t InitSecContextMock::input_chan_bindings = NULL;
gss_buffer_desc  InitSecContextMock::input_token;
gss_OID       InitSecContextMock::actual_mech_type = NULL;
gss_buffer_desc  InitSecContextMock::output_token;

void InitSecContextMock::reset()
{
  retVal = 0;
  minor_status = 0;
  nullify(claimant_cred_handle);
  nullify(context_handle);
  nullify(target_name);
  // Our current handling of OIDs means attempting to free one
  // will often result in a double-free or an attempt to free
  // a constant OID. For now, it is better to occasionally leak.
  // We should revisit this -Kevin
  mech_type = GSS_C_NO_OID;
  req_flags = 0;
  time_req = 0;
  nullify(input_chan_bindings);
  input_token.length = 0;
  input_token.value = NULL;
  actual_mech_type = GSS_C_NO_OID;
  output_token.length = 0;
  output_token.value = 0;
  ret_flags = 0;
  time_rec = 0;
  visited = false;
  invalidContextHandle = false;

}
