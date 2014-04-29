/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef MOCKIMPORTNAME_H
#define MOCKIMPORTNAME_H

#include <gssapi.h>

#include "datamodel/GSSBuffer.h"
#include "datamodel/GSSOID.h"
#include "datamodel/GSSName.h"

class MockImportName
{
public:
  static OM_uint32      retVal;
  static OM_uint32      minor_status;
  static GSSBuffer      input_name_buffer;
  static GSSOID         input_name_type;
  static GSSName        output_name;
  
  static bool           visited;
  
  static void reset();
};

#endif // MOCKIMPORTNAME_H
