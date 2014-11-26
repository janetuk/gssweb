/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
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
