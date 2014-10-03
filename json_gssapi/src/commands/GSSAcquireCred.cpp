/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSAcquireCred.h"
#include "GSSException.h"
#include <cache/GSSNameCache.h>

#include <stdexcept>

GSSAcquireCred::GSSAcquireCred(gss_acq_cred_type fn) : function(fn)
{
  desired_name = GSS_C_NO_NAME;
  desiredMechs.addOID( GSSOID((char *)"{ 1 3 6 1 5 5 15 1 1 18 }") );
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

  /**************
   * cred_usage *
   **************/
  if ( ! params->get("cred_usage").isNull() )
  {
    if (params->get("cred_usage").isString())
    {
      sCredUsage = params->get("cred_usage").string();
      if (sCredUsage == "GSS_C_BOTH")
        this->cred_usage = GSS_C_BOTH;
      else if (sCredUsage == "GSS_C_INITIATE")
        this->cred_usage = GSS_C_INITIATE;
      else if (sCredUsage == "GSS_C_ACCEPT")
        this->cred_usage = GSS_C_ACCEPT;
      else
        throw std::invalid_argument( std::string("Invalid cred_usage type given: ") + sCredUsage );
    } else if (params->get("cred_usage").isInteger())
      this->cred_usage = (gss_cred_usage_t)( params->get("cred_usage").integer() );
    else
      throw std::invalid_argument( "Unrecognized argument type for cred_usage." );
      }
  
  /*****************
   * desired_mechs *
   *****************/
  if ( ! params->get("desired_mechs").isNull() )
  {
    if ( params->get("desired_mechs").isArray() )
    {
      for (nDesiredMechs = 0; 
          nDesiredMechs < params->get("desired_mechs").size();
          nDesiredMechs++)
      {
        std::string mechStr = params->get("desired_mechs")[nDesiredMechs].string();
        desiredMechs.addOID( GSSOID(mechStr).toGss() );
      }
    } else
      throw std::invalid_argument("Unrecognized desired_mechs array.");
  }

  /****************
   * desired_name *
   ****************/
  if ( ! params->get("desired_name").isNull() )
  {
    std::string key = params->get("desired_name").string();
    this->desired_name = GSSNameCache::instance()->retrieve(key);
  }

  
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
 *     "major_status": 0,
 *     "minor_status": 0,
 *     "cred_handle": "###########",
 *     "actual_mechs": [
 *       "{ 1 2 3 4 }",
 *       "{ 5 6 7 8 }"
 *     ],
 *     "time_rec": 0
 * }
 */
JSONObject *GSSAcquireCred::toJSON()
{
  /* Variables */
  JSONObject *values = new JSONObject();
  JSONObject *temp;
  
  /* Error checking */
  
  /* Setup */
  
  /* Main */
  // Return Values
  // Easy stuff
  values->set("major_status", this->retVal);
  values->set("minor_status", this->minor_status);
  values->set("time_rec", (int)this->time_rec );
  
  // Objects that generate their own JSONObject
  temp = this->cred.toJSONValue();
  values->set("output_cred_handle", *temp );
  
  temp = this->actualMechs.toJSONValue();
  values->set("actual_mechs", *temp);
  
  /* Cleanup */
  
  /* Return */
  return(values);
}
