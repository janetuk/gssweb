/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSOIDSET_H
#define GSSOIDSET_H
#include "GSSOID.h"

class GSSOIDSet
{
public:
    GSSOIDSet();
    ~GSSOIDSet();

    GSSOIDSet& operator= ( const gss_OID_set other );
    void addOID (const GSSOID other );
    void addOID (const gss_OID other );
    gss_OID_set toGss() const { return(set); }
    
private:
    gss_OID_set set;

    GSSOIDSet ( const GSSOIDSet& other ) {};
    void init();
    gss_OID copyOID(gss_OID source, gss_OID destination);
    void releaseOIDSet();
};

#endif // GSSOIDSET_H
