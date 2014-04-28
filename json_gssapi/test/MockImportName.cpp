/*
 * Copyright (c) 2014 Painless Security, LLC.
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 * MockImportName - Mock out the parameters for calling the
 * GSSImportName object's function
 * 
 */

#include "MockImportName.h"
#include <string>

/*
class MockImportName
{
public:
  static OM_uint32              retVal;
  static OM_uint32              minor_status;
  static gss_buffer_t           input_name_buffer;
  static gss_OID                input_name_type;
  static gss_name_t             output_name;
  
  static bool                   visited;
  
  static void reset();
};

*/

OM_uint32 MockImportName::retVal = 0;
OM_uint32 MockImportName::minor_status = 0;
GSSBuffer MockImportName::input_name_buffer = GSSBuffer(std::string(""));
gss_OID MockImportName::input_name_type = NULL;
gss_name_t MockImportName::output_name = NULL;



void MockImportName::reset()
{
  retVal = 0;
  minor_status = 0;
  input_name_buffer.setValue(std::string());
}
