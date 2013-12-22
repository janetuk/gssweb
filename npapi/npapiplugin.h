/*
 * Copyright (c) 2013 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef NPAPIPLUGIN_H
#define NPAPIPLUGIN_H

#include <firefox/npapi.h>

class NPAPIPlugin
{
public:
NPAPIPlugin(NPNetscapeFuncs *aNPNFuncs, NPPluginFuncs *pFuncs);
~NPAPIPlugin(NPP instance, NPSavedData **save);


NPError NPP_New(NPMIMEType    pluginType,
                NPP instance, uint16_t mode,
                int16_t argc, char *argn[],
                char *argv[], NPSavedData *saved);

NPError NPP_Destroy(NPP instance, NPSavedData **save);

NPError NPP_GetValue(NPP instance,
                     NPPVariable variable,
                     void *value);


};

#endif // NPAPIPLUGIN_H
