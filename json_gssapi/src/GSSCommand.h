/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef GSSCOMMAND_H
#define GSSCOMMAND_H

#include "util_json.h"
#include <iostream>

class GSSCommand
{
public:
  GSSCommand() {};
  GSSCommand(JSONObject params) : parameters(params) { };
  
  virtual void execute() { std::cout << "\ninside GSSCommand::invoke()\n"; };  
  virtual JSONObject *toJSON() { return new JSONObject(); }
  
protected:
  JSONObject parameters;
};

#endif // GSSCOMMAND_H
