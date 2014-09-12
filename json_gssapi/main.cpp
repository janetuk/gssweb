#include <commands/GSSImportName.h>
#include <commands/GSSCreateSecContextCommand.h>
#include <commands/GSSAcquireCred.h>
#include <datamodel/GSSBuffer.h>
#include <exception>
#include <iostream>
#include <string>
#include <util_json.h>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

int main(int argc, char **argv) {
  /* Variables */
  string input, method;
  const char* c_str;
  JSONObject json;
  JSONObject *result;
  json_error_t jsonErr;
  GSSCommand *cmd;
  
  /* Error checking */
  
  /* Setup */
  
  /* Main processing */
  do 
  {
    try 
    {
      cout << "Give me some JSON: ";
      getline(cin, input);
      
      c_str = input.c_str();
      JSONObject json = JSONObject::load(c_str, 0, &jsonErr);
      
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
        cout << response.dump() << endl;
      
        continue;
      }

      cmd->execute();
      result = cmd->toJSON();
      delete cmd;
      
      cout << result->dump( JSON_INDENT(4) ) << endl;

    }
    catch ( std::bad_alloc )
    {
      JSONObject response;
      JSONObject error;
      response.set("method", "unknown");
      response.set("error_message", "Could not parse the input JSON");
      error.set("text", jsonErr.text);
      error.set("source", jsonErr.source);
      error.set("line", jsonErr.line);
      error.set("column", jsonErr.column);
      error.set("position", jsonErr.position);
      response.set("error", error);
      cout << response.dump() << endl;
    }
  } while(1);
  
  return 0;
}
