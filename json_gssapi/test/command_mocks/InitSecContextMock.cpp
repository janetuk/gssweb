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
  nullify((void **)&claimant_cred_handle);
  nullify((void **)&context_handle);
  nullify((void **)&target_name);
  nullify((void **)&mech_type);
  req_flags = 0;
  time_req = 0;
  nullify((void **)&input_chan_bindings);
  input_token.length = 0;
  input_token.value = NULL;
  nullify((void **)&actual_mech_type);
  output_token.length = 0;
  output_token.value = 0;
  ret_flags = 0;
  time_rec = 0;
  visited = false;
  invalidContextHandle = false;

}
