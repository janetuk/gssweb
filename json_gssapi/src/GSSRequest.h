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
#include "utils/util_json.h"

using std::string;

/* An exportable function to be called by firefox
 * to process a JSON string
 */
extern "C" {
  JSONGSSAPI_EXPORT char *gss_request(char *json_string);
  JSONGSSAPI_EXPORT void deallocate_reply(char *reply);
}

class GSSRequest
{
public:
  GSSRequest(string jsonString);
  ~GSSRequest();
  
  void execute();
  string getResponse();
  
private:
  string      requestString;
  JSONObject  request;
  JSONObject  response;
  GSSCommand *cmd;
  
  void getCommand();
  void parseJSON();
};

#endif // GSSREQUEST_H
