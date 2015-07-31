/*
 * Copyright (c) 2014, 2015 JANET(UK)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of JANET(UK) nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "GSSException.h"
#include "datamodel/GSSBuffer.h"
#include <sstream>
#include <string>

static std::string disp_status(OM_uint32 status, int status_type, gss_OID mech)
{
  /* Error checking */
  /* Variables */
  GSSBuffer disp_buf;
  std::string disp_status;
  
  OM_uint32 disp_major, disp_minor, disp_context;

  disp_context = 0;
  do
  {
    disp_major = gss_display_status(&disp_minor, 
				    status, 
			            status_type,
				    mech, 
				    &disp_context, 
				    disp_buf.toGss() );
    if (GSS_ERROR(disp_major))
      throw GSSException("An error occurred while dealing with another error.", disp_major, disp_minor, mech);
    else
      disp_status += disp_buf.toString();
  } while (disp_context != 0);

  return(disp_status);
}

GSSException::GSSException(std::string message, 
			   OM_uint32 major, 
			   OM_uint32 minor, 
			   gss_OID mech)
{
  /* Error checking */
  
  /* Variables */
  /* Setup */
  
  /* Main */
  this->major = major;
  this->minor = minor;
  this->mech = mech;
  this->message = message;

  this->major_msg = disp_status(major, GSS_C_GSS_CODE, mech);
  this->minor_msg = disp_status(minor, GSS_C_MECH_CODE, mech);
  
  /* Cleanup */
  /* Return */
}

const char* GSSException::what(void) const throw()
{
  /* Variables */
  return(this->message.c_str());
}

