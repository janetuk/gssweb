/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "GSSCreateSecContextCommand.h"
#include <gssapi.h>
#include <stdlib.h>
#include <string.h>

typedef OM_uint32 (*init_sec_context)(
    OM_uint32 *,        /* minor_status */
    gss_cred_id_t,      /* claimant_cred_handle */
    gss_ctx_id_t *,     /* context_handle */
    gss_name_t,         /* target_name */
    gss_OID,            /* mech_type (used to be const) */
    OM_uint32,          /* req_flags */
    OM_uint32,          /* time_req */
    gss_channel_bindings_t,     /* input_chan_bindings */
    gss_buffer_t,       /* input_token */
    gss_OID *,          /* actual_mech_type */
    gss_buffer_t,       /* output_token */
    OM_uint32 *,        /* ret_flags */
    OM_uint32 *         /* time_req */
);

/* Helper function - import the OID from a string */
gss_OID str_to_oid(const char *mech_type_str = NULL)
{
  /* Variables */
  gss_buffer_t gssbuffOID;
  gss_OID gssoidTargetOID;
  OM_uint32 major;
  OM_uint32 minor;
  
  /* Error checking */
  if (mech_type_str == NULL ||
      *mech_type_str == 0)
    return NULL;
  
  /* Setup */
  gssbuffOID = (gss_buffer_t)malloc(sizeof(gss_buffer_desc_struct));

  /* Main */
  gssbuffOID->value = (void *)(mech_type_str);
  gssbuffOID->length = strlen(mech_type_str);
  major = gss_str_to_oid(&minor, 
			 gssbuffOID, 
			 &gssoidTargetOID);
  if (major != GSS_S_COMPLETE)
    throw "Error converting string to OID: " + minor;

  /* Cleanup */
  free(gssbuffOID);
  
  /* Return */
  return gssoidTargetOID;
}

void
GSSCreateSecContextCommand::execute()
{
  init_sec_context fn = (init_sec_context)function;
  
  input_token = GSS_C_NO_BUFFER;
  output_token = (gss_buffer_t)malloc(sizeof(gss_buffer_desc));
  
  retVal = fn(
    &minor_status,
    GSS_C_NO_CREDENTIAL,
    &context_handle,
    target_name,
    mech_type,
    req_flags,
    time_req,
    GSS_C_NO_CHANNEL_BINDINGS,
    input_token,
    &actual_mech_type,
    output_token,
    &ret_flags,
    &time_rec);
}

const char* GSSCreateSecContextCommand::getTargetDisplayName()
{
  /* Variables */
  gss_buffer_t output_name;
  gss_OID output_type;
  OM_uint32 major, minor;
  const char *ret;
  
  /* error checking */
  
  /* Setup */
  output_name = (gss_buffer_t)malloc(sizeof(gss_buffer_desc_struct));
  output_type = (gss_OID)malloc(sizeof(gss_OID_desc_struct));
  
  /* Main */
  major = gss_display_name(&minor, target_name, output_name, &output_type);
  if (major == GSS_S_COMPLETE)
    ret = (const char *)output_name->value;
  else
    ret = NULL;
  
  /* cleanup */
  free(output_name);
  free(output_type);
  
  /* return */
  return( (char *)(output_name->value) );
}

const char* GSSCreateSecContextCommand::getMechType()
{
  gss_buffer_t output;
  OM_uint32 major, minor;
  const char *retVal;

  /* error checking */
  if (mech_type == NULL)
    return NULL;

  /* Setup */  
  output = (gss_buffer_t)malloc(sizeof(gss_buffer_desc_struct));
  
  /* main */
  major = gss_oid_to_str(&minor, mech_type, output);
  if (major == GSS_S_COMPLETE)
    retVal = (const char *)output->value;
  else
    retVal = NULL;
  
  /* cleanup */
  free(output);
  
  /* return */
  return( retVal );
}

bool GSSCreateSecContextCommand::loadParameters(JSONObject *params)
{
  /* Variables */
  OM_uint32 major, minor;
  gss_buffer_t gssbuffTargetName;
  const char *buffer;
  
  /* Error checking */
  // Should I zeroOut?
  
  /* Setup */
  
  /* Main processing */
  // Easy stuff
  this->time_req = (OM_uint32)( (*params)["time_req"].integer() );
  this->req_flags = (OM_uint32)( (*params)["req_flags"].integer() );
  
  // context_handle
  //   -- just treat the value passed in as correct.
  context_handle = (gss_ctx_id_t)( (*params)["context_handle"].integer() );
  
  // target_name
  buffer = (*params)["target_name"].string();
  if (buffer != NULL && *buffer != 0)
  {
    gssbuffTargetName = (gss_buffer_t)malloc(sizeof(gss_buffer_desc_struct));
    gssbuffTargetName->value = (void *)buffer;
    gssbuffTargetName->length = strlen( buffer );
    
    major = gss_import_name(&minor, 
			    gssbuffTargetName, 
			    GSS_C_NO_OID, 
			    &target_name);
    if (major != GSS_S_COMPLETE)
      throw "Error importing target_name: " + minor;
    
    free(gssbuffTargetName);
  }
  
  // mech_type  
  mech_type = str_to_oid( (*params)["mech_type"].string() );
  
  // input_token
  buffer = (*params)["input_token"].string();
  if (buffer != NULL && *buffer != 0)
  {
    this->input_token = (gss_buffer_t)malloc(sizeof(gss_buffer_desc_struct));
    this->input_token->value = (void *)buffer;
    this->input_token->length = strlen(buffer);
  }
  
  /* Cleanup */
  
  
  /* Return */
  return true;
}

bool GSSCreateSecContextCommand::zeroOut(bool initialized)
{
  /* Error checking */
  /* Variables */
  OM_uint32 minor;
  gss_buffer_t output;
  
  /* Setup */
  output = (gss_buffer_t)malloc(sizeof(gss_buffer_desc_struct));
  
  /* Main */

  // Free up existing memory if it's been set.  
  if (initialized)
  {
    if (this->context_handle != NULL)
      gss_delete_sec_context(&minor, &(this->context_handle), output);
    
    if (this->target_name != NULL)
      gss_release_name(&minor, &(this->target_name));
      
    if (mech_type != NULL)
      gss_release_oid(&minor, &(this->mech_type));
    
    if (this->input_token != NULL)
    {
      gss_release_buffer(&minor, this->input_token);
      free(this->input_token);
    }

    if (this->actual_mech_type != NULL)
      gss_release_oid(&minor, &(this->actual_mech_type));
  
    if (this->output_token != NULL)
    {
      gss_release_buffer(&minor, this->output_token);
      free( this->output_token );
    }
  }

  // Now set things to reasonable defaults
  this->retVal = 0;
  this->minor_status = 0;
  this->req_flags = 0;
  this->time_req = 0;
  this->ret_flags = 0;
  this->time_rec = 0;

  this->context_handle = GSS_C_NO_CONTEXT;
  this->target_name = GSS_C_NO_NAME;
  mech_type = str_to_oid( "{ 1 2 840 113554 1 2 1 4 }" );
  this->input_token = GSS_C_NO_BUFFER;
  this->actual_mech_type = GSS_C_NO_OID;
  this->output_token = GSS_C_NO_BUFFER;
  memset((void *)&(this->parameters), 0, sizeof(JSONObject));

  /* Cleanup */
  free(output);
  
  /* Return */
  return(true);
}

GSSCreateSecContextCommand::GSSCreateSecContextCommand(
  JSONObject *params, 
  void *fn) : GSSCommand(params, fn)
{
  zeroOut(false);
  loadParameters(params);
}

GSSCreateSecContextCommand::GSSCreateSecContextCommand(void *fn) : GSSCommand(fn)
{
  zeroOut(false);
}