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
#include <iostream>
#include <string.h>
#ifdef WIN32
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include <GSSRequest.h>

#ifdef WIN32
#define gssweb_read _read
#else
#define gssweb_read read
#endif

using std::cin;
using std::cout;
using std::endl;
using std::getline;

int main(int argc, char **argv) {
  /* Variables */
  char *input;
  char *output;
  int32_t len;
  
  /* Error checking */
  
  /* Setup */
  
  /* Main processing */
#ifdef WIN32
  _setmode(_fileno(stdin), _O_BINARY);
#endif
  int readThisRound, readTotal, readRemaining;
  do 
  {
    // Read 32 bit length
    len = 0;
    readThisRound = readTotal = 0;
    while(4 != readTotal)
    {
      readThisRound = gssweb_read(0, ((&len) + readTotal), 4 - readTotal);
      readTotal += readThisRound;
    }
    
    // Reads the number of bytes indicated by the above read
    input = new char[len + 1];
    readTotal = readThisRound = 0;
    while (readTotal < len)
    {
      readRemaining = len - readTotal;
      readThisRound = gssweb_read( 0, &(input[readTotal]), readRemaining);
      if (-1 == readThisRound)
        break;
      else
        readTotal += readThisRound;
    }
    // ... and null-terminate it
    input[len] = '\0';
    
    output = gss_request(input);
    len = (int32_t )strlen(output);
    
    cout.write((char *)&len, 4);
    cout << output;
    deallocate_reply(output);
    cout.flush();
  } while(1);
  return 0;
}
