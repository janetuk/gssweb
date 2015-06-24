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

