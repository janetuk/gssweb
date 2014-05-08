/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSAcquireCred.h"
#include "GSSException.h"

#include <stdexcept>

GSSAcquireCred::GSSAcquireCred(gss_acq_cred_type fn) : function(fn)
{

}

GSSAcquireCred::GSSAcquireCred ( const GSSAcquireCred& other )
{

}

GSSAcquireCred::~GSSAcquireCred()
{

}

GSSAcquireCred::GSSAcquireCred ( 
    JSONObject *params, 
    gss_acq_cred_type fn
) : GSSCommand ( params )
{
  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */
  loadParameters(params);
  function = fn;
  /* Cleanup */
  /* Return */
}


bool GSSAcquireCred::loadParameters(JSONObject *params)
{
  /* Variables */
  std::string sCredUsage;
  size_t nDesiredMechs;
  
  /* Error checking */
  /* Setup */
  // Should I zeroOut?
  
  /* Main processing */
  // Easy stuff(*params)
  this->time_req = (*params)["arguments"]["time_req"].integer();

  if ( ! params->get("arguments").get("cred_usage").isNull() )
  {
    if (params->get("arguments").get("cred_usage").isString())
    {
      sCredUsage = params->get("arguments").get("cred_usage").string();
      if (sCredUsage == "GSS_C_BOTH")
        this->cred_usage = GSS_C_BOTH;
      else if (sCredUsage == "GSS_C_INITIATE")
        this->cred_usage = GSS_C_INITIATE;
      else if (sCredUsage == "GSS_C_ACCEPT")
        this->cred_usage = GSS_C_ACCEPT;
      else
        throw std::invalid_argument( std::string("Invalid cred_usage type given: ") + sCredUsage );
    } else if (params->get("arguments").get("cred_usage").isInteger())
      this->cred_usage = (gss_cred_usage_t)( params->get("arguments").get("cred_usage").integer() );
    else
      throw std::invalid_argument( "Unrecognized argument type for cred_usage." );
      }
  
  if ( params->get("arguments").get("desired_mechs").isArray() )
  {
    for (nDesiredMechs = 0; 
         nDesiredMechs < params->get("arguments").get("desired_mechs").size();
         nDesiredMechs++)
    {
      std::string mechStr = params->get("arguments").get("desired_mechs")[nDesiredMechs].string();
      desiredMechs.addOID( GSSOID(mechStr).toGss() );
    }
  } else
    throw std::invalid_argument("Unrecognized desired_mechs array.");
  
  /* Cleanup */
  /* Return */
  return true;
}



void GSSAcquireCred::execute()
{
  /* Variables */
  gss_cred_id_t output_cred_handle;
  gss_OID_set   actual_mechs;
  
  /* Error checking */
  /* Setup */
  /* Main */
  this->retVal = function(
    &this->minor_status,
    this->desired_name.toGss(),
    this->time_req,
    this->desiredMechs.toGss(),
    this->cred_usage,
    &output_cred_handle,
    &actual_mechs,
    &this->time_rec
  );
  
  if (GSS_ERROR(this->retVal) )
  {
    std::string err("Error acquiring credential for user '");
    err += desired_name.toString();
    err += "'.";
    throw GSSException(err, this->retVal, this->minor_status);
  }
  
  this->cred.setValue(output_cred_handle);
  this->actualMechs = actual_mechs;
  
  /* Cleanup */
  /* Return */
}

/* Desired JSON output:
 * 
 * {
 *   "command": "gss_acquire_cred",
 *   "return_values": {
 *     "major_status": 0,
 *     "minor_status": 0,
 *     "cred_handle": "###########",
 *     "actual_mechs": [
 *       "{ 1 2 3 4 }",
 *       "{ 5 6 7 8 }"
 *     ],
 *     "time_rec": 0
 *   }
 * }
 */
JSONObject *GSSAcquireCred::toJSON()
{
  /* Variables */
  JSONObject *ret = new JSONObject();
  JSONObject *values = new JSONObject();
  JSONObject mechs = JSONObject::array();
  JSONObject *temp;
  gss_OID_set gssMechs;
  OM_uint32 i;
  
  /* Error checking */
  
  /* Setup */
  
  /* Main */
  gssMechs = actualMechs.toGss();
  for (i = 0; i < gssMechs->count; i++)
  {
    GSSOID m( gssMechs->elements + i );
    temp = m.toJSONValue();
    std::cout << "\nMech value: " << temp->string();
    mechs.append( *temp );
  }
  values->set("major_status", this->retVal);
  values->set("minor_status", this->minor_status);
  temp = this->cred.toJSONValue();
  values->set("output_cred_handle", *temp );
  values->set("actual_mechs", mechs);
  values->set("time_rec", (int)this->time_rec );
  ret->set("command", "gss_acquire_cred");
  ret->set("return_values", *values);
  
  /* Cleanup */
  
  /* Return */
  return(ret);
}
