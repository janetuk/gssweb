/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSREQUEST_H
#define GSSREQUEST_H

#include <string>
#include "jsongssapi_export.h"
#include "commands/GSSCommand.h"
#include "util_json.h"

using std::string;

/* An exportable function to be called by firefox
 * to process a JSON string
 */
extern "C" {
  char *gss_request(char *json_string);
  void deallocate_reply(char *reply);
}

class GSSRequest
{
public:
  JSONGSSAPI_EXPORT GSSRequest(string jsonString);
  JSONGSSAPI_EXPORT ~GSSRequest();
  
  JSONGSSAPI_EXPORT void execute();
  JSONGSSAPI_EXPORT string getResponse();
  
private:
  string      requestString;
  JSONObject  request;
  JSONObject  response;
  GSSCommand *cmd;
  
  void getCommand();
  void parseJSON();
};

#endif // GSSREQUEST_H
