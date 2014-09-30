#include <commands/GSSImportName.h>
#include <commands/GSSCreateSecContextCommand.h>
#include <commands/GSSAcquireCred.h>
#include <datamodel/GSSBuffer.h>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <util_json.h>


using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

int main(int argc, char **argv) {
  /* Variables */
  string method, output;
  const char* c_str;
  char *input;
  JSONObject json;
  JSONObject *result;
  json_error_t jsonErr;
  GSSCommand *cmd;
  int len;
  ssize_t readTotal, readThisRound, readRemaining;
  std::streamsize threeTwoBits = 32 / sizeof(string::size_type);
  
  /* Error checking */
  
  /* Setup */
  
  /* Main processing */
  do 
  {
    try 
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
      
      
      // Parse the JSON
      JSONObject json = JSONObject::load(input, 0, &jsonErr);
      delete[] input;
      
      if ( json.get("method").isNull() )
      {
        JSONObject response;
        response.set("method", "unknown");
        response.set("error_message", "Did not find a valid method to execute.");
	output = response.dump();
	len = output.length();
	cout.write((char *)&len, threeTwoBits);
        cout << output << endl;
	continue;
      }

      // Oh, how I wish I could simply use: switch(json.get("method"))
      c_str = json.get("method").string();
      method = c_str;
      if ("gss_import_name" == method)
      {
        cmd = new GSSImportName(&json);
      }
      else if ("gss_create_sec_context" == method)
      {
        cmd = new GSSCreateSecContextCommand(&json);
      }
      else if ("gss_acquire_cred" == method)
      {
        cmd = new GSSAcquireCred(&json);
      }
      else 
      {
        JSONObject response;
        response.set("method", "unknown");
        response.set("error_message", "Did not find a valid method to execute.");
	output = response.dump();
	len = output.length();
	cout.write((char *)&len, threeTwoBits);
        cout << output << endl;
      
        continue;
      }

      cmd->execute();
      result = cmd->toJSON();
      delete cmd;
      
      output = result->dump();
      len = output.length();
      
      cout.write((char *)&len, threeTwoBits);
      cout << output;
      cout.flush();

    }
    catch ( std::bad_alloc& e )
    {
      JSONObject response;
      JSONObject error;
      response.set("method", "unknown");
      response.set("error_message", "Could not parse the input JSON");
      response.set("original message", input);
      error.set("text", jsonErr.text);
      error.set("source", jsonErr.source);
      error.set("line", jsonErr.line);
      error.set("column", jsonErr.column);
      error.set("position", jsonErr.position);
      response.set("error", error);
      output = response.dump();
      len = output.length();
      cout.write((char *)&len, threeTwoBits);
      cout << output << endl;
    }
  } while(1);
  
  return 0;
}
