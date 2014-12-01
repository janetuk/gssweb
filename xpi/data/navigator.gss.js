console.log("Bob is here. #1");

var GSSEap = (function () 
  {
    function GSSEap(config)
    {
      console.log("BobBobBob");
    };
    
    return GSSEap;
  }
)();



unsafeWindow.navigator.GSSEap = 
       cloneInto(GSSEap,
                 unsafeWindow, 
                 { cloneFunctions: true, 
                   wrapReflectors: true});

var mesaGSS = createObjectIn(unsafeWindow, {defineAs: "mesaGSS"});
function sayHello(addressee) 
{
  console.log("Saying hello to " + addressee);
  return("Hello, " + addressee);
};
exportFunction(sayHello, mesaGSS, { defineAs: "sayHello" });

console.log("Bob is leaving. #1");

/* 
 * Gives us navigator.gss_eap, but "new navigator.gss_eap()" returns "not a constructor"
unsafeWindow.navigator.gss_eap = cloneInto(GSSEap, unsafeWindow, {cloneFunctions: true});
*/


// unsafeWindow.gss_eap = cloneInto(GSSEap, unsafeWindow);

//var gss_eap = createObjectIn(unsafeWindow.navigator, {defineAs: 'gss_eap'});
//var gss_eap_prototype = createObjectIn(gss_eap, {defineAs: 'prototype'});
//exportFunction(GSSEap.prototype.constructor, gss_eap_prototype, {defineAs: 'constructor'});

//exportFunction(GSSEap, unsafeWindow.navigator, {defineAs: 'gss_eap'});


