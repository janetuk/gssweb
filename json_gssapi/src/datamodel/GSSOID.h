/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include <string>
#include "gssapi.h"
#include "utils/util_json.h"

#include "GSSBuffer.h"

#ifndef GSSOID_H
#define GSSOID_H

class GSSOID { 
public:
  GSSOID() { oid = GSS_C_NO_OID; gssInternal = true; };
  GSSOID(char *oid_str);
  GSSOID(std::string oid_str);
  GSSOID(GSSBuffer oid_str);
  GSSOID(gss_OID gssoid);
  GSSOID(const GSSOID &gssoid);
  ~GSSOID();
  
  gss_OID toGss() const { return(oid); };
  std::string toString() const;
  
  bool setValue(GSSBuffer buf);
  bool setValue(gss_OID   gssOID);

  JSONObject *toJSONValue() const;
private:
  gss_OID oid;
  bool gssInternal;
  
  void init(GSSBuffer oid_str);
  void release();
};


#endif // GSSOID_H
