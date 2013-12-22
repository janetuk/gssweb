/*
 * Copyright (c) 2013 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "npapiplugin.h"


NPAPIPlugin::NPAPIPlugin(NPNetscapeFuncs *aNPNFuncs, NPPluginFuncs *pFuncs)
{
  /*
   * Setup - Ensure a sane environment
   */  
  if( pFuncs == NULL || 
      pFuncs->size < sizeof(NPNetscapeFuncs) )
    return NPERR_INVALID_FUNCTABLE_ERROR;

  if(HIBYTE(pFuncs->version) > NP_VERSION_MAJOR)
    return NPERR_INCOMPATIBLE_VERSION_ERROR;

  /*
   * Main body:
   *   Tell the browser about the API functions we implement
   */
  pFuncs->newp = NPP_New;
  pFuncs->destroy = NPP_Destroy;
  pFuncs->getvalue = NPP_GetValue;

  return NPERR_NO_ERROR;

}

NPAPIPlugin::~NPAPIPlugin(NPP instance, NPSavedData **save)
{

}


/*
 * Create and destroy an instance of the plugin
 */
NPError NPAPIPlugin::NPP_New(NPMIMEType    pluginType,
                NPP instance, uint16_t mode,
                int16_t argc, char *argn[],
                char *argv[], NPSavedData *saved)
{
  return(NPERR_NO_ERROR);
}

NPError NPAPIPlugin::NPP_Destroy(NPP instance, NPSavedData **save)
{
  return(NPERR_NO_ERROR);
}

NPError NPAPIPlugin::NPP_GetValue(NPP instance,
                     NPPVariable variable,
                     void *value)
{
  switch(variable)
  {
    case NPPVpluginScriptableNPObject:
      cerr << "Received the scriptable object call!\n";
      //value = NPN_CreateObject(instance, )
      break;
  }
  cerr << "Called NPP_GetValue with " << variable << ".\n";
  return NPERR_NO_ERROR;
}
