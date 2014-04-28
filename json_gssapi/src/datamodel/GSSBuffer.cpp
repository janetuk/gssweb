/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include <stdexcept>
#include <cstring>
#include "GSSBuffer.h"
#include "../GSSException.h"


/* Constructor pattern:
 * 1) Set gssInternal to false, so that setting values is permitted.
 * 2) Allocate memory for a new gss_buffer_desc
 * 3) Set the buffer length to 0, so that setting values doesn't try
 *    to free memory.
 * 4) Set the value of the buffer.
 * 5) Set the value of gssInternal correctly
 */
GSSBuffer::GSSBuffer ( const GSSBuffer &other )
{
  gss_buffer_t otherBuf = other.buf;
  initEmpty();
  setValue( (char *)(otherBuf->value), otherBuf->length);
  this->gssInternal = false;
}

GSSBuffer::GSSBuffer(std::string str, bool gssInternal)
{
  initEmpty();
  setValue(str);
  this->gssInternal = gssInternal;
}

GSSBuffer::GSSBuffer(char *str, bool gssInternal)
{
  initEmpty();
  setValue(str);
  this->gssInternal = gssInternal;
}

GSSBuffer::GSSBuffer(char *str, int len, bool gssInternal)
{
  initEmpty();
  setValue(str, len);
  this->gssInternal = gssInternal;
}

GSSBuffer::GSSBuffer(gss_buffer_t gssbuf, bool gssInternal)
{
  initEmpty();
  setValue(gssbuf);
  this->gssInternal = gssInternal;
}

GSSBuffer::~GSSBuffer(void)
{
  OM_uint32 major, minor;

  if (this->gssInternal) 
  {
    major = gss_release_buffer(&minor, buf);
    if (GSS_ERROR(major))
    {
      throw GSSException("Error in releasing a buffer allocated by GSS", major, minor);
    }
  } else {
    freeBufValue();
  }
}


void GSSBuffer::initEmpty()
{
  this->gssInternal = false;
  this->buf = new gss_buffer_desc;
  this->buf->length = 0;
  this->buf->value = NULL;
}

void GSSBuffer::freeBufValue()
{
  if (this->buf->length == 0 && this->buf->value != NULL)
    delete[] ( (char *)(this->buf->value) );
  this->buf->length = 0;
  this->buf->value = NULL;
}


/* setValue pattern:
 * 1) Throw an exception when attempting to mutate a buffer supplied by 
 *    GSS itself. 
 * 2) Free the GSS buffer value, if needed.
 * 3) Set the buffer value as needed.
 */
void GSSBuffer::setValue(std::string str)
{
  setValue((char *)str.c_str(), str.size());
}

void GSSBuffer::setValue(char *str)
{
  setValue(str, std::strlen(str));
}

void GSSBuffer::setValue(char *str, int len)
{
  /* Variables */
  /* Error checking */
  if (this->gssInternal)
    throw std::logic_error("Attempting to modify an immutable GSSBuffer");
  
  /* Setup */
  freeBufValue();
  
  /* Main */
  this->buf->value = new char[len];
  std::memcpy( (void *)(this->buf->value), (void *)str, len);
  this->buf->length = len;
  
  /* Cleanup */
  /* Return */
}

void GSSBuffer::setValue(gss_buffer_t gssbuf)
{
  if (this->gssInternal)
    throw std::logic_error("Attempting to modify an immutable GSSBuffer");
  freeBufValue();
  delete buf;
  this->buf = gssbuf;
}
