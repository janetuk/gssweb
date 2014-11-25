#include <commands/GSSImportName.h>
#include <commands/GSSInitSecContext.h>
#include <commands/GSSAcquireCred.h>
#include <datamodel/GSSBuffer.h>
#include <exception>
#include <iostream>
#include <string>
#ifdef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <share.h>
#else
#include <unistd.h>
#endif
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
  size_t len;
  
  /* Error checking */
  
  /* Setup */
  
  /* Main processing */
#ifdef WIN32
  if (argc < 2) {
      return -1;
  }
  int fd;
  if (_sopen_s(&fd, argv[1], _O_BINARY, _SH_DENYNO, _S_IREAD) != 0)
  {
      cout << "error :" << errno << " opening file: " << argv[1] << "\n";
      return -1;
  }
  struct _stat fs;
  if (_fstat(fd, &fs) != 0) {
      cout << "error: " << errno << " from _fstat.\n";
      return -1;
  }

  FILE *f =_fdopen(fd, "rb");
  if (f == NULL) {
      cout << "error: " << errno << " from _fdopen.\n";
      return -1;
  }
  len = fs.st_size;
  input = new char[len+1];
  size_t count = fread(input, 1, len, f) ;
  if (count != len) {
      cout << "expected " << len << " bytes from fread; got " << count << ".\n";
      return -1;
  }
  fclose(f);
#else
  ssize_t readThisRound;
  size_t readTotal, readRemaining;
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
#endif
    input[len] = '\0';
    
    GSSRequest *req = new GSSRequest(string(input));
    req->execute();
    output = req->getResponse();
    len = output.length();
    
    cout.write((char *)&len, 4);
    cout << output;
    cout.flush();
#ifndef WIN32
  } while(1);
#endif
  return 0;
}
