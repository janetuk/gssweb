//
#include <iostream>
#ifdef WIN32
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#else
#include <unistd.h>
#include <string.h>
#endif
#include <GSSRequest.h>


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
      readThisRound = _read(0, ((&len) + readTotal), 4 - readTotal);
      readTotal += readThisRound;
    }
    
    // Reads the number of bytes indicated by the above read
    input = new char[len + 1];
    readTotal = readThisRound = 0;
    while (readTotal < len)
    {
      readRemaining = len - readTotal;
      readThisRound = _read( 0, &(input[readTotal]), readRemaining);
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
