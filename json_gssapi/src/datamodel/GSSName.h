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
  GSSName() {name = GSS_C_NO_NAME; };
  GSSName(std::string nameStr, 
          GSSOID name_type = GSSOID( (gss_OID)GSS_C_NO_OID ), 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(std::string nameStr, 
          gss_OID name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(std::string nameStr, 
          std::string name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);

  GSSName(char *namestr,       
          GSSOID name_type = GSSOID( (gss_OID)GSS_C_NO_OID ), 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(char *namestr,       
          gss_OID name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(char *namestr,       
          std::string name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);

  GSSName(GSSBuffer namestr,   
          GSSOID name_type = GSSOID( (gss_OID)GSS_C_NO_OID ), 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(GSSBuffer namestr,   
          gss_OID name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);
  GSSName(GSSBuffer namestr,   
          std::string name_type, 
          gss_imp_name_type fn = (gss_imp_name_type)&gss_import_name);

  GSSName(gss_name_t gss_name) { name = gss_name; }

  ~GSSName();
  
  
  gss_name_t toGss() const { return(name); }
  std::string toString() const;
  
  bool setValue(gss_name_t newName);
  void setKey(std::string key) { this->hashKey = key; }
  std::string getKey() const { return this->hashKey; }
  
  OM_uint32 getMajorStatus() const { return this->major_status; }
  OM_uint32 getMinorStatus() const { return this->minor_status; }
  
private:
  OM_uint32 major_status, minor_status;
  gss_name_t name;
  gss_imp_name_type function;
  std::string hashKey;
  
  void init(const GSSBuffer namestr, GSSOID name_type, gss_imp_name_type fn);
  void release();
};


#endif // GSSNAME_H
