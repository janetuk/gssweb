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
  GSSCommand(void *fn) : function(fn) {};
  GSSCommand(JSONObject params, void *fn) : function(fn) { };
  
  void *getGSSFunction() { return function; };
  virtual void execute() { std::cout << "\ninside GSSCommand::invoke()\n"; };  
  virtual JSONObject *toJSON() { return new JSONObject(); }
  
protected:
  void *function;
};

#endif // GSSCOMMAND_H
