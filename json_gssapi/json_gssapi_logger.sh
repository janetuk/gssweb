#!/bin/bash

mydir=`/usr/bin/dirname $0`

/usr/bin/tee /tmp/json_gssapi_input.txt | $mydir/json_gssapi.exe | /usr/bin/tee /tmp/json_gssapi_outout.txt
