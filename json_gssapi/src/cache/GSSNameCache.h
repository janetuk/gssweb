/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSNAMECACHE_H
#define GSSNAMECACHE_H

#include <map>
#include <string>

#include "datamodel/GSSName.h"

typedef std::map<std::string, GSSName> NameMap;

class GSSNameCache
{
public:
    static GSSNameCache* instance();
    ~GSSNameCache();
    GSSNameCache& operator= ( const GSSNameCache& other );
    
    std::string store(GSSName &data, const std::string key = "");
    GSSName &retrieve(std::string key);
    NameMap getNames() { return(names); };

protected:
    GSSNameCache();
    GSSNameCache ( const GSSNameCache& other );

private:
    NameMap names;
    
    bool generateKey(std::string &key);
    static GSSNameCache* _instance;
};

#endif // GSSNAMECACHE_H
