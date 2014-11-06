/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef GSSBUFFER_H
#define GSSBUFFER_H

#include <string>
#include "gssapi.h"
#include <iostream>

class GSSBuffer {
public:
  GSSBuffer(std::string str = "", bool gssInternal = false);
  GSSBuffer(char *str, bool gssInternal = false);
  GSSBuffer(char *str, int len, bool gssInternal = false);
  GSSBuffer(gss_buffer_t gssbuf, bool gssInternal = false);
  GSSBuffer(const GSSBuffer &other);
  ~GSSBuffer(void);
  
  std::string toString() const { return(std::string((char *)buf->value, buf->length)); }
  gss_buffer_t toGss() const { return( buf ); }
  int length() const { return((int )buf->length); }
  
  void setValue(std::string str);
  void setValue(char *str);
  void setValue(char *str, int len);
  void setValue(gss_buffer_t gssbuf);
  
  bool operator==(GSSBuffer B) const { return(this->toString() == B.toString()); }

private:
  gss_buffer_t buf;
  bool         gssInternal;
  
  void freeBufValue();
  void initEmpty();
};




#endif // GSSBUFFER_H
