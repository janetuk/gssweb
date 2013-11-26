#include <stdio.h>
#include <string.h>
#include <firefox/npapi.h>
#include <firefox/nptypes.h>
#include <firefox/npfunctions.h>

/*
 * Loading and unloading the library 
 */
NPError NP_Initialize(NPNetscapeFuncs *aNPNFuncs, NPPluginFuncs *aNPPFuncs)
{
  return NPERR_NO_ERROR;
}

void NP_Shutdown(void)
{
  return;
}

/*
 * Create and destroy an instance of the plugin
 */
NPError NPP_New(NPMIMEType    pluginType,
                NPP instance, uint16_t mode,
                int16_t argc, char *argn[],
                char *argv[], NPSavedData *saved)
{
  return(NPERR_NO_ERROR);
}

NPError NPP_Destroy(NPP instance, NPSavedData **save)
{
  return(NPERR_NO_ERROR);
}



/*
 * Register the plugin for MIME type, and name, etc.
 */
#define MIME_TYPES_DESCRIPTION "application/web-shot:wsht:Web plugin for the Moonshot libraries"
const char* NP_GetMIMEDescription(void)
{
  return(MIME_TYPES_DESCRIPTION);
}

NPError NP_GetValue(void *instance, 
                    NPPVariable variable, 
                    void *value)
{
  switch(variable)
  {
  case NPPVpluginNameString:
    *((char **)value) = "GSS-web Plugin\0";
    break;
  case NPPVpluginDescriptionString:
    *((char **)value) = "This plugin facilitates identification of you and authorization to access web resources using GSS-EAP, a standards-compliant mechanism for establishing an identity and access rights within a sophisticated organization.\0";
    break;
  default:
    return NPERR_GENERIC_ERROR;
  }
  
  return NPERR_NO_ERROR;
}
