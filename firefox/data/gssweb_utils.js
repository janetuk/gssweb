console.log('Loading gssweb_utils.js - #1');

navigator.generateNonce = function() {
  // TODO: Make sure that we don't have a collision!
  // Random integer in the range [0..(2^32-1)]
  return Math.floor(Math.random() * ( 4294967295 )) ;
}


 
