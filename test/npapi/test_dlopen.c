#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <firefox/npapi.h>

int main(int argc, char **argv) 
{
   void *lib_handle;
   int (*fn)(void);
   int x;
   char *error;
   
   char* funcs[4] = {"NP_Initialize", "NP_Shutdown", "NPP_New", "NPP_Destroy"};

   lib_handle = dlopen("../../npapi/libWebShot.so", RTLD_LAZY);
   if (!lib_handle) 
   {
      fprintf(stderr, "%s\n", dlerror());
      exit(1);
   }
   
   for (x=0; x<4; x++)
   {
     int retVal = 0;

     fn = dlsym(lib_handle, funcs[x]);
     if ((error = dlerror()) != NULL)  
     {
       fprintf(stderr, "Did not find function %s: %s\n", funcs[x], error);
       exit(2);
     }
     
     retVal = (*fn)();
     printf("%s() => %i\n", funcs[x], retVal);
   }


   /*
   (*fn)(&x);
   printf("Valx=%d\n",x);
   */

   dlclose(lib_handle);
   return 0;
}
