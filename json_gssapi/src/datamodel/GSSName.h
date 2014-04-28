/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include <string>
#include "GSSBuffer.h"
#include "GSSOID.h"

#ifndef GSSNAME_H
#define GSSNAME_H


typedef OM_uint32 (*gss_imp_name_type)(
    OM_uint32 *,        /* minor_status */
    gss_buffer_t,       /* input_name_buffer */
    gss_OID,            /* input_name_type(used to be const) */
    gss_name_t *);      /* output_name */


class GSSName {
public:
  GSSName(std::string nameStr, 
          GSSOID name_type = GSSOID( (gss_OID)GSS_C_NO_OID ), 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(char *namestr,       
          GSSOID name_type = GSSOID( (gss_OID)GSS_C_NO_OID ), 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(GSSBuffer namestr,   
          GSSOID name_type = GSSOID( (gss_OID)GSS_C_NO_OID ), 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);

  GSSName(std::string nameStr, 
          gss_OID name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(char *namestr,       
          gss_OID name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(GSSBuffer namestr,   
          gss_OID name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);

  GSSName(std::string nameStr, 
          std::string name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(char *namestr,       
          std::string name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(GSSBuffer namestr,   
          std::string name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);

  ~GSSName();
  
  
  gss_name_t toGSS() { return(name); }
  std::string toString();
  
private:
  gss_name_t name;
  void init(GSSBuffer namestr, GSSOID name_type, gss_imp_name_type fn);
  gss_imp_name_type function;
};


#endif // GSSNAME_H
