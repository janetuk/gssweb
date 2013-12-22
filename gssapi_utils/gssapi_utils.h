#include <gssapi.h>

/**********************************************************************
 **********************************************************************
 ** Data Structures and Typedefs                                     **
 **********************************************************************
 **********************************************************************/
typedef struct gss_status_struct {
    OM_uint32 major;
    OM_uint32 minor;
} *gss_status_t;




/**********************************************************************
 **********************************************************************
 ** Functions                                                        **
 **********************************************************************
 **********************************************************************/

/**********************************************************************
 * Function:  str_to_oid
 * Returns:   0 on success, 1 on error.
 * Arguments: 
 *    status: (OUT) A gss_status_t 
 *    str:    (IN)  The string to convert to an OID
 *    oid:    (OUT) The resultant gss_OID 
 **********************************************************************/
OM_uint32
gss_construct_sec_context(
    gss_status_t,            /* status                                        */
    gss_cred_id_t,           /* cred_handle - reserved for future use         */
    gss_ctx_id_t *,          /* context_handle                                */
    gss_name_t,              /* target_name                                   */ 
    gss_OID,                 /* mech_type                                     */
    OM_uint32,               /* req_flags                                     */
    OM_uint32,               /* time_req                                      */
    gss_channel_bindings_t   /* input_chan_bindings - reserved for future use */
);
