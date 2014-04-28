/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include <string>
#include "gssapi.h"

#include "GSSBuffer.h"

#ifndef GSSOID_H
#define GSSOID_H

class GSSOID { 
public:
  GSSOID(char *oid_str);
  GSSOID(std::string oid_str);
  GSSOID(GSSBuffer oid_str);
  GSSOID(gss_OID gssoid);
  ~GSSOID();
  
  gss_OID toGss() { return(oid); };
  std::string toString();

private:
  gss_OID oid;
  bool gssInternal;
  
  void init(GSSBuffer oid_str);
};


#endif // GSSOID_H
