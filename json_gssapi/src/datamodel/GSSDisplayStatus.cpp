/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSDisplayStatus.h"

#include <sstream>

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
      output_stream << "Internal, unknown GSS error";
      break; // Get out of this loop.
    } else
      output_stream << (char *)disp_buf.value;
  } while (disp_context != 0);
}


GSSDisplayStatus::GSSDisplayStatus(OM_uint32 major, 
                     OM_uint32 minor,
		     gss_OID   mech  )
{
  std::stringstream major_stream;
  std::stringstream minor_stream;
  /* Setup */
  
  /* Main */
  disp_status(major, GSS_C_GSS_CODE, mech, major_stream);
  majorMessage = major_stream.str();
  
  disp_status(minor, GSS_C_MECH_CODE, mech, minor_stream);
  minorMessage = minor_stream.str();
}

GSSDisplayStatus::~GSSDisplayStatus()
{

}

