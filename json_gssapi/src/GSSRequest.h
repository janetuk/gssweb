/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSREQUEST_H
#define GSSREQUEST_H

#include <string>

#include "commands/GSSCommand.h"
#include "util_json.h"

using std::string;

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
