#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "gssapi_utils.h"

OM_uint32
gss_construct_sec_context(
    gss_status_t status,
    gss_cred_id_t unused_cred_handle,
    gss_ctx_id_t *context_handle,
    gss_name_t target_name,
    gss_OID mech_type,
    OM_uint32 req_flags,
    OM_uint32 time_req,
    gss_channel_bindings_t unused_input_chan_bindings)
{
  gss_buffer_desc input_token;
  gss_OID actual_mech_type;
  gss_buffer_desc output_token;
  OM_uint32 ret_flags;
  OM_uint32 time_rec;
  
  gss_cred_id_t cred_handle = GSS_C_NO_CREDENTIAL;
  gss_channel_bindings_t input_chan_bindings = GSS_C_NO_CHANNEL_BINDINGS;
  
  /* Setup - input validation */
  if (status == NULL)
  {
    /* nothing much to be done here */
    return(GSS_S_CALL_INACCESSIBLE_WRITE);
  }
  
  if (target_name == NULL)
  {
    /* Kinda hard to do any meaningful processing here ... */
    status->major = GSS_S_CALL_INACCESSIBLE_READ;
    return(1);
  }
  else
  {
    OM_uint32 maj;
    OM_uint32 min;
    gss_buffer_desc printable;
    gss_OID nametype;

    maj = gss_display_name(&min, target_name, &printable, &nametype);
    
    fprintf(stderr, "target_name:\n");
    fprintf(stderr, "    value: %s\n", (char *)(printable.value) );
    fprintf(stderr, "    nametype: [ length: %u, elements: %s ]\n",
	    nametype->length,
	    (char *)(nametype->elements));
  }
  
  if (mech_type == NULL)
  {
    fprintf(stderr, "NULL mech_type; putting in one for SPNEGO\n");
    /* Defaut to SPNEGO - 1.3.6.1.5.5.2 */
    OM_uint32 maj = 0;
    OM_uint32 min = 0;
    gss_buffer_desc spnego_str;

    char *elems;
    
    spnego_str.value = "1.3.6.1.5.5.2";
    spnego_str.length = 13;
    mech_type = (gss_OID)malloc(sizeof(gss_OID_desc));
    maj = gss_str_to_oid(&min, &spnego_str, &mech_type);

    elems = (char *)malloc(mech_type->length + 1);
    strncpy(elems, (char *)(mech_type->elements), mech_type->length);
    elems[mech_type->length] = '\0';
    fprintf(stderr, "str_to_oid major/minor %u/%u\n", maj, min);
    fprintf(stderr, "mech_type: [length: %u, elements: %s]\n",
	    mech_type->length,
	    elems);
  }
  /* mech_type = GSS_C_NO_OID; */
  
  /* Main processing */
  
  status->major = gss_init_sec_context(&(status->minor),
                                       cred_handle,
                                       context_handle,
                                       target_name,
                                       mech_type,
                                       req_flags,
                                       time_req,
                                       input_chan_bindings,
                                       &input_token,
                                       &actual_mech_type,
                                       &output_token,
                                       &ret_flags,
                                       &time_rec);

  if (status->major != 0)
  {
    OM_uint32 maj;
    OM_uint32 min;
    OM_uint32 context = 0;
    gss_buffer_desc statbuf;
    
    fprintf(stderr, "major status is: 0x%x\n", status->major);
    do {
      maj = gss_display_status(&min, status->major, GSS_C_GSS_CODE, 
			       mech_type, &context, &statbuf);
      fprintf(stderr, "Major status: %s\n", (char *)(statbuf.value));
    } while(context != 0);
    
    if (actual_mech_type == NULL)
      fprintf(stderr, "actual mech type is NULL.\n");
    else
    {
      fprintf(stderr, "actual_mech_type: [length: %u, elements: %s]\n",
	      actual_mech_type->length,
	      (char *)(actual_mech_type->elements));
    }
    
  }
  if (status->minor != 0)
  {
    OM_uint32 maj;
    OM_uint32 min;
    OM_uint32 context = 0;
    gss_buffer_desc statbuf;

    fprintf(stderr, "minor status is: %d/0x%x/%s\n", 
	    status->minor, 
	    status->minor, 
	    strerror(status->minor));
    do {
      maj = gss_display_status(&min, status->minor, GSS_C_MECH_CODE, 
			       GSS_C_NO_OID, &context, &statbuf);
      fprintf(stderr, "Minor status: %s\n", (char *)(statbuf.value));
    } while(context != 0);
  }    
  return(0);
}

