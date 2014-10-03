/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSDISPLAYSTATUS_H
#define GSSDISPLAYSTATUS_H

#include <gssapi.h>
#include <string>
#include <ostream>

using std::string;

class GSSDisplayStatus
{
public:
    GSSDisplayStatus(OM_uint32 major, 
                     OM_uint32 minor,
		     gss_OID   mech  );
    ~GSSDisplayStatus();
    
    string getMajorMessage() const { return majorMessage; }
    string getMinorMessage() const { return minorMessage; }
    
private:
    string minorMessage;
    string majorMessage;
        
    GSSDisplayStatus& operator= ( const GSSDisplayStatus& other ) {};
};

#endif // GSSDISPLAYSTATUS_H
