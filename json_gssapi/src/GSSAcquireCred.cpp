/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSAcquireCred.h"
#include "GSSException.h"

GSSAcquireCred::GSSAcquireCred(gss_acq_cred_type fn) : function(fn)
{

}

GSSAcquireCred::GSSAcquireCred ( const GSSAcquireCred& other )
{

}

GSSAcquireCred::~GSSAcquireCred()
{

}

GSSAcquireCred& GSSAcquireCred::operator= ( const GSSAcquireCred& other )
{
  return(*this);
}

GSSAcquireCred::GSSAcquireCred ( JSONObject params ) : GSSCommand ( params )
{

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

JSONObject *GSSAcquireCred::toJSON()
{
  return new JSONObject();
}
