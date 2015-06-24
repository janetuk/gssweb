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
#include <sstream>
#include <ios>
#include <iostream>

static void disp_status(OM_uint32 status, int status_type, gss_OID mech, std::ostream &output_stream)
{
  OM_uint32 disp_major, disp_minor, disp_context;
  gss_buffer_desc disp_buf;

  disp_context = 0;
  do
  {
    disp_major = gss_display_status(&disp_minor, 
				    status, 
			            status_type,
				    mech, 
				    &disp_context, 
				    &disp_buf);
    if (GSS_ERROR(disp_major))
    {
      disp_context = 0; // Get out of this loop.
      output_stream << "    An error occurred, and then another error" << std::endl <<
         "    occurred in creating a message to describe the first one. " << std::endl <<
         "    How embarrassing. It's time to quit before it gets any worse." <<  std::endl <<
         "    For now, the error code itself was 0x" << std::hex << status << std::dec;
      std::cout << "\n     -------------------------\n    Something wonky happened in displaying status:\n";
      std::cout << "     gss_display_status(&disp_minor, " << status << ", " << (int)status_type << ", " << (long int)mech << ", " << disp_context << ", &buf);\n";
      disp_status(disp_major, GSS_C_GSS_CODE, mech, std::cout);
      disp_status(disp_minor, GSS_C_MECH_CODE, mech, std::cout);
    } else
      output_stream << "    " << (char *)disp_buf.value << std::endl;
  } while (disp_context != 0);
}

GSSException::GSSException(std::string message, 
			   OM_uint32 major, 
			   OM_uint32 minor, 
			   gss_OID mech)
{
  /* Error checking */
  
  /* Variables */
  std::stringstream output_stream;
  /* Setup */
  
  /* Main */
  this->major = major;
  this->minor = minor;
  
  output_stream << message << std::endl;
  output_stream << "GSS Error message:" << std::endl;
  output_stream << "  Major status:" << std::endl;
  disp_status(major, GSS_C_GSS_CODE, mech, output_stream);
  output_stream << std::endl;
  output_stream << "  Minor status details: " << std::endl;
  disp_status(minor, GSS_C_MECH_CODE, mech, output_stream);
  reason = output_stream.str();
  
  
  /* Cleanup */
  /* Return */
}
