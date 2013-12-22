var widgets = require("sdk/widget");
var tabs = require("sdk/tabs");

const {Cu} = require("chrome");

function myDump(message) {
  dump("\n");
  dump("**********************************************************************\n");
  dump("**********************************************************************\n");
  dump("** " + message + " **\n");
  dump("**********************************************************************\n");
  dump("**********************************************************************\n");
}

myDump("About to load ctypes");
Cu.import("resource://gre/modules/ctypes.jsm");

/************************
 ** Declaring types    **
 ************************/
const gss_buffer_t_struct = ctypes.StructType(
    "gss_buffer_t",
    [
        {"length": ctypes.size_t},
        {"value":  ctypes.void_t.ptr}
    ]
);
const gss_buffer_t = ctypes.PointerType(gss_buffer_t_struct);
const gss_OID_struct = ctypes.StructType(
    "gss_OID", 
    [
        {"length" :   ctypes.uint32_t},
        {"elements" : ctypes.voidptr_t}
    ]
);
const gss_OID = ctypes.PointerType(gss_OID_struct);
const gss_name_t = ctypes.StructType("gss_name_t");

const gss_cred_id_struct = ctypes.StructType("gss_cred_id_struct");
const gss_cred_id_t = ctypes.PointerType(gss_cred_id_struct);

const gss_ctx_id_struct = ctypes.StructType("gss_ctx_id_struct");
const gss_ctx_id_t = ctypes.PointerType(gss_ctx_id_struct);

const gss_channel_bindings_struct = ctypes.StructType(
    "gss_channel_bindings_struct",
    [
	{"initiator_addrtype": ctypes.uint32_t},
	{"initiator_address":  gss_buffer_t_struct},
	{"acceptor_addrtype":  ctypes.uint32_t},
	{"acceptor_address":   gss_buffer_t_struct},
	{"application_data":   gss_buffer_t_struct}
    ]
);
const gss_channel_bindings_t = ctypes.PointerType(gss_channel_bindings_struct);

const gss_status_struct = ctypes.StructType(
    "gss_status_struct",
    [
	{"major": ctypes.uint32_t},
	{"minor": ctypes.uint32_t}
    ]
);
const gss_status_t = ctypes.PointerType(gss_status_struct);


/**********************
 * Utility functions  *
 **********************/
function generate_gss_buffer(buff) {
  myDump("Generate a gss_buffer_t for (" + buff + ")");

  var buffString = ctypes.char.array()(buff);

  var gss_buffer = new gss_buffer_t_struct();
  gss_buffer.length = new ctypes.size_t(buff.length);
  gss_buffer.value = buffString.address();

  myDump("Returning the buffer_t");
  return gss_buffer; 
}

//"1.3.6.1.5.5.2" - spnego
//"1.2.840.113554.1.2.1.4" - nt hostservice

function str_to_oid(oid_str)
{
    var buffer = generate_gss_buffer(oid_str);
    var oid = new gss_OID_struct();
    var oid_ptr = oid.address();
    minStatus = new ctypes.uint32_t(0);
    var majStatus;
    
    myDump("Abotut to str_to_oid");
    majStatus = gss_str_to_oid(
	minStatus.address(), 
	buffer.address(), 
	oid_ptr.address() );
    myDump("Finished the str_to_oid: " + majStatus + "/" + minStatus);
    
    return(oid);
}

function import_name(name)
{
    myDump("Entering import_name(" + name + ")");
    var name_buffer     = generate_gss_buffer(name);
    var input_name_type = new gss_OID();
    var oid             = str_to_oid("1.2.840.113554.1.2.1.4");
                          /* GSS_C_NT_HOSTBASED_SERVICE */
    var output_name_ptr = gss_name_t.ptr(ctypes.UInt64("0x0"));
    var minor           = new ctypes.uint32_t(0);
    var major;
    
    major = gss_import_name(
	minor.address(),
	name_buffer.address(),
	oid.address(),
	output_name_ptr.address()
    );
    myDump("Finished the gss_import_name: " + major + "/" + minor);
    
    return(output_name_ptr);
}


try{
    var libkrb5;
    var libgss_utils;
    try{
	libkrb5 = ctypes.open("libgssapi_krb5.so");
	libgss_utils = ctypes.open("libgssapi_utils.so");
    }
    catch (e) {
	myDump("Could not open utils: " + e);
	libkrb5 = ctypes.open("libgss.dll");
	libgss_utils = ctypes.open("libgssapi_utils.dll");
    }

    myDump("libkrb5 return value: " + libkrb5);
    myDump("libgss_utils return value: " + libgss_utils);
    
    myDump("ctypes.open");

    /**************************************************************
     ** Function definitions
     **************************************************************/
    const gss_str_to_oid = libkrb5.declare("gss_str_to_oid",
        ctypes.default_abi, 
        ctypes.uint32_t,     /* Return value           */
        ctypes.uint32_t.ptr, /* arg: minor_status */
        gss_buffer_t,        /* arg: oid_str */
        gss_OID.ptr);        /* arg: oid */
    myDump("gss_str_to_oid function is " + gss_str_to_oid);

    const gss_import_name = libkrb5.declare("gss_import_name", 
        ctypes.default_abi, 
        ctypes.uint32_t,     /* Return value           */
        ctypes.uint32_t.ptr, /* arg: minor status      */
        gss_buffer_t,        /* arg: input_name_buffer */
        gss_OID,             /* arg: input_name_type   */
        ctypes.PointerType(gss_name_t).ptr );     /* arg: output_name       */
    myDump("gss_import_name is function is " + gss_import_name);

    const gss_init_sec_context = libkrb5.declare("gss_init_sec_context", 
        ctypes.default_abi, 
        ctypes.uint32_t,     /* Return value              */
        ctypes.uint32_t.ptr, /* arg: minor status         */
        gss_cred_id_t,       /* arg: claimant_cred_handle */
        gss_ctx_id_t.ptr,    /* arg: context_handle       */
        gss_name_t.ptr,      /* arg: target_name          */
        gss_OID,             /* arg: mech_type            */
        ctypes.uint32_t,     /* arg: req_flags            */
        ctypes.uint32_t,     /* arg: time_req             */
        gss_channel_bindings_t, /* arg: channel_bindings, reserved for future use */
        gss_buffer_t,        /* arg: input_token          */
        gss_OID.ptr,         /* arg: actual_mech_type     */
        gss_buffer_t,        /* arg: output_token         */
        ctypes.uint32_t.ptr, /* arg: ret_flags            */
        ctypes.uint32_t.ptr  /* arg: time_rec             */
    );
    myDump("gss_init_sec_context function is " + gss_init_sec_context);
    
    const gss_construct_sec_context = libgss_utils.declare(
	"gss_construct_sec_context", 
	ctypes.default_abi,
	ctypes.uint32_t,         /* Return value - boolean                             */
	gss_status_t,            /* arg: Status struct pointer                         */
	gss_cred_id_t,           /* arg: claimant_cred_handle - reserved for future    */
	gss_ctx_id_t.ptr,        /* arg: context_handle, the output of the function    */
	gss_name_t.ptr,          /* arg: target_name - the other endpoint              */
	gss_OID,                 /* arg: mech_type - the desired mechanism type        */
	ctypes.uint32_t,         /* arg: req_flags                                     */
	ctypes.uint32_t,         /* arg: time_req                                      */
	gss_channel_bindings_t   /* arg: input_chan_bindings - reserved for future use */
    );



    var majStatus, minStatus, output_name_ptr;
    minStatus = new ctypes.uint32_t(0);
    
    output_name_ptr = import_name('http@www.project-moonshot.org/PROJECT-MOONSHOT.ORG');
    
    var cred_handle = gss_cred_id_struct.ptr(ctypes.UInt64("0x0"));
    var context_handle = gss_ctx_id_struct.ptr(ctypes.UInt64("0x0"));
    var GSS_C_NO_OID = gss_OID_struct().address();
    var null_mechanism = gss_OID_struct.ptr(ctypes.UInt64("0x0"));
    var channel_bindings = new gss_channel_bindings_struct();
    var input_token = new gss_buffer_t_struct();
    var actual_mech_type = gss_OID();
    var output_token = new gss_buffer_t_struct();
    var ret_flags = new ctypes.uint32_t(0);
    var time_rec = new ctypes.uint32_t(0);
    var status = new gss_status_struct();
    
    var success = gss_construct_sec_context(
	status.address(),
	cred_handle,
	context_handle.address(),
	output_name_ptr,
	null_mechanism,
	ctypes.uint32_t(0),          /* req_flags   */
	ctypes.uint32_t(0),          /* time_req    */
	channel_bindings.address()
    );
    
    myDump("gss_construct_sec_context call: " + success + 
           " [0x" + status.major.toString(16) + "/" + status.minor + "]");
/*
    do
    {

	majStatus = gss_init_sec_context(
	    minStatus.address(),
	    cred_handle,
	    context_handle.address(),
	    output_name_ptr,
	    GSS_C_NO_OID,                /* Default OID * /
	    ctypes.uint32_t(0),          /* req_flags   * /
	    ctypes.uint32_t(0),          /* time_req    * /
	    channel_bindings.address(),
	    input_token.address(),
	    actual_mech_type.address(),
	    output_token.address(),
	    ret_flags.address(),
	    time_rec.address()
	);
        myDump("Finished the init_sec_context: " + majStatus + "/" + minStatus);
        myDump(": " + output_name_ptr);


    } while ((majStatus & 1) == 1)
*/

    myDump("All calls succeeded");
    
    libkrb5.close();
    myDump("close");
    
}
catch (e)
{
    myDump("Caught exception: " + e);
}


