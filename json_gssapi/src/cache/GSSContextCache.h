/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSCONTEXTCACHE_H
#define GSSCONTEXTCACHE_H

#include <map>
#include <string>

#include "datamodel/GSSContext.h"

typedef std::map<std::string, GSSContext> ContextMap;

class GSSContextCache
{
public:
    static GSSContextCache* instance();
    std::string store(GSSContext &data, const std::string key = "");
    GSSContext &retrieve(std::string key);

protected:
    GSSContextCache();
    ~GSSContextCache();

private:
    ContextMap contexts;
    
    bool generateKey(std::string &key);
    
    static GSSContextCache* _instance;
};

#endif // GSSCONTEXTCACHE_H
