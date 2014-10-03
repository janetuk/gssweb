#include <commands/GSSImportName.h>
#include <commands/GSSInitSecContext.h>
#include <commands/GSSAcquireCred.h>
#include <datamodel/GSSBuffer.h>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <util_json.h>
#include <GSSRequest.h>


using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

int main(int argc, char **argv) {
  /* Variables */
  string output;
  char *input;
  int len;
  ssize_t readTotal, readThisRound, readRemaining;
  
  /* Error checking */
  
  /* Setup */
  
  /* Main processing */
  do 
  {
    // Read 32 bit length
    len = 0;
    readThisRound = readTotal = 0;
    while(4 != readTotal)
    {
      readThisRound = read(0, ((&len) + readTotal), 4 - readTotal);
      readTotal += readThisRound;
    }
    
    // Reads the number of bytes indicated by the above read
    input = new char[len + 1];
    readTotal = readThisRound = 0;
    while (readTotal < len)
    {
      readRemaining = len - readTotal;
      readThisRound = read( 0, &(input[readTotal]), readRemaining);
      if (-1 == readThisRound)
        break;
      else
        readTotal += readThisRound;
    }
    // ... and null-terminate it
    input[len] = '\0';
    
    
    GSSRequest *req = new GSSRequest(string(input));
    req->execute();
    output = req->getResponse();
    len = output.length();
    
    cout.write((char *)&len, 4);
    cout << output;
    cout.flush();
    
  } while(1);
  
  return 0;
}
