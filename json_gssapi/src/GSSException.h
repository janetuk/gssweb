/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef GSSEXCEPTION_H
#define GSSEXCEPTION_H

#include <exception>
#include <string>

#include <gssapi.h>

class GSSException : public std::exception
{
public:
    GSSException(std::string msg, OM_uint32 major = 0, OM_uint32 minor = 0 , gss_OID mech = GSS_C_NO_OID);
    ~GSSException(void) throw() {  }
      
    virtual const char* what(void) const throw() { return reason.c_str(); }
    
    
private:
    std::string reason;
};

#endif // GSSEXCEPTION_H
